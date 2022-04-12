#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/stat.h>
#include <curl/curl.h>

#include "besic.h"


#define BESIC_TYPE_FILE "/etc/besic/type.conf"
#define TMP_BEACON_FILE "/tmp/besic/beacon"
#define CACHED_BEACON_FILE "/var/cache/besic/beacon"


// Private functions
int apiRawCall(char *mac, char *type, BESIC_Data *data);
int apiBeaconCall();
int readBeaconFile();
int writeBeaconFile(int id);
char* lowerStr();
char* getFileVar(const char* filepath, const char *name, const char *fallback);



//////// Libarary functions ////////

// Make heartbeat API call
int apiHeartbeat() {
	char *type = lowerStr(getDeviceType());
	if ((strcmp(type, "relay")) && (strcmp(type, "basestation"))) {
		perror("Invalid Device Type for Heartbeat\n");
		return -1;
	}
	return apiRawCall(getDeviceMAC(), type, 0);
}
// Make data heartbeat API call
int apiDataHeartbeat(BESIC_Data *data) {
	char *type = lowerStr(getDeviceType());
	if (strcmp(type, "relay")) {
		perror("Invalid Device Type for Data Heartbeat\n");
		return -1;
	}
	return apiRawCall(getDeviceMAC(), type, data);
}
// Make custom heartbeat API call, use null (0) for device defaults
int apiTestHeartbeat(char *mac, char *type) {
	if (mac == 0)
		mac = getDeviceMAC();
	if (type == 0)
		type = lowerStr(getDeviceType());
	return apiRawCall(mac, type, 0);
}


// Get device MAC address formatted without colons
char* getDeviceMAC() {
	// Get MAC
	char raw[18];
	FILE *fp = fopen("/sys/class/net/wlan0/address", "r");
	if (!fp) {
		perror("Loading MAC");
		exit(-1);
	}
	if (fgets(raw, 18, fp) == 0) {
		perror("Loading MAC");
		exit(-1);
	}
	fclose(fp);
	// Format MAC
	char *mac = malloc(13);
	for (int i = 0; i < 6; i++) {
		memcpy(&mac[i*2], &raw[i*3], 2);
	}
	mac[12] = 0;
	return mac;
}
// Get device type from /etc/besic/type.conf, or NONE if not found
char* getDeviceType() {
	return getFileVar(BESIC_TYPE_FILE, "TYPE", "NONE");
}
// Get device ID from API each reboot, or from a cache file otherwise
//   returns the ID on success, -1 on failure
int getDeviceID() {
	int cached = readBeaconFile();
	// Check tmp file exists, creates once each reboot when API is polled
	if (access(TMP_BEACON_FILE, F_OK) == 0) {
		if (cached >= 0) {
			return cached; // Successful cache use
		}
		// Use API if cache fails
		int api = apiBeaconCall();
		if (api < 0) {
			if (geteuid() == 0) // check root
				remove(TMP_BEACON_FILE);
			return -1; // Both API and cache have failed
		}
		int write = writeBeaconFile(api);
		if (write < 0) // If cache can't be fixed, tmp file should be removed
			if (geteuid() == 0) // check root
				remove(TMP_BEACON_FILE);
		return api;
	}
	apiBeaconCall();
	int api = -1;
	if (api < 0) // If API failes use cache, returns fail (-1) if cache failed
		return cached;
	// Update cache with API value
	int write = writeBeaconFile(api);
	if (write >= 0) { // Create tmp file if update sucessful
		if (geteuid() == 0) { // check root
			mkdir(dirname(TMP_BEACON_FILE), 0777);
			FILE *fp = fopen(TMP_BEACON_FILE, "w");
			if (fp != 0)
				fclose(fp);
		}
	}
	return api;
}


// Get API URL
char* getApiUrl() {
	return getFileVar(BESIC_CONFIG_FILE, "API_URL", BESIC_DEFAULT_API_URL);
}
// Get path to data directory
char* getDataPath() {
	return getFileVar(BESIC_CONFIG_FILE, "DATA_PATH", BESIC_DEFAULT_DATA_PATH);
}
// Get path to archive directory
char* getArchivePath() {
	return getFileVar(BESIC_CONFIG_FILE, "ARCHIVE_PATH", BESIC_DEFAULT_ARCHIVE_PATH);
}



