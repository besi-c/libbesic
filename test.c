#include <stdio.h>
#include <string.h>

#include "besic.h"

int main(int argc, char **argv) {
	printf("MAC          = %s\n", getDeviceMAC());
	printf("Type         = %s\n", getDeviceType());
	printf("Beacon ID    = %02x\n", getDeviceID());
	printf("API URL      = %s\n", getApiUrl());
	printf("Data Path    = %s\n", getDataPath());
	printf("Archive Path = %s\n", getArchivePath());

	if (argc == 1)
		return 0;

	if (!strcmp(argv[1], "api")) {
		printf("Heartbeat: %d\n", apiHeartbeat());

		BESIC_Data data;
		data.timestamp = 1;
		data.lux = 0.1;
		data.tmp = 1.0;
		data.prs = 0.0;
		data.hum = 0.0;
		printf("Data Heartbeat: %d\n", apiDataHeartbeat(&data));

		return 0;
	}

	printf("Unknown arguement: '%s'\n", argv[1]);
	return 0;
}
