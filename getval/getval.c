#include <stdio.h>
#include <besic.h>

char compare(const char *a, const char *b) {
	for (int i = 0; ; i++) {
		if (a[i] == b[i]) {
			if (a[i] == 0)
				return 1;
			continue;
		}
		char da = ((a[i] > 64) && (a[i] < 91)) ? 32 : 0;
		char db = ((b[i] > 64) && (b[i] < 91)) ? 32 : 0;
		if (a[i] + da != b[i] + db)
			return 0;
	}
}

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Missing arguement\n");
		return 1;
	}
	if (compare(argv[1], "mac")) {
		printf("%s\n", getDeviceMAC());
	} else if (compare(argv[1], "password")) {
		printf("%s\n", getDevicePassword());
	} else if (compare(argv[1], "type")) {
		printf("%s\n", getDeviceType());
	} else if (compare(argv[1], "api-url")) {
		printf("%s\n", getApiUrl());
	} else if (compare(argv[1], "data-path")) {
		printf("%s\n", getDataPath());
	} else if (compare(argv[1], "archive-path")) {
		printf("%s\n", getArchivePath());
	} else {
		printf("Unknown arguement '%s'\n", argv[1]);
		return 1;
	}
	return 0;
}
