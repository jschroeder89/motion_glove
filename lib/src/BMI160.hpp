#ifndef BMI160_HPP
#define BMI160_HPP

#include <Arduino.h>
#include <ArduinoJson.h>

#define GYRO_X_LSB                  UINT8_C(0x0C)
#define GYRO_X_MSB                  UINT8_C(0x0D)
#define GYRO_Y_LSB                  UINT8_C(0x0E)
#define GYRO_Y_MSB                  UINT8_C(0x0F)
#define GYRO_Z_LSB                  UINT8_C(0x10)
#define GYRO_Z_MSB                  UINT8_C(0x11)
#define ACC_X_LSB                   UINT8_C(0x12)
#define ACC_X_MSB                   UINT8_C(0x13)
#define ACC_Y_LSB                   UINT8_C(0x14)
#define ACC_Y_MSB                   UINT8_C(0x15)
#define ACC_Z_LSB                   UINT8_C(0x16)
#define ACC_Z_MSB                   UINT8_C(0x17)

#define GYRO_X                      UINT8_C(0)
#define GYRO_Y                      UINT8_C(1)
#define GYRO_Z                      UINT8_C(2)
#define ACC_X                       UINT8_C(3)
#define ACC_Y                       UINT8_C(4)
#define ACC_Z                       UINT8_C(5)

/* ####### ACC CONF ####### */        
#define ACC_CONF_REG                UINT8_C(0x40)
#define ACC_ODR_25HZ                UINT8_C(0x06)
#define ACC_ODR_50HZ                UINT8_C(0x07)
#define ACC_ODR_100HZ               UINT8_C(0x08)
#define ACC_ODR_200HZ               UINT8_C(0x09)
#define ACC_ODR_400HZ               UINT8_C(0x0A)
#define ACC_ODR_800HZ               UINT8_C(0x0B)
#define ACC_ODR_1600HZ              UINT8_C(0x0C)
#define ACC_ODR_MASK                UINT8_C(0x0F)
#define ACC_US_MASK                 UINT8_C(0x7F)
#define ACC_US_ENABLED              UINT8_C(0x80)
#define ACC_US_DISABLED             UINT8_C(0x00)
#define ACC_BW_NORMAL_MODE          UINT8_C(0x20)
#define ACC_BW_OSR2_MODE            UINT8_C(0x10)
#define ACC_BW_OSR4_MODE            UINT8_C(0x00)
#define ACC_BW_MASK                 UINT8_C(0x8F)
#define ACC_PWR_NORMAL_MODE         UINT8_C(0x11)
#define ACC_RANGE_REG               UINT8_C(0x41)
#define ACC_RANGE_2G                UINT8_C(0x03)
#define ACC_RANGE_4G                UINT8_C(0x05)
#define ACC_RANGE_8G                UINT8_C(0x08)
#define ACC_RANGE_MASK              UINT8_C(0xF0)
#define ACC_PMU_STATUS_MASK         UINT8_C(0xCF)

/* ####### GYRO CONF ####### */
#define GYRO_CONF_REG               UINT8_C(0x42)
#define GYRO_ODR_MASK               UINT8_C(0xF0)
#define GYRO_ODR_3200HZ             UINT8_C(0x0D)
#define GYRO_ODR_1600HZ             UINT8_C(0x0C)
#define GYRO_ODR_800HZ              UINT8_C(0x0B)
#define GYRO_ODR_400HZ              UINT8_C(0x0A)
#define GYRO_ODR_200HZ              UINT8_C(0x09)
#define GYRO_ODR_100HZ              UINT8_C(0x08)
#define GYRO_ODR_50HZ               UINT8_C(0x07)
#define GYRO_ODR_25HZ               UINT8_C(0x06)
#define GYRO_BW_MASK                UINT8_C(0xF0)
#define GYRO_BW_OSR4                UINT8_C(0x00)
#define GYRO_RANGE_REG              UINT8_C(0x43)
#define GYRO_RANGE_MASK             UINT8_C(0xF8)
#define GYRO_RANGE_2000             UINT8_C(0x00)
#define GYRO_RANGE_1000             UINT8_C(0x01)
#define GYRO_RANGE_500              UINT8_C(0x02)
#define GYRO_RANGE_250              UINT8_C(0x03)
#define GYRO_RANGE_125              UINT8_C(0x04)
#define GYRO_PWR_NORMAL_MODE        UINT8_C(0x15)
#define GYRO_PMU_STATUS_MASK        UINT8_C(0xF3)

/* ####### INTERRUPT REGISTERS & MASKS ####### */
#define INT_STATUS_0_REG            UINT8_C(0x1C)
#define INT_S_TAP                   UINT8_C(0x20)
#define INT_D_TAP                   UINT8_C(0x10)
#define INT_EN_0_REG                UINT8_C(0x50)
#define S_D_TAP_ENABLE_BYTE         UINT8_C(0xFF)
#define INT1_OUTPUT_EN              UINT8_C(0x04)
#define INT_OUT_CTRL_REG            UINT8_C(0x53)
#define INT_MAP_0_REG               UINT8_C(0x55)
#define INT_TAP_0_REG               UINT8_C(0x63)
#define INT_TAP_1_REG               UINT8_C(0x64)
#define INT_TAP_0_CONF_BYTE         UINT8_C(0x80)
#define INT_TAP_1_CONF_BYTE         UINT8_C(0x01)
#define INT_0_DISABLE_BYTE          UINT8_C(0x00)
#define INT_0_ENABLE_BYTE           UINT8_C(0x04)
#define LATCH_INT                   UINT8_C(0x1A)
#define INT_LATCH_REG               UINT8_C(0x54)

/* ####### BMI160 REGISTERS & MASKS ####### */
#define RESET_MASK                  UINT8_C(0xFF)
#define BMI160_CMD_REG              UINT8_C(0x7E)
#define PMU_STATUS_REG              UINT8_C(0x03)
#define ACC_Z_LOW_REG               UINT8_C(0x16)
#define ACC_Z_HIGH_REG              UINT8_C(0x17)

class BMI160
{
private:
    bool ACC_PWRMODE_NORMAL = false;
    bool GYRO_PWRMODE_NORMAL = false;
    uint8_t SLAVE_ADDR;
public:
    BMI160(uint8_t addr);
    ~BMI160();
    void initialize_I2C();
    void initialize_interrupt_engines();
    void latch_int_reg();
    void interrupt_detection_index();
    void interrupt_detection_middle();
    void interrupt_test();
    void read_int_status_reg(uint8_t *data);
    void read_reg(uint8_t *data, uint8_t addr, uint8_t len);
    void write_reg(uint8_t *data, uint8_t addr, uint8_t len);
    void check_acc_range_conf(uint8_t *data);
    void check_acc_us_conf(uint8_t *data);
    void check_acc_bw_conf(uint8_t *data);
    void check_acc_odr_conf(uint8_t *data);
    void initialize_acc(uint8_t *data);
    void check_gyro_odr_conf(uint8_t *data);
    void check_gyro_bw_conf(uint8_t *data);
    void check_gyro_range_conf(uint8_t *data);
    void initialize_gyro(uint8_t *data);
    void check_acc_pwr_mode(uint8_t *data);
    void check_gyro_pwr_mode(uint8_t *data);
    void initialize_power_mode(uint8_t *data);
    void get_acc_data(uint8_t *data, JsonArray& array);
    void get_gyro_data(uint8_t *data, JsonArray& array);
    void get_sensor_data();
    size_t publish_sensor_data(JsonDocument& doc);
    bool interruptTriggerd = false;
};

#endif