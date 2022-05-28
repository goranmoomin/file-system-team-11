#!/bin/bash

############################################
# OSSPR2022                                #
#                                          #
# Sets up images to boot up QEMU.          #
# You should have built the kernel.        #
############################################

set -Eeu
trap cleanup EXIT

SCRIPTDIR=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null && pwd -P)

msg() {
	printf >&2 "%s\n" "${1:-}"
}

die() {
	local msg=$1
	local code=${2:-1}
	msg "$msg"
	exit "$code"
}

depriv() {
	if [[ -n "${SUDO_USER:-}" ]]; then
		sudo -u "$SUDO_USER" -- "$@"
	else
		"$@"
	fi
}

fetch() {
	if [[ ! -f $1 ]]; then
		msg "$(basename "$1") not found. downloading from $2..."
		depriv curl -sSL "$2" -o "$1"
		msg "download done."
	else
		msg "$(basename "$1") found."
	fi
}

RELEASE_URL='https://download.tizen.org/releases/milestone/tizen/unified/tizen-unified_20201020.1'

ROOTFS_URL="$RELEASE_URL/images/standard/iot-headless-2parts-armv7l-btrfs-rootfs-rpi/tizen-unified_20201020.1_iot-headless-2parts-armv7l-btrfs-rootfs-rpi.tar.gz"
ROOTFS="$SCRIPTDIR/$(basename "$ROOTFS_URL")"

TMUX_URL='https://github.com/mjakob-gh/build-static-tmux/releases/download/v3.1d/tmux.linux-arm64.stripped.gz'
TMUX="$SCRIPTDIR/$(basename "$TMUX_URL")"

IMAGEDIR="$(depriv mktemp -d)"
MOUNTDIR="$(depriv mktemp -d)"

DESTDIR="$SCRIPTDIR/tizen-image"

cleanup() {
	trap - EXIT
	umount "$MOUNTDIR" &>/dev/null || true
	rm -rf "$IMAGEDIR" "$MOUNTDIR"
}

mkramdisk() {
	msg "creating ramdisk.img..."
	msg "mounting..."
	mount -o sync "$1" "$MOUNTDIR"
	sed -i 's/\/bin\/mount -o remount,ro .//' "$MOUNTDIR/usr/sbin/init"
	msg "unmounting..."
	umount "$MOUNTDIR"
	msg "creating ramdisk.img done."
}

mkrootfs() {
	msg "creating rootfs.img..."
	msg "mounting..."
	mount -o sync "$1" "$MOUNTDIR"
        msg "adding tmux..."
        gunzip -c "$TMUX" > "$MOUNTDIR/bin/tmux"
        chmod +x "$MOUNTDIR/bin/tmux"
        msg "adding .profile..."
        cat <<'EOF' > "$MOUNTDIR/root/.bash_profile"
old="$(stty -g)"
stty raw -echo min 0 time 5
printf '\0337\033[r\033[999;999H\033[6n\0338' > /dev/tty
IFS='[;R' read -r _ rows cols _ < /dev/tty
stty "$old"
stty cols "$cols" rows "$rows"
unset old
unset cols
unset rows
export TERM=xterm-256color
EOF
	msg "adding test binaries..."
	find "$SCRIPTDIR/test" -maxdepth 1 -executable -exec cp '{}' "$MOUNTDIR/root" \;
	msg "unmounting..."
	umount "$MOUNTDIR"
	msg "creating rootfs.img done."
}

main() {
	if [[ "$EUID" != '0' ]]; then die "This script must be run as root."; fi

	msg "creating images..."

	fetch "$ROOTFS" "$ROOTFS_URL"
        fetch "$TMUX" "$TMUX_URL"

	msg "unarchiving downloaded images..."
	depriv tar -xzf "$ROOTFS" -C "$IMAGEDIR"
	msg "unarchiving done."

	mkramdisk "$IMAGEDIR/ramdisk.img"
	mkrootfs "$IMAGEDIR/rootfs.img"

	msg "emptying $(basename "$DESTDIR")..."
	depriv rm -rf "$DESTDIR"
	depriv mkdir -p "$DESTDIR"
	msg "copying images to $(basename "$DESTDIR")..."
	depriv cp "$IMAGEDIR"/* "$DESTDIR"
	msg "creating images done."
}

main "$@"
