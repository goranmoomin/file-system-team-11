#include "ext2.h"

#include <linux/gps.h>
#include <linux/gps-fixed.h>

int ext2_is_close_to_current(struct gps_location *loc)
{
	int ret;
	spin_lock(&curloc_lock);
	ret = is_geo_close(loc->lat_integer, loc->lat_fractional,
			   loc->lng_integer, loc->lng_fractional,
			   curloc.lat_integer, curloc.lat_fractional,
			   curloc.lng_integer, curloc.lng_fractional,
			   loc->accuracy, curloc.accuracy);
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
