import datetime
import time


cdef extern from "besic.h":

    ctypedef struct besic_data:
        long long timestamp; # unix time in milliseconds
        float lux;
        float tmp;
        float prs;
        float hum;

    int besic_heartbeat();
    int besic_data_heartbeat(besic_data *data);
    int besic_custom_heartbeat(char *mac, char *path, besic_data *data);


    char* besic_device_mac();
    char* besic_device_type();
    int besic_device_id();
    char* besic_secret(const char *name);


    char* besic_api_url();
    char* besic_s3_bucket();

    char* besic_data_dir();
    char* besic_archive_dir();
    char* besic_log_dir();



# Struct Equivalent
class Data:
    def __init__(self):
        self.timestamp = int(time.time_ns() / 1000)
        self.lux = 0.0
        self.tmp = 0.0
        self.prs = 0.0
        self.hum = 0.0

    def __str__(self):
        dt = datetime.datetime.fromtimestamp(self.timestamp/1e6)
        return dt.isoformat("_", "milliseconds") + f'=[lux:{self.lux},tmp:{self.tmp},prs:{self.prs},hum:{self.hum}]'


# Functions
def heartbeat() -> int:
    return besic_heartbeat()

def data_heartbeat(data: Data = None) -> int:
    if data:
        d = {
            "timestamp": data.timestamp,
            "lux": data.lux,
            "tmp": data.tmp,
            "prs": data.prs,
            "hum": data.hum,
        }
    else:
        d = None
    return besic_data_heartbeat(<besic_data*>d)

def custom_heartbeat(mac: str = None, path: str = None, data: Data = None) -> int:
    if len(mac):
        m = str.encode(mac)
    else:
        m = None
    if len(path):
        p = str.encode(path)
    else:
        p = None
    if data:
        d = {
            "timestamp": data.timestamp,
            "lux": data.lux,
            "tmp": data.tmp,
            "prs": data.prs,
            "hum": data.hum,
        }
    else:
        d = None
    return besic_custom_heartbeat(m, p, <besic_data*>d)



def device_mac() -> str:
    return str(besic_device_mac())

def device_type() -> str:
    return str(besic_device_type())

def device_id() -> int:
    return besic_device_id()

def secret(name: str) -> str:
    s = besic_secret(str.encode(name))
    if s == NULL:
        return ""
    else:
        return s.decode()


def api_url() -> str:
    return besic_api_url().decode()

def s3_bucket() -> str:
    return besic_s3_bucket().decode()

def data_dir() -> str:
    return besic_data_dir().decode()

def archive_dir() -> str:
    return besic_archive_dir().decode()

def log_dir() -> str:
    return besic_log_dir().decode()
