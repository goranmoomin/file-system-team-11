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

The overall implementation works as described below:

- GPS-related properties are added to the structs `struct ext2_inode`
  and `struct ext2_inode_info`, and the functions `ext2_iget` and
  `ext2_write_inode` that interact with the disk are made aware of the
  new properties.

- A custom permission implementation, `ext2_permission` is implemented
  and added to the various `struct inode_operation`s.

- The new inode operations `set/get_gps_location` are added, and
  appropriate places that modifies `mtime` are modified to check
  whether the `set_gps_location` operation is implemented and calls it
  appropriately.

- The `sys_get_gps_location` syscall uses the `get_gps_location` inode
  operation to get the current location of the provided file.

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

### Concurrency concerns

Updating the current GPS location atomically is trivially solved by
having a global spinlock `curloc_lock`.

Reading and writing the GPS location of an inode must also be atomic
to prevent tearing, and granting permissions that must not be granted.
For this, the spinlock `i_lock` of the inode is locked before reading
and writing the location data of `ext2_inode_info`.

### Creating the file system

As the ext2 file system allows arbitarily sized inode header, creating
the file system didn’t require modifying the mke2fs program; just
creating a zero-ed file, creating an empty ext2 file system with a
bigger inode header (`-I 256` in our case), and adding files in QEMU
with our custom kernel worked fine.

The final file system is added to the repository as `test/proj4.fs`;
the build scripts automatically adds the file system to the root disk
image.

## Possible design improvements

### Are the new `inode_operation`s required?

We have been told in swsnu/osspr2022#118 that the generic GPS
operations are a hard requirement of the project; however it seems
that the places that should updates the inode GPS location are almost
always in the ext2 implementation.

The only place where we use `i_op->set_gps_location` is inside
`generic_update_time`, the default implementation of the `update_time`
inode operation. But we could have easily implemented an
`ext2_update_time` specific to ext2, and not dirty the generic
`update_time` implementation with GPS-related code. (In fact, we
planned to do the exact thing, but we ran out of time after a bunch of
different exams.)

In the same vein, the only use of `i_op->get_gps_location` is inside
the syscall `sys_get_gps_location`, where we get the inode and call
the operation. But we could also have easily implemented to directly
check if the inode is ext2, and read the GPS location by calling
ext2/GPS-specific helper functions.

### Using RCU locks for inode GPS location

We only have to protect GPS location of the inode
