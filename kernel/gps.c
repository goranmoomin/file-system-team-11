#include <linux/fs.h>
#include <linux/namei.h>
#include <linux/gps.h>
#include <linux/syscalls.h>
#include <linux/types.h>

struct gps_location curloc = {};
DEFINE_SPINLOCK(curloc_lock);

static inline is_valid_gps_location(struct gps_location *loc)
{
	if (loc->lat_integer == 90 && loc->lat_fractional != 0) {
		return false;
	}

	if (loc->lng_integer == 180 && loc->lng_fractional != 0) {
		return false;
	}

	if (!(-90 <= loc->lat_integer && loc->lat_integer <= 90 &&
	      -180 <= loc->lng_integer && loc->lng_integer <= 180)) {
		return false;
	}

	if (!(0 <= loc->lat_fractional && loc->lat_fractional < 1000000 &&
	      0 <= loc->lng_fractional && loc->lng_fractional < 1000000)) {
		return false;
	}

	if (!(0 <= loc->accuracy && loc->accuracy < 1000)) {
		return false;
	}

	return true;
}

SYSCALL_DEFINE1(set_gps_location, struct gps_location __user *, loc)
{
	struct gps_location newloc;

	if (loc == NULL) {
		return -EINVAL;
	}

	if (copy_from_user(&newloc, loc, sizeof(newloc))) {
		return -EFAULT;
	};

	if (!is_valid_gps_location(&newloc)) {
		return -EINVAL;
	}

	spin_lock(&curloc_lock);
	curloc = newloc;
	spin_unlock(&curloc_lock);

	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char __user *, pathname,
		struct gps_location __user *, loc)
{
	struct path path;
	struct gps_location buf;
	int (*get_gps_location)(struct inode *, struct gps_location *);
	int ret;

	if (!pathname || !loc) {
		return -EINVAL;
	}

	if ((ret = user_path(pathname, &path))) {
		return ret;
	}

	if (!(get_gps_location =
		      path.dentry->d_inode->i_op->get_gps_location)) {
		return -ENODEV;
	}

	get_gps_location(path.dentry->d_inode, &buf);

	if (copy_to_user(loc, &buf, sizeof(buf))) {
		return -EFAULT;
	}

	return 0;
}
