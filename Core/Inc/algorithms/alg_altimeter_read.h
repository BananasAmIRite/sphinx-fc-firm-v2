#ifndef __ALG_ALTIMETER_READ_H__
#define __ALG_ALTIMETER_READ_H__

#include "algorithm.h"

typedef struct {

} AltReadContext; 

void alt_read_start(Algorithm *self); 
void alt_read_loop(Algorithm *self); 
void alt_read_stop(Algorithm *self); 

extern Algorithm alt_read_algorithm; 

#endif