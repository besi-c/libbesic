#include "besic.h"

#define ALPHABET "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"

#define CONFIG_PATH "/var/besic/besic.conf"
#define DEVICE_PATH "/var/besic/device.conf"
#define TYPE_PATH "/var/besic/type.conf"

#define DEFAULT_API_URL "https://api.besic.org"
#define DEFAULT_DATA_PATH "/var/besic/data"
#define DEFAULT_ARCHIVE_PATH "/var/besic/archive"


// Private functions
void createDeviceFile();
const char* getFileVar(const char* filepath, const char *name, const char *fallback);


//////// Libarary functions

// Get device MAC, generate if not available
const char* getDeviceMAC() {
	const char *name = "MAC";
	const char *fin = getFileVar(DEVICE_PATH, name, 0);
	if (fin == 0) {
		createDeviceFile();
		fin = getFileVar(DEVICE_PATH, name, 0);
	}
	return fin;
}
// Get device password, generate if not available
const char* getDevicePassword() {
	const char *name = "PASSWORD";
	const char *fin = getFileVar(DEVICE_PATH, name, 0);
	if (fin == 0) {
		createDeviceFile();
		fin = getFileVar(DEVICE_PATH, name, 0);
	}
	return fin;
}
// Get device type, 'NONE' if unset
const char* getDeviceType() {
	return getFileVar(TYPE_PATH, "TYPE", "NONE");
}

// Get API URL
const char* getApiUrl() {
	return getFileVar(CONFIG_PATH, "API_URL", DEFAULT_API_URL);
}

// Get path to data directory
const char* getDataPath() {
	return getFileVar(CONFIG_PATH, "DATA_PATH", DEFAULT_DATA_PATH);
}
// Get path to archive directory
const char* getArchivePath() {
	return getFileVar(CONFIG_PATH, "ARCHIVE_PATH", DEFAULT_ARCHIVE_PATH);
}


//////// Private Functions

// Create device file with MAC and random password
void createDeviceFile() {
	// Get MAC
	char mac[18];
	FILE *fp = fopen("/sys/class/net/wlan0/address","r");
	if (!fp) {
		perror("Loading MAC");
		exit(-1);
	}
	fgets(mac, 18, fp);
	fclose(fp);
	//printf("mac = '%s'\n", mac);

	// Open device file
	fp = fopen(DEVICE_PATH, "w");
	if (fp == 0) {
		perror("Creating Device File");
		exit(-1);
	}
	// Write Warning
	fprintf(fp, "# Generated file DO NOT EDIT\n");
	// Write MAC
	fprintf(fp, "MAC=\"");
	for (int i = 0; i < 17; i += 3) {
		fwrite(&mac[i], 2, 1, fp);
	}
	fprintf(fp, "\"\nPASSWORD=\"");
	// Write random password
	srand((unsigned) time(NULL));
	for (int i = 0; i < 32; i++) {
		fputc(ALPHABET[rand() % strlen(ALPHABET)], fp);
	}
	fprintf(fp, "\"\n");
	fclose(fp);
}

// Read variable from ENV format file
const char* getFileVar(const char* filepath, const char *name, const char *fallback) {
	//enum progress state = VAR_NAME;
	FILE *fp;
	fp = fopen(filepath, "r");
	if (fp == 0)
		return fallback;

	char c = 0;
	int i = -1;
	while (1) {
		// Check for start of line
		i++;
		if (c == '\n')
			i = 0;
		c = fgetc(fp);
		if (feof(fp))
			break;

		// If beginning of line check for variable
		if (i == 0) {
			// Check variable name matches
			while (1) {
				if (c != name[i])
					break;
				c = fgetc(fp);
				if (feof(fp))
					break;
				i++;
			}
			if (name[i] != 0)
				continue;
			// Check for = with spaces on either side
			while (c == ' ')
				c = fgetc(fp);
			if (c != '=')
				continue;
			c = fgetc(fp);
			while (c == ' ')
				c = fgetc(fp);
			// Check starting quotation mark
			if (c != '"')
				continue;
			// Count value length
			i = 0;
			do {
				c = fgetc(fp);
				if (feof(fp))
					break;
				if (c == '\n')
					break;
				i++;
			} while (c != '"');
			// If end quotation mark has been reach return value
			if (c == '"') {
				char *fin = malloc(i);
				fseek(fp, -1*i, SEEK_CUR);
				fread(fin, i - 1, 1, fp);
				fin[i] = 0;
				fclose(fp);
				return fin;
			}
		}
	}
	// Return fallback value if end of file reached
	fclose(fp);
	return fallback;
}
