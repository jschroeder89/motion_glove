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

uint8_t BNO055::check_acc_calib_status(uint8_t *data) 
{
    uint8_t status = (data[0] & ~ACC_CALIB_MASK);
    return status;
}

uint8_t BNO055::check_gyr_calib_status(uint8_t *data)
{
    uint8_t status = (data[0] & ~GYR_CALIB_MASK);
    return status;
}

uint8_t BNO055::check_mag_calib_status(uint8_t *data)
{
    uint8_t status = (data[0] & ~MAG_CALIB_MASK);
    return status;
}

void BNO055::check_calibration_status() {
    uint8_t data[1] = {0};
    uint8_t status = 0;
    read_reg(data, CALIB_STAT_REG, 1);
    if (data[0] == SYSTEM_FULLY_CALIBRATED) {
        Serial.println("System fully calibrated!");
        return;
    }    
    Serial.println("Begin magnetometer calibration!\n");
    status = check_mag_calib_status(data);
    while (status != MAG_FULLY_CALIBRATED) {
        status = check_mag_calib_status(data);
        switch (status) {
        case MAG_33_CALIBRATED:
            Serial.println("MAG: 33 percent calibrated!");
            break;
        case MAG_66_CALIBRATED:
            Serial.println("MAG: 66 percent calibrated!");
            break;
        case MAG_FULLY_CALIBRATED:
            Serial.println("MAG: 100 percent calibrated!");
            break;

        default:
            break;
        }
    }
    Serial.println("Begin accelerometer calibration!\n");
    status = check_acc_calib_status(data);
    while (status != ACC_FULLY_CALIBRATED) {
        status = check_acc_calib_status(data);
        switch (status) {
        case ACC_33_CALIBRATED:
            Serial.println("ACC: 33 percent calibrated!");
            break;
        case ACC_66_CALIBRATED:
            Serial.println("ACC: 66 percent calibrated!");
            break;
        case ACC_FULLY_CALIBRATED:
            Serial.println("ACC: 100 percent calibrated!");
            break;

        default:
            break;
        }
    }
    Serial.println("Begin gyroscope calibration!\n");
    status = check_gyr_calib_status(data);
    while (status != GYR_FULLY_CALIBRATED) {
        status = check_gyr_calib_status(data);
        switch (status) {
        case GYR_33_CALIBRATED:
            Serial.println("GYR: 33 percent calibrated!");
            break;
        case GYR_66_CALIBRATED:
            Serial.println("GYR: 66 percent calibrated!");
            break;
        case GYR_FULLY_CALIBRATED:
            Serial.println("GYR: 100 percent calibrated!");
            break;

        default:
            break;
        }
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
        if (format == EULE) {
            data_mode_fusion_relative_euler();
        } else if (format == QUAT) {
            data_mode_fusion_relative_quaternion();
        } else if (format == NONE) {

        } break;
    case OPR_MODE_LIN_ACC:
        if (format == NONE) {
            data_mode_linear_acceleration();
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
    BNO055_ARRAY.add("BNO055");
    JsonArray BNO055_DATA = doc.createNestedArray();
    select_unit(UNIT_SEL_ACC_MG);
    get_acc_data(data, BNO055_DATA);
    get_mag_data(data, BNO055_DATA);
    get_gyro_data(data, BNO055_DATA);
    publish_sensor_data(doc);
}

void BNO055::get_acc_data(uint8_t *data, JsonArray& array)
{
    uint16_t lsb;
    uint16_t msb;
    uint16_t id = 0;
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
    uint16_t lsb;
    uint16_t msb;
    uint16_t id = 0;
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
    uint16_t lsb;
    uint16_t msb;
    uint16_t id = 0;
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
    uint16_t lsb;
    uint16_t msb;
    uint16_t id = 0;
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

void BNO055::get_quant(uint8_t *data, JsonArray &array) 
{
    uint16_t lsb;
    uint16_t msb;
    uint16_t id = 0;
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

void BNO055::get_acc_lin(uint8_t *data, JsonArray &array) 
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], LIA_DATA_X_LSB, 6);
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

void BNO055::data_mode_linear_acceleration()
{
    uint8_t data[6] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BNO055_ARRAY = doc.to<JsonArray>();
    BNO055_ARRAY.add("BNO055");
    JsonArray BNO055_DATA = doc.createNestedArray();
    select_unit(UNIT_SEL_ACC_MG);
    get_acc_lin(data, BNO055_DATA);
    publish_sensor_data(doc);
    return;
}

void BNO055::data_mode_fusion_absolute_euler()
{

}

void BNO055::data_mode_fusion_relative_euler() {
    uint8_t data[6] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BNO055_ARRAY = doc.to<JsonArray>();
    BNO055_ARRAY.add("MAIN");
    JsonArray BNO055_DATA = doc.createNestedArray();
    // select_unit(UNIT_SEL_EU_ANG_DEG);
    get_euler_hrp(data, BNO055_DATA);
    publish_sensor_data(doc);
    return;
}

void BNO055::data_mode_fusion_absolute_quaternion() {

}

void BNO055::data_mode_fusion_relative_quaternion() {
    uint8_t data[8] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BNO055_ARRAY = doc.to<JsonArray>();
    BNO055_ARRAY.add("BNO055_0x28");
    JsonArray BNO055_DATA = doc.createNestedArray();
    get_quant(data, BNO055_DATA);
    publish_sensor_data(doc);
    return;
}

size_t BNO055::publish_sensor_data(JsonDocument& doc)
{
    size_t num_bytes = serializeJson(doc, Serial);
    Serial.println("");
    return num_bytes;
}
