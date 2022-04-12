#ifndef BESIC_H
#define BESIC_H

#define BESIC_CONFIG_FILE "/etc/besic/besic.conf"
#define BESIC_DEFAULT_API_URL "https://api.besic.org"
#define BESIC_DEFAULT_DATA_PATH "/var/besic/data"
#define BESIC_DEFAULT_ARCHIVE_PATH "/var/besic/archive"


// One environmental data sensors reading
typedef struct {
	long timestamp; // unix time in milliseconds
	float lux;
	float tmp;
	float prs;
	float hum;
} BESIC_Data;


// Make heartbeat API call
int apiHeartbeat();
// Make data heartbeat API call
int apiDataHeartbeat(BESIC_Data *data);
// Make custom heartbeat API call, use null (0) for device defaults
int apiTestHeartbeat(char *mac, char *type);


// Get device MAC address formatted without colons
char* getDeviceMAC();
// Get device type from /etc/besic/type.conf, or NONE if not found
char* getDeviceType();
// Get device ID from API each reboot, or from a cache file otherwise
//   returns the ID on success, -1 on failure
int getDeviceID();


// Get API URL from config file, or default if not set
char* getApiUrl();
// Get data directory path from config file, or default if not set
char* getDataPath();
// Get archive directory path from config file, or default if not set
char* getArchivePath();


#endif
