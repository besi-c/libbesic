#include <stdio.h>
#include <string.h>
#include "besic.h"

int main(int argc, char **argv) {
	if (argc == 1) {
		printf("Missing argument\n");
		return 1;
	}
	char *text = besic_secret(argv[1]);
	if (text == 0) {
		printf("Secret '%s' not found\n", argv[1]);
		return 1;
	} else {
		printf("%s\n", text);
		return 0;
	}
}
