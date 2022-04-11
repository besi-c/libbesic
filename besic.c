#include "besic.h"

#define CONFIG_PATH "/etc/besic/besic.conf"
#define TYPE_PATH "/etc/besic/type.conf"

#define DEFAULT_API_URL "https://api.besic.org"
#define DEFAULT_DATA_PATH "/var/besic/data"
#define DEFAULT_ARCHIVE_PATH "/var/besic/archive"


// Private functions
char* getFileVar(const char* filepath, const char *name, const char *fallback);


//////// Libarary functions

// Get device MAC, generate if not available
char* getDeviceMAC() {
	// Get MAC
	char raw[18];
	FILE *fp = fopen("/sys/class/net/wlan0/address", "r");
	if (!fp) {
		perror("Loading MAC");
		exit(-1);
	}
	fgets(raw, 18, fp);
	fclose(fp);
	// Format MAC
	char *mac = malloc(13);
	for (int i = 0; i < 6; i++) {
		memcpy(&mac[i*2], &raw[i*3], 2);
	}
	mac[12] = 0;
	return mac;
}
// Get device type, 'NONE' if unset
char* getDeviceType() {
	return getFileVar(TYPE_PATH, "TYPE", "NONE");
}

// Get API URL
char* getApiUrl() {
	return getFileVar(CONFIG_PATH, "API_URL", DEFAULT_API_URL);
}
// Get path to data directory
char* getDataPath() {
	return getFileVar(CONFIG_PATH, "DATA_PATH", DEFAULT_DATA_PATH);
}
// Get path to archive directory
char* getArchivePath() {
	return getFileVar(CONFIG_PATH, "ARCHIVE_PATH", DEFAULT_ARCHIVE_PATH);
}


//////// Private Functions

// Read variable from ENV format file
char* getFileVar(const char* filepath, const char *name, const char *fallback) {
	//enum progress state = VAR_NAME;
	FILE *fp;
	fp = fopen(filepath, "r");
	if (fp == 0)
		return (char*)fallback;

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
	return (char*)fallback;
}
