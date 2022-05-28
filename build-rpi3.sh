#!/bin/bash

############################################
# OSSPR2022                                #
#                                          #
# Builds your ARM64 kernel.                #
############################################

set -Eeu
trap cleanup EXIT

SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

usage() {
	cat <<EOF
Usage: $(basename -- "${BASH_SOURCE[0]}") [-h] [--noclang] [--noccache]

Build the kernel and test programs.

Available options:

-h, --help
	Print this help message and exit.

--noclang
	Do not use clang even if found.

--noccache
	Do not use ccache even if found.
EOF
	exit
}

msg() {
	printf >&2 "%s\n" "${1:-}"
}

die() {
	local msg=$1
	local code=${2:-1}
	msg "$msg"
	exit "$code"
}

SRCDIR="$SCRIPTDIR"
TESTHDRDIR="$SRCDIR/usr"
TESTSRCDIR="$SRCDIR/test"

ARCH=arm64
CROSS_COMPILE="aarch64-linux-gnu-"
CC="${CROSS_COMPILE}gcc"

NOCLANG=
NOCCACHE=

cleanup() {
    trap - EXIT
}

parse_params() {
	while :; do
		case "${1:-}" in
			-h | --help)
				usage
				;;
			--noclang)
				NOCLANG=on
				;;
			--noccache)
				NOCCACHE=on
				;;
			-?*)
				die "Unknown option: $1"
				;;
			*)
				break
				;;
		esac
		shift
	done
}

main() {
	parse_params "$@"

	if [[ -z "$NOCLANG" ]] && which clang &> /dev/null; then
		msg "clang found. using clang..."
		CC=clang
	fi

	if [[ -z "$NOCCACHE" ]] && which ccache &> /dev/null; then
		msg "ccache found. using ccache..."
		CC="ccache $CC"
	fi

	# Build .config
	msg "building config..."
	make -C "$SRCDIR" ARCH="$ARCH" CC="$CC" CROSS_COMPILE="$CROSS_COMPILE" tizen_bcmrpi3_defconfig
	msg "config build done."

	# Build kernel
	msg "building kernel..."
	make -C "$SRCDIR" ARCH="$ARCH" CC="$CC" CROSS_COMPILE="$CROSS_COMPILE" -j$(nproc)
	msg "kernel build done."

	# Install kernel headers to ./usr
	msg "installing kernel headers to $(basename "$TESTHDRDIR")..."
	make -C "$SRCDIR" INSTALL_HDR_PATH="$TESTHDRDIR" headers_install
	msg "kernel headers install done."

	# Build test program
	msg "rebuilding test programs in $(basename "$TESTSRCDIR")..."
	make -C "$TESTSRCDIR" clean
	make -C "$TESTSRCDIR" CC="$CC" CROSS_COMPILE="$CROSS_COMPILE" HDRDIR="$TESTHDRDIR" all
	msg "test programs build done."

	msg "proceed by running sudo ./setup-images.sh to create images for qemu."
}

main "$@"
