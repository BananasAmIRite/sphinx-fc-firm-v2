#include "host_io.h"
#include <stdio.h>


int8_t altimeter_init() {
    printf("altimeter init\r\n"); 
    return 0; 

}
int8_t altimeter_read(AltimeterReadings* readings) {
    printf("altimeter reading...\r\n"); 
    return 0; 

} 

int8_t altimeter_tof_init() {
    printf("tof init\r\n"); 
    return 0; 

}
int8_t altimeter_tof_read(AltimeterReadings* readings) {
    printf("tof reading...\r\n"); 
    return 0; 

}