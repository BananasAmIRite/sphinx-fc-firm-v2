#include "stm_sd_interface.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern SD_HandleTypeDef hsd1;
extern uint8_t retSD;

FIL file;
DIR dir;
FILINFO filinfo;
uint32_t file_number = 0;
char file_name[32] = {0};
volatile FRESULT sd_mount_result = FR_OK;
volatile FRESULT sd_mkfs_result = FR_OK;
volatile FRESULT sd_open_result = FR_OK;
volatile FRESULT sd_write_result = FR_OK;
volatile FRESULT sd_close_result = FR_OK;
volatile UINT sd_bytes_written = 0;

volatile uint8_t sd_detected = 0;
volatile HAL_StatusTypeDef sd_hal_init_status = HAL_OK;
volatile uint32_t sd_hal_error = 0;
volatile HAL_SD_CardStateTypeDef sd_hal_card_state = HAL_SD_CARD_DISCONNECTED;
volatile uint8_t sd_hal_ready_timeout = 0;
volatile uint8_t sd_link_result = 0;
volatile DSTATUS sd_disk_status_before = STA_NOINIT;
volatile DSTATUS sd_disk_init_result = STA_NOINIT;
volatile DSTATUS sd_disk_status_after = STA_NOINIT;

void sd_init() {
    BYTE mkfs_work[_MAX_SS];
    FRESULT res;
    uint32_t start_ms;
    HAL_Delay(100);

    // Force low-level SD init first so FatFs does not fail with STA_NOINIT.
    HAL_SD_DeInit(&hsd1);
    HAL_Delay(20);
    sd_hal_init_status = HAL_SD_Init(&hsd1);
    sd_hal_error = HAL_SD_GetError(&hsd1);
    sd_hal_card_state = HAL_SD_GetCardState(&hsd1);
    sd_hal_ready_timeout = 0;
    if (sd_hal_init_status != HAL_OK) {
        sd_mount_result = FR_NOT_READY;
        return;
    }

    // Wait until the card reports transfer state before mounting.
    start_ms = HAL_GetTick();
    while (HAL_SD_GetCardState(&hsd1) != HAL_SD_CARD_TRANSFER) {
        if ((HAL_GetTick() - start_ms) > 500) {
            sd_hal_error = HAL_SD_GetError(&hsd1);
            sd_hal_card_state = HAL_SD_GetCardState(&hsd1);
            sd_hal_ready_timeout = 1;
            sd_mount_result = FR_NOT_READY;
            return;
        }
    }
    sd_hal_card_state = HAL_SD_GetCardState(&hsd1);
    sd_link_result = retSD;
    sd_disk_status_before = disk_status(0);
    sd_disk_init_result = disk_initialize(0);
    sd_disk_status_after = disk_status(0);

    res = f_mount(&SDFatFS, SDPath, 1);
    sd_mount_result = res;
    if (res == FR_NO_FILESYSTEM) {
        res = f_mkfs("0:", FM_ANY, 0, mkfs_work, sizeof(mkfs_work));
        sd_mkfs_result = res; 
        if (res == FR_OK) {
            res = f_mount(&SDFatFS, SDPath, 1);
            sd_mount_result = res;
        }
    }
    if (res != FR_OK) {
        return;
    }

    res = f_opendir(&dir, "/");
    if (res == FR_OK) {
        while (1) {
            res = f_readdir(&dir, &filinfo);
            if((res == FR_OK) && (filinfo.fname[0]!= 0)) {
                file_number++;
            }
            if (res != FR_OK || filinfo.fname[0] == 0) break;
        }
    }
    f_closedir(&dir);
    sprintf(file_name, "rec%d.csv", file_number);

    res = f_open(&file, file_name, FA_WRITE | FA_CREATE_ALWAYS);
    sd_open_result = res;
    if (res != FR_OK) {
        return;
    }
}

void sd_write(char* str) {
    if (sd_open_result != FR_OK) {
        return;
    }
    UINT bytes_written;
    FRESULT res = f_write(&file, str, strlen(str), &bytes_written);
    sd_write_result = res;
    f_sync(&file);
}