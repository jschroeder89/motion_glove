#include "BNO055.hpp"
#include "Wire.h"
#include <string.h>

BNO055::BNO055(uint8_t addr):SLAVE_ADDR(addr){}

BNO055::~BNO055(){}

void BNO055::initialize_I2C(uint8_t opr)
{   
    uint8_t data[2] = {0};
    initialize_BNO055(data, opr);
    return;
}

void BNO055::initialize_BNO055(uint8_t *data, uint8_t opr)
{
    read_reg(data, PWR_MODE, 1);
    check_power_mode(&data[0]);
    initialize_operating_mode(opr);
    return;
}

void BNO055::check_power_mode(uint8_t *data)
{
    uint8_t pwr_mode_normal = (data[0] & ~PWR_MODE_NORMAL_MASK);
    if (pwr_mode_normal != PWR_MODE_NORMAL)
    {
        pwr_mode_normal = PWR_MODE_NORMAL;
        write_reg(&pwr_mode_normal, PWR_MODE, 1);
    }
    return;
}

void BNO055::initialize_operating_mode(uint8_t opr_mode)
{
    write_reg(&opr_mode, OPR_MODE, 1);  
    return;
}

void BNO055::select_unit(uint8_t unit) {
    write_reg(&unit, UNIT_SEL, 1);
}

void BNO055::write_reg(uint8_t *data, uint8_t addr, uint8_t len)
{
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(addr);
    for (int i = 0; i < len; i++)
    {
        Wire.write(data[i]);
        //delay(1);
    }
    Wire.endTransmission(true);
    return;
}

void BNO055::read_reg(uint8_t *data, uint8_t addr, uint8_t len)
{
    Wire.beginTransmission(SLAVE_ADDR);
    Wire.write(addr);
    Wire.endTransmission(true);
    //delay(10);
    Wire.requestFrom(SLAVE_ADDR, len);
    for (int i = 0; i < len; i++)
    {
        data[i] = Wire.read();
        //delay(1);
        
        /* if (data[i] < 0x10)
        {
            Serial.print("Data: 0x0");
        }
        else
            Serial.print("Data: 0x");
        Serial.print(data[i], HEX);
        if (addr < 0x10)
        {
            Serial.print(" @Adress: 0x0");
        }
        else
            Serial.print(" @Adress: 0x");
        Serial.print(addr++, HEX);
        Serial.println(); */
    }
    return;
}

void BNO055::get_sensor_data(uint8_t opr, bool format)
{
    switch (opr)
    {
    case OPR_MODE_AMG:
        if (format == NONE)
        {
            data_mode_amg();
        } break;

    case OPR_MODE_IMU:
        if (format == EULE)
        {
            data_mode_fusion_relative_euler();
        } else if (format == QUAT) {
            data_mode_fusion_relative_quaternion();
        } break;
    
    case OPR_MODE_M4G:
        if (format == EULE)
        {
            data_mode_fusion_relative_euler();
        } else if (format == QUAT) {
            data_mode_fusion_relative_quaternion();
        } break;    

    case OPR_MODE_NDOF:
        if (format == EULE)
        {
            data_mode_fusion_relative_euler();
        } else if (format == QUAT) {
            data_mode_fusion_relative_quaternion();
        } break;

    case OPR_MODE_NDOF_FMC_OFF:
        if (format == EULE)
        {
            data_mode_fusion_relative_euler();
        } else if (format == QUAT) {
            data_mode_fusion_relative_quaternion();
        } break;

    default:
        break;
    }
}

void BNO055::data_mode_amg()
{
    uint8_t data[9] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BNO055_ARRAY = doc.to<JsonArray>();
    BNO055_ARRAY.add("BNO055_0x28");
    JsonArray BNO055_DATA = doc.createNestedArray();
    select_unit(UNIT_SEL_ACC_MG);
    get_acc_data(data, BNO055_DATA);
    get_mag_data(data, BNO055_DATA);
    get_gyro_data(data, BNO055_DATA);
    publish_sensor_data(doc);
}

void BNO055::get_sensor_offset() {
    uint8_t data[18] = {0};
    get_acc_offset(&data[0]);
    Serial.print("ACC_Z_OFFSET: ");
    Serial.print(ACC_Z_OFFSET);
    Serial.println("");
}    

void BNO055::get_acc_offset(uint8_t *data) {
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], ACC_OFFSET_X_LSB, 6);
    lsb = data[id++];
    msb = data[id++];
    ACC_X_OFFSET = (((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    ACC_Y_OFFSET = (((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    ACC_Z_OFFSET = (((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::get_acc_data(uint8_t *data, JsonArray& array)
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], ACC_DATA_X_LSB, 6);
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::get_mag_data(uint8_t *data, JsonArray& array)
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], MAG_DATA_X_LSB, 6);
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::get_gyro_data(uint8_t *data, JsonArray& array)
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], GYR_DATA_X_LSB, 6);
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::get_euler_hrp(uint8_t *data, JsonArray &array) {
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], EUL_HEADING_LS, 6);
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::get_quant(uint8_t *data, JsonArray &array) {
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], QUA_DATA_W_LSB, 8);
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

void BNO055::data_mode_fusion_absolute_euler()
{

}

void BNO055::data_mode_fusion_relative_euler() {
    uint8_t data[6] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BNO055_ARRAY = doc.to<JsonArray>();
    BNO055_ARRAY.add("BNO055_0x28");
    JsonArray BNO055_DATA = doc.createNestedArray();
    select_unit(UNIT_SEL_EU_ANG_DEG);
    get_euler_hrp(data, BNO055_DATA);
    publish_sensor_data(doc);
}

void BNO055::data_mode_fusion_absolute_quaternion() {

}

void BNO055::data_mode_fusion_relative_quaternion() { 

}

size_t BNO055::publish_sensor_data(JsonDocument& doc)
{
    size_t num_bytes = serializeJson(doc, Serial);
    Serial.println("");
    return num_bytes;
}
