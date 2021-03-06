#include "BMI160.hpp"
#include <Wire.h>
#include <string.h>

BMI160::BMI160(uint8_t addr):SLAVE_ADDR(addr){}
BMI160::~BMI160(){}

void BMI160::check_acc_range_conf(uint8_t *data) {
    uint8_t acc_range_val = (data[1] & ~ACC_RANGE_MASK);
    if (acc_range_val < 0x03) {
        acc_range_val = ACC_RANGE_2G;
        write_reg(&acc_range_val, ACC_RANGE_REG, 1);
    }
    else
        return;
}

void BMI160::check_acc_us_conf(uint8_t *data)
{
    uint8_t acc_conf_val = (data[0] & ~ACC_US_MASK);
    if (acc_conf_val == ACC_US_ENABLED) {
        acc_conf_val = ACC_US_DISABLED;
        write_reg(&acc_conf_val, ACC_CONF_REG, 1);
    }
    else
        return;
}

void BMI160::check_acc_bw_conf(uint8_t *data)
{
    uint8_t acc_conf_val = (data[0] & ~ACC_BW_MASK);
    if ((acc_conf_val == ACC_BW_NORMAL_MODE) || (acc_conf_val == ACC_BW_OSR2_MODE)) {
        acc_conf_val = ACC_BW_OSR4_MODE;
        write_reg(&acc_conf_val, ACC_CONF_REG, 1);
    }
    else
        return;
}

void BMI160::check_acc_odr_conf(uint8_t *data)
{
    uint8_t acc_conf_val = (data[0] & ~ACC_ODR_MASK);
    if (acc_conf_val < ACC_ODR_1600HZ){
        acc_conf_val = ACC_ODR_1600HZ;
        write_reg(&acc_conf_val, ACC_CONF_REG, 1);
    }
    else
        return;
}

void BMI160::check_gyro_odr_conf(uint8_t *data)
{
    uint8_t gyro_odr_value = (data[0] & ~GYRO_ODR_MASK);
    if (gyro_odr_value != GYRO_ODR_3200HZ)
    {
        gyro_odr_value = GYRO_ODR_3200HZ;
        write_reg(&gyro_odr_value, GYRO_CONF_REG, 1);
    }
    else
        return;
}

void BMI160::check_gyro_bw_conf(uint8_t *data)
{
    uint8_t gyro_bw_value = (data[0] & ~GYRO_BW_MASK);
    if (gyro_bw_value != GYRO_BW_OSR4)
    {
        gyro_bw_value = GYRO_BW_OSR4;
        write_reg(&gyro_bw_value, GYRO_CONF_REG, 1);
    }
    else
        return;
}

void BMI160::check_gyro_range_conf(uint8_t *data)
{
    uint8_t gyro_range_value = (data[0] & ~GYRO_RANGE_MASK);
    if (gyro_range_value < 0x04)
    {
        gyro_range_value = GYRO_RANGE_125;
        write_reg(&gyro_range_value, GYRO_RANGE_REG, 1);
        delay(5);
    }
    else
        return;
}

void BMI160::check_acc_pwr_mode(uint8_t *data)
{
    uint8_t acc_pmu_status = (data[0] & ~ACC_PMU_STATUS_MASK);
    if (acc_pmu_status == 0x10)
        ACC_PWRMODE_NORMAL = true;
    else
        ACC_PWRMODE_NORMAL = false;
    if (ACC_PWRMODE_NORMAL == true)
    {
        return;
    }
    else
    {
        acc_pmu_status = ACC_PWR_NORMAL_MODE;
        write_reg(&acc_pmu_status, BMI160_CMD_REG, 1);
        delay(7);
    }
    return;
}

void BMI160::check_gyro_pwr_mode(uint8_t *data)
{
    uint8_t gyro_pmu_status = (data[0] & ~GYRO_PMU_STATUS_MASK);
    if (gyro_pmu_status == 0x04)
        GYRO_PWRMODE_NORMAL = true;
    else
        GYRO_PWRMODE_NORMAL = false;
    if (GYRO_PWRMODE_NORMAL == true)
    {
        return;
    }
    else
    {
        gyro_pmu_status = GYRO_PWR_NORMAL_MODE;
        write_reg(&gyro_pmu_status, BMI160_CMD_REG, 1);
        delay(85);
    }
    return;
}

void BMI160::initialize_gyro(uint8_t *data)
{
    read_reg(data, GYRO_CONF_REG, 2);
    check_gyro_odr_conf(&data[0]);
    check_gyro_bw_conf(&data[0]);
    check_gyro_range_conf(&data[1]);
    read_reg(data, GYRO_CONF_REG, 2);
    return;
}

void BMI160::initialize_acc(uint8_t *data)
{
    read_reg(data, ACC_CONF_REG, 2);
    check_acc_us_conf(&data[0]);
    check_acc_bw_conf(&data[0]);
    check_acc_odr_conf(&data[0]);
    check_acc_range_conf(&data[1]);
    return;
}

void BMI160::initialize_power_mode(uint8_t *data)
{
    read_reg(data, PMU_STATUS_REG, 1);
    check_acc_pwr_mode(&data[0]);
    check_gyro_pwr_mode(&data[0]);
    return;
}

