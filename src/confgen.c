#include <stdio.h>
#include "besic.h"

int main(int argc, char **argv) {
	printf("# BESI-C System Configuration File\n");
	printf("# \n");
	printf("# Uncomment and edit options to change device defaults\n");
	printf("# \n");
	printf("# src: https://github.com/besi-c/libbesic\n");
	printf("\n\n");

	printf("# BESI-C API base URL\n");
	printf("#API_URL=\"%s\"\n", BESIC_DEFAULT_API_URL);
	printf("\n");

	printf("# S3 Backup Bucket\n");
	printf("#S3_BUCKET=\"%s\"\n", BESIC_DEFAULT_S3_BUCKET);
	printf("\n");

	printf("# BESI-C Data Directory\n");
	printf("#DATA_DIR=\"%s\"\n", BESIC_DEFAULT_DATA_DIR);
	printf("\n");

	printf("# BESI-C Archive Directory\n");
	printf("#ARCHIVE_DIR=\"%s\"\n", BESIC_DEFAULT_ARCHIVE_DIR);
	printf("\n");

	printf("# BESI-C Log Directory\n");
	printf("#LOG_DIR=\"%s\"\n", BESIC_DEFAULT_LOG_DIR);
	printf("\n");
}
