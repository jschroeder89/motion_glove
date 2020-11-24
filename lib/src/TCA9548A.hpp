#ifndef TCA9548A_HPP
#define TCA9548A_HPP
#include <Arduino.h>
#include "BMI160.hpp"

class TCA9548A 
{
private:
    uint8_t CHANNEL_ID;
    uint8_t I2C_ADDR;
public:
    TCA9548A();
    ~TCA9548A();
    void set_i2c_addr(uint8_t i2c_addr);
    void select_bus(uint8_t bus_id);
};



#endif