void BMI160::initialize_I2C()
{
    uint8_t data[2] = {0};
    initialize_power_mode(data);
    initialize_acc(data);
    initialize_gyro(data);
    return;
}

void BMI160::latch_int_reg() {
    uint8_t data[1] = {0};
    data[0] = LATCH_INT;
    write_reg(&data[0], INT_LATCH_REG, 1);
    return;
}

void BMI160::initialize_interrupt_engines() 
{
    uint8_t data[2] = {0}; 
    data[0] = INT1_OUTPUT_EN;
    write_reg(&data[0], INT_OUT_CTRL_REG, 1);
    data[0] = S_D_TAP_ENABLE_BYTE;
    data[1] = 0xF0;
    write_reg(&data[0], INT_MAP_0_REG, 2);
    data[0] = 0x30;
    write_reg(&data[0], INT_EN_0_REG, 1);
    data[0] = INT_TAP_0_CONF_BYTE;
    data[1] = INT_TAP_1_CONF_BYTE;
    write_reg(&data[0], INT_TAP_0_REG, 2);
    data[0] = LATCH_INT;
    write_reg(&data[0], INT_LATCH_REG, 1);
    return;
}

void BMI160::interrupt_detection_index() {
    int c = 0;
    uint8_t data[1] = {0};
    read_reg(&data[0], INT_STATUS_0_REG, 1);
    while (data[0] == 0) {
        // Serial.println(c);
        c++;
        if (c > 1000) {
            break;
        } 
        
        if (data[0] == 1) return;
        read_reg(&data[0], INT_STATUS_0_REG, 1);
    } 
    DynamicJsonDocument doc(128);
    JsonArray BMI160_ARRAY = doc.to<JsonArray>();
    BMI160_ARRAY.add("INDEX");
    JsonArray BMI160_DATA = doc.createNestedArray();
    if (data[0] == INT_S_TAP) {
        BMI160_DATA.add("LEFT_CLICK");
        publish_sensor_data(doc);
    } else if ((data[0] == INT_D_TAP) || (data[0] == 0x30) ) {
        BMI160_DATA.add("DOBBLE_CLICK");
        publish_sensor_data(doc);
    }
    return;
}

void BMI160::interrupt_detection_middle()
{
    uint8_t data[1] = {0};
    read_reg(&data[0], INT_STATUS_0_REG, 1);
    Serial.println(data[0], HEX);
    while (data[0] == 0) {
        read_reg(&data[0], INT_STATUS_0_REG, 1);
    }
    write_reg(&data[0], INT_LATCH_REG, 1);
    DynamicJsonDocument doc(32);
    if (data[0] == INT_S_TAP) {
        doc["MIDDLE"] = "RIGHT_CLICK";
        publish_sensor_data(doc);
    }
    return;
}

void BMI160::write_reg(uint8_t *data, uint8_t addr, uint8_t len)
{
    if ((ACC_PWRMODE_NORMAL == true) || (GYRO_PWRMODE_NORMAL == true)) {
        Wire.beginTransmission(SLAVE_ADDR);
        Wire.write(addr);
        for (int i = 0; i < len; i++) {
            Wire.write(data[i]);
            //delay(1);
        }
        Wire.endTransmission(true);
    }
    else
    {
        for (int i = 0; i < len; i++)
        {
            Wire.beginTransmission(SLAVE_ADDR);
            Wire.write(addr);
            Wire.write(data[i]);
            //delay(1);
            Wire.endTransmission(true);
        }
    }
    return;
}

void BMI160::read_reg(uint8_t *data, uint8_t addr, uint8_t len)
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

void BMI160::get_sensor_data()
{
    uint8_t data[6] = {0};
    DynamicJsonDocument doc(256);
    JsonArray BMI160_ARRAY = doc.to<JsonArray>();
    BMI160_ARRAY.add("INDEX");
    JsonArray BMI160_DATA = doc.createNestedArray();
    // get_gyro_data(data, BMI160_DATA);
    get_acc_data(data, BMI160_DATA);
    publish_sensor_data(doc);
    return;
}


void BMI160::get_acc_data(uint8_t *data, JsonArray& array) 
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], 0x12, 6);
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

void BMI160::get_gyro_data(uint8_t *data, JsonArray& array)
{
    uint8_t lsb;
    uint8_t msb;
    uint8_t id = 0;
    read_reg(&data[0], 0x0C, 6);
    lsb = data[id++];
    msb = data[id++];
    //array[GYRO_X] = ((int16_t)((msb << 8) | lsb));
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    //array[GYRO_Y] = ((int16_t)((msb << 8) | lsb));
    array.add(((uint16_t)((msb << 8) | lsb)));
    lsb = data[id++];
    msb = data[id++];
    //array[GYRO_Z] = ((int16_t)((msb << 8) | lsb));
    array.add(((uint16_t)((msb << 8) | lsb)));
    return;
}

size_t BMI160::publish_sensor_data(JsonDocument& doc) 
{
    size_t num_bytes = serializeJson(doc, Serial);
    // Serial.println("");
    return num_bytes;
}