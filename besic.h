#ifndef BESIC_H
#define BESIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char* getDeviceMAC();
const char* getDevicePassword();
const char* getDeviceType();

const char* getApiUrl();

const char* getDataPath();
const char* getArchivePath();

#endif
