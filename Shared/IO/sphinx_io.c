#include "sphinx_io.h"

void sphinx_io_init(SphinxIO *sphinx_io_inst) {
    sphinx_io_inst->altimeter_init(); 
    sphinx_io_inst->altimeter_tof_init(); 
}