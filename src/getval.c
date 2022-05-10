#include <stdio.h>
#include "besic.h"

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
		printf("Missing argument\n");
		return 1;
	}
	if (compare(argv[1], "mac")) {
		printf("%s\n", besic_device_mac());
	} else if (compare(argv[1], "type")) {
		printf("%s\n", besic_device_type());
	} else if (compare(argv[1], "id")) {
		printf("%02x\n", besic_device_id());
	} else if (compare(argv[1], "api-url")) {
		printf("%s\n", besic_api_url());
	} else if (compare(argv[1], "s3-bucket")) {
		printf("%s\n", besic_s3_bucket());
	} else if (compare(argv[1], "data-dir")) {
		printf("%s\n", besic_data_dir());
	} else if (compare(argv[1], "archive-dir")) {
		printf("%s\n", besic_archive_dir());
	} else if (compare(argv[1], "log-dir")) {
		printf("%s\n", besic_log_dir());
	} else {
		printf("Unknown argument '%s'\n", argv[1]);
		return 1;
	}
	return 0;
}
