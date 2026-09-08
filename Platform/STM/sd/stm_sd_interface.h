#ifndef __STM_SD_INTERFACE_H__
#define __STM_SD_INTERFACE_H__

#include <stdbool.h>
#include "fatfs.h"
#include "diskio.h"

void sd_init();
void sd_write(char* str);

#endif