//////// Private Functions ////////

size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {
	if (stream == 0)
		return size*nmemb;
	int write_size = size*nmemb < 16 ? size*nmemb : 16;
	memcpy(stream, ptr, write_size);
	((char*)stream)[write_size] = 0;
	return size*nmemb;
}

// Make curl request to heartbeat endpoint
int apiRawCall(char *mac, char *path, BESIC_Data *data) {
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L); // 10 seconds
	struct curl_slist *hs=NULL;
	hs = curl_slist_append(hs, "Content-Type: application/json");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, hs);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	char curl_str[16];
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_str);
	{
		char url[128];
		sprintf(url, "%s/%s", getApiUrl(), path);
		curl_easy_setopt(curl, CURLOPT_URL, url);
	}

	char body[256];
	if (data == 0) {
		sprintf(body, "{\"mac\":\"%s\"}", mac);
	} else {
		char *fmt = "{\"mac\":\"%s\",\"timeStamp\":%ld,\"data\":{\"lux\":%.2f,\"tmp\":%.2f,\"prs\":%.2f,\"hum\":%.2f}}";
		sprintf(body, fmt, mac, data->timestamp, data->lux, data->tmp, data->prs, data->hum);
	}
	//printf("body: %dB '%s'\n", strlen(body), body);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

	CURLcode res = curl_easy_perform(curl);
	int fin = -1;
	if (res == CURLE_OK) {
		long code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
		if (code == 200) {
			fin = 0;
		} else {
			fin = code;
		}
		//printf("CURL Success %ld\n", code);
	}
	curl_easy_cleanup(curl);
	return fin;
}


// Get beacon ID from API
int apiBeaconCall() {
	// Check that device is a relay
	char *type = getDeviceType();
	if (strcmp(type, "RELAY")) {
		//printf("%s\n", type);
		perror("Only RELAY devices have a beacon ID\n");
		return -1;
	}

	// Setup CURL
	CURL *curl = curl_easy_init();
	char curl_str[16];
	curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L); // 30 seconds
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, curl_str);
	{ // Set URL
		char url[128];
		sprintf(url, "%s/relay/beacon?mac=%s", getApiUrl(), getDeviceMAC());
		curl_easy_setopt(curl, CURLOPT_URL, url);
	}

	int fin = -1;
	CURLcode res = curl_easy_perform(curl);
	if (res == CURLE_OK) {
		long code;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &code);
		//printf("curl %ld: %s\n", code, curl_str);
		if (code == 200)
			fin = atoi(curl_str);
	}
	curl_easy_cleanup(curl);
	return fin;
}
// Get beacon ID from cache file
int readBeaconFile() {
	FILE *fp = fopen(CACHED_BEACON_FILE, "r");
	if (fp == 0)
		return -1;
	char raw[16];
	int i = fread(raw, 1, 15, fp);
	raw[i] = 0;
	fclose(fp);
	return atoi(raw);
}
// Save beacon ID to cache file
int writeBeaconFile(int id) {
	if (id < 0)
		return -1;
	if (id > 0xff)
		return -1;
	if (geteuid() != 0)
		return -1;
	mkdir(dirname(CACHED_BEACON_FILE), 0777);
	FILE *fp = fopen(CACHED_BEACON_FILE, "w");
	if (fp == 0)
		return -1;

	char text[16];
	int len = sprintf(text, "%02d", id);
	fwrite(text, len, 1, fp);
	fclose(fp);
	return 0;
}


// Convert string to lowercase
char* lowerStr(char *str) {
	char *fin = malloc(strlen(str));
	fin[strlen(str)] = 0;
	for (int i = 0; str[i] != 0; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			fin[i] = str[i] + 'a' - 'A';
		} else {
			fin[i] = str[i];
		}
	}
	return fin;
}


// Read variable from ENV format file
char* getFileVar(const char* filepath, const char *name, const char *fallback) {
	//enum progress state = VAR_NAME;
	FILE *fp = fopen(filepath, "r");
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
				int r = fread(fin, 1, i - 1, fp);
				if (r != i - 1)
					return (char*)fallback;
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
