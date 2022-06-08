# Project 4: Bye, File System!

## Building the project

TLDR: the below shell command will build the kernel and test programs,
create the images, and boot QEMU.

```shellsession
$ sudo -v && ./build-rpi3.sh && sudo ./setup-images.sh && ./qemu.sh
```

`build-rpi3.sh` checks `clang` and `ccache` and sets `CC`
appropriately before compiling the config and kernel. It also installs
the UAPI headers to `usr/include` and compiles the test programs in
`test/` with the installed headers. `setup-images.sh` downloads the
root file system required for boot, and copies test binaries, tmux,
and the test file systems named `*.fs` to the images. Finally
`qemu.sh` boots QEMU with the appropriate arguments.

## Implementation overview

The following files were modified or added:

- `fs/ext2/dir.c`, `fs/ext2/ialloc.c`, `fs/inode.c`:
- `fs/ext2/ext2.h`: Add appropriate properties to `struct ext2_inode`
  and `struct ext2_inode_info`, add function declarations
- `fs/ext2/file.c`: Add implementations of `permission` and gps
  location operations.
- `fs/ext2/gps.c`: Implement set, get gps location operations.
- `fs/ext2/inode.c`: Update gps on setsize, add loading and writing
  inode to disk, add permission implementation.
- `fs/ext2/namei.c`: Add implementations of operations to dir inode
  operations.
- `include/linux/fs.h`: Add `set_gps_location`, `get_gps_location` to
  `struct inode_operations`.

- `test/gpsupdate.c`
- `test/print_loc.c`
- `proj4.fs`, `test/proj4.fs`: Test file system

- Miscellaneous files: `include/uapi/asm-generic/unistd.h`,
  `include/linux/syscalls.h`,

- `include/linux/gps-fixed.h`, `kernel/gps-fixed.c`

- `kernel/gps.c`, `include/linux/gps.h`

### Why are location operations needed?

Blah…
