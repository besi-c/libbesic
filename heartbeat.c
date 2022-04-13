#include <stdio.h>
#include <string.h>
#include "besic.h"

int end(int res) {
	if (res < 0) {
		printf("libbesic error\n");
		return 2;
	} else if (res == 0) {
		printf("200 OK\n");
		return 0;
	}
	printf("Error %d\n", res);
	return 1;
}

int main(int argc, char **argv) {
	if (argc == 1) {
		return end(besic_heartbeat());
	}
	char *mac = 0;
	char *type = 0;
	for (int i = 1; i < argc; i++) {
		if ((!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help"))) {
			printf("besic-heartbeat [options]\n");
			printf("\n");
			printf("By default, makes a heartbeat with the device's MAC address for its device type\n");
			printf("\n");
			printf("Options:\n");
			printf("  -m <m>, --mac <mac>    Override the default MAC address\n");
			printf("  -p <p>, --path <path>  Override the default URL path (sets device type)\n");
			return 0;
		} else if ((!strcmp(argv[i], "-p")) || (!strcmp(argv[i], "--path"))) {
			i++;
			type = argv[i];
		} else if ((!strcmp(argv[i], "-m")) || (!strcmp(argv[i], "--mac"))) {
			i++;
			mac = argv[i];
		} else {
			printf("Invalid argument '%s'", argv[i]);
			return 1;
		}
	}
	return end(besic_custom_heartbeat(mac, type, 0));
}
