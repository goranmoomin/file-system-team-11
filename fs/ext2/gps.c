#include "ext2.h"

#include <linux/gps.h>
#include <linux/fixp-arith.h>

/* 6378100 * PI / 180 */
#define EXT2_GPS_DEG_TO_MET 111319

int is_current_gps_location(struct gps_location *loc)
{
	int ret = 0;
	s32 tmp;

	spin_lock(&curloc_lock);
	tmp = ((s64)(fixp_sin32(loc->lat_integer)) *
	       fixp_sin32(curloc.lat_integer)) /
	      S32_MAX;
	tmp += ((s64)(fixp_cos32(loc->lat_integer)) *
		fixp_cos32(curloc.lat_integer) / S32_MAX) *
	       fixp_cos32(abs(loc->lng_integer - curloc.lng_integer)) / S32_MAX;
	ret = tmp > fixp_cos32(1);
	/* fixp_cos32((loc->accuracy + curloc.accuracy) / EXT2_GPS_DEG_TO_MET); */
	spin_unlock(&curloc_lock);
	return ret;
}

int ext2_set_gps_location(struct inode *inode)
{
	struct ext2_inode_info *ei = EXT2_I(inode);

	spin_lock(&curloc_lock);

	ei->i_lat_integer = curloc.lat_integer;
	ei->i_lat_fractional = curloc.lat_fractional;
	ei->i_lng_integer = curloc.lng_integer;
	ei->i_lng_fractional = curloc.lng_fractional;
	ei->i_accuracy = curloc.accuracy;

	spin_unlock(&curloc_lock);

	return 0;
}

int ext2_get_gps_location(struct inode *inode,
			  struct gps_location *gps_location)
{
	struct ext2_inode_info *ei = EXT2_I(inode);

	gps_location->lat_integer = ei->i_lat_integer;
	gps_location->lat_fractional = ei->i_lat_fractional;
	gps_location->lng_integer = ei->i_lng_integer;
	gps_location->lng_fractional = ei->i_lng_fractional;
	gps_location->accuracy = ei->i_accuracy;

	return 0;
}
