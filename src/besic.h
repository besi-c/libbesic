#ifndef BESIC_H
#define BESIC_H

#define BESIC_CONFIG_FILE "/etc/besic/besic.conf"
#define BESIC_DEFAULT_API_URL "https://api.besic.org"
#define BESIC_DEFAULT_S3_BUCKET "relays"
#define BESIC_DEFAULT_DATA_DIR "/var/besic/data"
#define BESIC_DEFAULT_ARCHIVE_DIR "/var/besic/archive"
#define BESIC_DEFAULT_LOG_DIR "/var/log/besic"


// One environmental data sensors reading
typedef struct {
	long long timestamp; // unix time in milliseconds
	float lux;
	float tmp;
	float prs;
	float hum;
} besic_data;


// Make heartbeat API call
int besic_heartbeat();
// Make data heartbeat API call
int besic_data_heartbeat(besic_data *data);
// Make custom heartbeat API call, use null (0) for device defaults
int besic_custom_heartbeat(char *mac, char *path, besic_data *data);


// Get device MAC address formatted without colons
char* besic_device_mac();
// Get device type from /etc/besic/type.conf, or NONE if not found
char* besic_device_type();
// Get device ID from API each reboot, or from a cache file otherwise
//   returns the ID on success, -1 on failure
int besic_device_id();

// Get secret from secrets.conf file, return null (0) if not found
char* besic_secret(const char *name);


// Get API URL from config file, or default if not set
char* besic_api_url();
// Get S3 bucket from config file, or default if not set
char* besic_s3_bucket();

// Get data directory path from config file, or default if not set
char* besic_data_dir();
// Get archive directory path from config file, or default if not set
char* besic_archive_dir();
// Get log directory path from config file, or default if not set
char* besic_log_dir();


#endif
