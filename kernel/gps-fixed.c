#include <linux/gps-fixed.h>

#define RADIUS 6371000

fixed64_t cos_to_hav(fixed64_t cos)
{
	return (FIXED_PRECISION - cos) >> 1;
}

fixed64_t calc_hav(const int x_lat_int, const int x_lat_frac,
		   const int x_lng_int, const int x_lng_frac,
		   const int y_lat_int, const int y_lat_frac,
		   const int y_lng_int, const int y_lng_frac)
{
	fixed64_t lat1 = to_fixed(x_lat_int, x_lat_frac);
	fixed64_t lat2 = to_fixed(y_lat_int, y_lat_frac);
	fixed64_t d_lat = lat2 - lat1;
	fixed64_t s_lat = lat2 + lat1;
	fixed64_t lng1 = to_fixed(x_lng_int, x_lng_frac);
	fixed64_t lng2 = to_fixed(y_lng_int, y_lng_frac);
	fixed64_t d_lng = lng2 - lng1;
	fixed64_t t21 = xcos(d_lat); // cos(lat2 - lat1)
	fixed64_t t22 = xcos(s_lat); // cos(lat2 + lat1)
	fixed64_t t11 = cos_to_hav(t21); // sin^2(lat2 - lat1)
	fixed64_t t23 = cos_to_hav(xcos(d_lng)); // sin^2(lng2 - lng1)
	fixed64_t ret = t11 + (xmul(t21 + t22, t23) >> 1);

	return ret ? ret : 1;
}

int is_geo_close(const int x_lat_int, const int x_lat_frac, const int x_lng_int,
		 const int x_lng_frac, const int y_lat_int,
		 const int y_lat_frac, const int y_lng_int,
		 const int y_lng_frac, const int x_acc, const int y_acc)
{
	fixed64_t cur_hav, acc, acc_rad, acc_deg, acc_cos, acc_hav;
	if (x_lat_int == y_lat_int && x_lng_int == y_lng_int &&
	    x_lat_frac == y_lat_frac && x_lng_frac == y_lng_frac) {
		return 1;
	}

	cur_hav = calc_hav(x_lat_int, x_lat_frac, x_lng_int, x_lng_frac,
			   y_lat_int, y_lat_frac, y_lng_int, y_lng_frac);

	acc = to_fixed(x_acc + y_acc, 0);
	acc_rad = div_s64(acc, RADIUS);
	acc_deg = xdeg(acc_rad);
	acc_cos = xcos(acc_deg);
	acc_hav = cos_to_hav(acc_cos);
	return cur_hav < acc_hav;
}
