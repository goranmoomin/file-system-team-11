#ifndef _LINUX_GPS_H
#define _LINUX_GPS_H

#include <linux/spinlock.h>

struct gps_location {
	int lat_integer;
	int lat_fractional;
	int lng_integer;
	int lng_fractional;
	int accuracy;
};

extern struct gps_location curloc;
extern spinlock_t curloc_lock;

#endif /* _LINUX_GPS_H */
