#include <linux/gps.h>
#include <linux/syscalls.h>

SYSCALL_DEFINE1(set_gps_location, struct gps_location *, loc)
{
	return 0;
}

SYSCALL_DEFINE2(get_gps_location, const char *, pathname, struct gps_location *,
		loc)
{
	return 0;
}
