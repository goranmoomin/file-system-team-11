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

- GPS properties are added to the structures `struct ext2_inode` and
  `struct ext2_inode_info`
  - The functions `ext2_iget` and `ext2_write_inode` that interact
    with the disk are made aware of the new properties.

- A custom permission function `ext2_permission` adds additional GPS
  location restrictions based on `generic_permission`.
  - The `struct inode_operations` for ext2 files and directories are
    updated to use them.

- New inode operations `set/get_gps_location` are added and
  implemented for ext2.
  - `mtime` updates are modified to call either the `set_gps_location`
    inode operation only when implemented, or the
    `ext2_set_gps_location` function directly depending on whether the
    code is generic or not.

- The `sys_get_gps_location` syscall uses `user_path` to get the inode
  for the provided path and calls the inode operation
  `get_gps_location` to get the current location of the provided file.

The following kernel files were modified or added:

- `fs/ext2/dir.c`, `fs/ext2/ialloc.c`, `fs/ext2/inode.c`,
  `fs/inode.c`: Add the appropriate calls to `set_gps_location`.
- `fs/ext2/ext2.h`: Add properties to the structures `struct
  ext2_inode` and `struct ext2_inode_info`.
- `fs/ext2/file.c`, `fs/ext2/namei.c`: Add the inode operation
  implementations to `struct inode_operations` structures.
- `fs/ext2/gps.c`: Implement the GPS inode operations for ext2.
- `fs/ext2/inode.c`: Update the functions `ext2_iget` and
  `ext2_write_inode` to get and write the new properties.
- `include/linux/fs.h`: Add the new inode operations
  `set/get_gps_location`.

- `kernel/gps.c`, `include/linux/gps.h`: Add the new syscalls
  `sys_set_gps_location` and `sys_get_gps_location`.
- `include/uapi/asm-generic/unistd.h`, `include/linux/syscalls.h`:
  Assign syscall numbers to the new syscalls.
- `include/linux/gps-fixed.h`, `kernel/gps-fixed.c`: Add the provided
  fixed point location calculation code.

### Addressing concurrency concerns

Updating the current GPS location can be trivially synchronized by
having a global spinlock `curloc_lock`.

Reading from and writing to the GPS location of an inode must also be
synchronized to prevent data tearing, which might grant permissions
that should not be granted. For this, the spinlock `i_lock` of the
inode is locked during reads and writes of the location data of
`struct ext2_inode_info`. Note that `curloc_lock` is expected to be
locked before locking the inode.

### Creating the sample file system

As the ext2 file system allows arbitrarily sized inode headers,
creating the file system didn’t require modifying the mke2fs program;
creating an empty file with `dd`, creating an empty ext2 file system
with a bigger inode header (`-I 256` in our case), and adding files
from QEMU with our custom kernel worked fine.

The sample file system is added to the repository as `test/proj4.fs`;
the build scripts automatically add it to the root disk image.

## Possible design improvements

### Are the new `inode_operation`s required?

We have been told in swsnu/osspr2022#118 that the generic GPS
operations are a hard requirement of the project; however, it seems
that the places to update the GPS locations are almost always specific
to the ext2 implementation, and hence don’t require adding a generic
inode operation.

The only use of `i_op->set_gps_location` is inside the function
`generic_update_time`, the default implementation of the `update_time`
inode operation. But we could have easily implemented an
`ext2_update_time` specific to ext2, and avoided dirtying the generic
`update_time` implementation with GPS-related code. (In fact, we
planned to update the code with this approach, but we ran out of time
after a bunch of different exams.)

In the same vein, the only use of `i_op->get_gps_location` is inside
the syscall `sys_get_gps_location`, where we get the inode and call
the operation. But we could also have easily implemented to directly
check if the inode is ext2, and read the GPS location by calling the
ext2-specific functions.

### Using RCU locks for inode GPS location

The biggest concurrency concern on the GPS location updates is that
the location updates must be atomic. We thought that it might be
possible to avoid locking in `ext2_permission` at all by using RCU
locks and having an atomically replaced pointer to a `struct
gps_location` in the structure `struct ext2_inode_info`. We’re not
sure if it’s allowed to have a pointer in the inode structure, and the
specification is quite clear on where and what properties we have to
add to the inode structures; hence we did not go further on this idea.
