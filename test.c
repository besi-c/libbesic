#include <stdio.h>

#include "besic.h"

int main() {
	printf("MAC          = %s\n", getDeviceMAC());
	printf("Type         = %s\n", getDeviceType());
	printf("API URL      = %s\n", getApiUrl());
	printf("Data Path    = %s\n", getDataPath());
	printf("Archive Path = %s\n", getArchivePath());
	return 0;
}
