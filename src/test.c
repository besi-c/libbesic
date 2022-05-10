#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#include "besic.h"

int main(int argc, char **argv) {
	printf("MAC          = %s\n", besic_device_mac());
	printf("Type         = %s\n", besic_device_type());
	printf("Beacon ID    = %02x\n", besic_device_id());
	printf("API URL      = %s\n", besic_api_url());
	printf("Data Path    = %s\n", besic_data_dir());
	printf("Archive Path = %s\n", besic_archive_dir());

	if (argc == 1)
		return 0;

	if (!strcmp(argv[1], "api")) {
		besic_data data;
		struct timeval time;
		gettimeofday(&time, NULL);
		data.timestamp = time.tv_sec*1000LL + time.tv_usec/1000LL;
		data.lux = 0.0;
		data.tmp = -273.15;
		data.prs = 0.0;
		data.hum = 100.0;

		int res = besic_data_heartbeat(&data);
		if (res == -1) {
			printf("Heartbeat: %d\n", besic_heartbeat());
		} else {
			printf("Data Heartbeat: %d\n", res);
		}
		return 0;
	}

	printf("Unknown argument: '%s'\n", argv[1]);
	return 0;
}

