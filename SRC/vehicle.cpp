#include "vehicle.h"
#include <iostream>

using namespace std;

void _VEHICLE::print_on_console(void)
{
    std::cout << "Vehicle ID=" << this->ID << "\t" << this->capacity << std::endl;
}
