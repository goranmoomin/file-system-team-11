#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NR_SET_GPS_LOCATION 294

struct gps_location {
	int lat_integer;
	int lat_fractional;
	int lng_integer;
	int lng_fractional;
	int accuracy;
};

int main(int argc, char **argv)
{
	if (argc != 6) {
		fprintf(stderr,
			"Usage: %s lat_integer lat_fractional lng_integer lng_fractional accuracy\n",
			argv[0]);
		return EXIT_FAILURE;
	}

	struct gps_location newloc = {
		.lat_integer = atoi(argv[1]),
		.lat_fractional = atoi(argv[2]),
		.lng_integer = atoi(argv[3]),
		.lng_fractional = atoi(argv[4]),
		.accuracy = atoi(argv[5]),
	};

	long ret = syscall(NR_SET_GPS_LOCATION, &newloc);
	if (ret < 0) {
		perror("Failed to set current location");
		return ret;
	}

	return EXIT_SUCCESS;
}
