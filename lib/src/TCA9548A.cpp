#include "TCA9548A.hpp"
#include "Wire.h"


TCA9548A::TCA9548A(){}

TCA9548A::~TCA9548A(){}

void TCA9548A::set_i2c_addr(uint8_t i2c_addr) {
    I2C_ADDR = i2c_addr;
}

void TCA9548A::select_bus(uint8_t bus_id) {
    Wire.beginTransmission(I2C_ADDR);
    Wire.write(1 << bus_id);
    Wire.endTransmission(true);
}
