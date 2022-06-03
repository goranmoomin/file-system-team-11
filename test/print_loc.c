#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_GET_GPS_LOCATION 295

struct gps_location {
	int lat_integer;
	int lat_fractional;
	int lng_integer;
	int lng_fractional;
	int accuracy;
};

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s pathname\n", argv[0]);
		return EXIT_FAILURE;
	}

	struct gps_location loc;
	long ret = syscall(NR_GET_GPS_LOCATION, argv[1], &loc);
	if (ret < 0) {
		perror("Failed to get location");
		return ret;
	}
	printf("Latitude = %.6f, Longitude = %.6f\n"
	       "Accuracy = %d [m]\n"
	       "Google Link = https://maps.google.com/?q=%.6f,%.6f\n",
	       loc.lat_integer + (double)loc.lat_fractional / 1000000,
	       loc.lng_integer + (double)loc.lng_fractional / 1000000,
	       loc.accuracy,
	       loc.lat_integer + (double)loc.lat_fractional / 1000000,
	       loc.lng_integer + (double)loc.lng_fractional / 1000000);

	return EXIT_SUCCESS;
}
