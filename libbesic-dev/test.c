#include <stdio.h>

#include "besic.h"

int main() {
	printf("mac %s\n", getDeviceMAC());
	printf("password     = %s\n", getDevicePassword());
	printf("Type         = %s\n", getDeviceType());
	printf("API URL      = %s\n", getApiUrl());
	printf("Data Path    = %s\n", getDataPath());
	printf("Archive Path = %s\n", getArchivePath());
	return 0;
}
