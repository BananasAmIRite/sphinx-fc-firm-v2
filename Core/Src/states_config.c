#include "states_config.h"
#include "alg_altimeter_read.h"
 
extern Algorithm alt_read_algorithm;

State idle_state = {
    .algorithms = {
        &alt_read_algorithm
    },
    .algorithm_count = 1, 
    .alg_enabled = 0xFF
};


State flight_state = {
    .algorithms = {
        &alt_read_algorithm
    },
    .algorithm_count = 1, 
    .alg_enabled = 0xFF
};