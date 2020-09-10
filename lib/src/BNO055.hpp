#ifndef BNO055_HPP
#define BNO055_HPP

#include <Arduino.h>
#include <ArduinoJson.h>

#define CHIP_ID                 UINT8_C(0x00) // should be 0xA0
#define ACC_ID                  UINT8_C(0x01)  // should be 0xFB
#define MAG_ID                  UINT8_C(0x02)  // should be 0x32
#define GYRO_ID                 UINT8_C(0x03) // should be 0x0F
#define SW_REV_ID_LSB           UINT8_C(0x04)
#define SW_REV_ID_MSB           UINT8_C(0x05)
#define BL_REV_ID               UINT8_C(0x06)
#define PAGE_ID                 UINT8_C(0x07)
#define ACC_DATA_X_LSB          UINT8_C(0x08)
#define ACC_DATA_X_MSB          UINT8_C(0x09)
#define ACC_DATA_Y_LSB          UINT8_C(0x0A)
#define ACC_DATA_Y_MSB          UINT8_C(0x0B)
#define ACC_DATA_Z_LSB          UINT8_C(0x0C)
#define ACC_DATA_Z_MSB          UINT8_C(0x0D)
#define MAG_DATA_X_LSB          UINT8_C(0x0E)
#define MAG_DATA_X_MSB          UINT8_C(0x0F)
#define MAG_DATA_Y_LSB          UINT8_C(0x10)
#define MAG_DATA_Y_MSB          UINT8_C(0x11)
#define MAG_DATA_Z_LSB          UINT8_C(0x12)
#define MAG_DATA_Z_MSB          UINT8_C(0x13)
#define GYR_DATA_X_LSB          UINT8_C(0x14)
#define GYR_DATA_X_MSB          UINT8_C(0x15)
#define GYR_DATA_Y_LSB          UINT8_C(0x16)
#define GYR_DATA_Y_MSB          UINT8_C(0x17)
#define GYR_DATA_Z_LSB          UINT8_C(0x18)
#define GYR_DATA_Z_MSB          UINT8_C(0x19)
#define EUL_HEADING_LS          UINT8_C(0x1A)
#define EUL_HEADING_MS          UINT8_C(0x1B)
#define EUL_ROLL_LSB            UINT8_C(0x1C)
#define EUL_ROLL_MSB            UINT8_C(0x1D)
#define EUL_PITCH_LSB           UINT8_C(0x1E)
#define EUL_PITCH_MSB           UINT8_C(0x1F)
#define QUA_DATA_W_LSB          UINT8_C(0x20)
#define QUA_DATA_W_MSB          UINT8_C(0x21)
#define QUA_DATA_X_LSB          UINT8_C(0x22)
#define QUA_DATA_X_MSB          UINT8_C(0x23)
#define QUA_DATA_Y_LSB          UINT8_C(0x24)
#define QUA_DATA_Y_MSB          UINT8_C(0x25)
#define QUA_DATA_Z_LSB          UINT8_C(0x26)
#define QUA_DATA_Z_MSB          UINT8_C(0x27)
#define LIA_DATA_X_LSB          UINT8_C(0x28)
#define LIA_DATA_X_MSB          UINT8_C(0x29)
#define LIA_DATA_Y_LSB          UINT8_C(0x2A)
#define LIA_DATA_Y_MSB          UINT8_C(0x2B)
#define LIA_DATA_Z_LSB          UINT8_C(0x2C)
#define LIA_DATA_Z_MSB          UINT8_C(0x2D)
#define GRV_DATA_X_LSB          UINT8_C(0x2E)
#define GRV_DATA_X_MSB          UINT8_C(0x2F)
#define GRV_DATA_Y_LSB          UINT8_C(0x30)
#define GRV_DATA_Y_MSB          UINT8_C(0x31)
#define GRV_DATA_Z_LSB          UINT8_C(0x32)
#define GRV_DATA_Z_MSB          UINT8_C(0x33)
#define TEMP                    UINT8_C(0x34)
#define CALIB_STAT              UINT8_C(0x35)
#define ST_RESULT               UINT8_C(0x36)
#define INT_STATUS              UINT8_C(0x37)
#define SYS_CLK_STATUS          UINT8_C(0x38)
#define SYS_STATUS              UINT8_C(0x39)
#define SYS_ERR                 UINT8_C(0x3A)
#define UNIT_SEL                UINT8_C(0x3B)
#define OPR_MODE                UINT8_C(0x3D)
#define PWR_MODE                UINT8_C(0x3E)
#define SYS_TRIGGER             UINT8_C(0x3F)
#define TEMP_SOURCE             UINT8_C(0x40)
#define AXIS_MAP_CONFI          UINT8_C(0x41)
#define AXIS_MAP_SIGN           UINT8_C(0x42)
#define ACC_OFFSET_X_LSB        UINT8_C(0x55)
#define ACC_OFFSET_X_MSB        UINT8_C(0x56)
#define ACC_OFFSET_Y_LSB        UINT8_C(0x57)
#define ACC_OFFSET_Y_MSB        UINT8_C(0x58)
#define ACC_OFFSET_Z_LSB        UINT8_C(0x59)
#define ACC_OFFSET_Z_MSB        UINT8_C(0x5A)
#define MAG_OFFSET_X_LSB        UINT8_C(0x5B)
#define MAG_OFFSET_X_MSB        UINT8_C(0x5C)
#define MAG_OFFSET_Y_LSB        UINT8_C(0x5D)
#define MAG_OFFSET_Y_MSB        UINT8_C(0x5E)
#define MAG_OFFSET_Z_LSB        UINT8_C(0x5F)
#define MAG_OFFSET_Z_MSB        UINT8_C(0x60)
#define GYR_OFFSET_X_LSB        UINT8_C(0x61)
#define GYR_OFFSET_X_MSB        UINT8_C(0x62)
#define GYR_OFFSET_Y_LSB        UINT8_C(0x63)
#define GYR_OFFSET_Y_MSB        UINT8_C(0x64)
#define GYR_OFFSET_Z_LSB        UINT8_C(0x65)
#define GYR_OFFSET_Z_MSB        UINT8_C(0x66)
#define ACC_RADIUS_LSB          UINT8_C(0x67)
#define ACC_RADIUS_MSB          UINT8_C(0x68)
#define MAG_RADIUS_LSB          UINT8_C(0x69)
#define MAG_RADIUS_MSB          UINT8_C(0x6A)
//
// BNO055 Page 1
#define PAGE_ID                 UINT8_C(0x07)
#define ACC_CONFIG              UINT8_C(0x08)
#define MAG_CONFIG              UINT8_C(0x09)
#define GYRO_CONFIG_0           UINT8_C(0x0A)
#define GYRO_CONFIG_1           UINT8_C(0x0B)
#define ACC_SLEEP_CONFIG        UINT8_C(0x0C)
#define GYR_SLEEP_CONFIG        UINT8_C(0x0D)
#define INT_MSK                 UINT8_C(0x0F)
#define INT_EN                  UINT8_C(0x10)
#define ACC_AM_THRES            UINT8_C(0x11)
#define ACC_INT_SETTINGS        UINT8_C(0x12)
#define ACC_HG_DURATION         UINT8_C(0x13)
#define ACC_HG_THRESH           UINT8_C(0x14)
#define ACC_NM_THRESH           UINT8_C(0x15)
#define ACC_NM_SET              UINT8_C(0x16)
#define GYR_INT_SETTINGS        UINT8_C(0x17)
#define GYR_HR_X_SET            UINT8_C(0x18)
#define GYR_DUR_X               UINT8_C(0x19)
#define GYR_HR_Y_SET            UINT8_C(0x1A)
#define GYR_DUR_Y               UINT8_C(0x1B)
#define GYR_HR_Z_SET            UINT8_C(0x1C)
#define GYR_DUR_Z               UINT8_C(0x1D)
#define GYR_AM_THRESH           UINT8_C(0x1E)
#define GYR_AM_SET              UINT8_C(0x1F)
#define PWR_MODE_NORMAL_MASK    UINT8_C(0xFC)
#define PWR_MODE_NORMAL         UINT8_C(0x00)
#define OPR_MODE_MASK           UINT8_C(0xF0)
#define OPR_MODE_CONF           UINT8_C(0x00)
#define OPR_MODE_AMG            UINT8_C(0x07)
#define OPR_MODE_IMU            UINT8_C(0x08)
#define OPR_MODE_M4G            UINT8_C(0x0A)
#define OPR_MODE_NDOF_FMC_OFF   UINT8_C(0x0B)
#define OPR_MODE_NDOF           UINT8_C(0x0C)
#define UNIT_SEL_ACC_M_S2       UINT8_C(0x00)
#define UNIT_SEL_ACC_MG         UINT8_C(0x01)
#define UNIT_SEL_ANG_RATE_DEG   UINT8_C(0x00)
#define UNIT_SEL_ANG_RATE_RAD   UINT8_C(0x02)
#define UNIT_SEL_EU_ANG_DEG     UINT8_C(0x00)
#define UNIT_SEL_EU_ANG_RAD     UINT8_C(0x04)
#define EULE                    true   
#define QUAT                    false
#define NONE                    false


class BNO055 
{
private:
    uint8_t SLAVE_ADDR; //BNO055 is using the I2C address 0x28
public:
    BNO055(uint8_t addr);
    ~BNO055();
    void initialize_I2C(uint8_t opr);
    void initialize_BNO055(uint8_t *data, uint8_t opr);
    void check_power_mode(uint8_t *data);
    void initialize_operating_mode(uint8_t opr_mode); 
    void select_unit(uint8_t unit);
    void get_sensor_data(uint8_t opr, bool format);
    void data_mode_amg();
    void data_mode_fusion_absolute_euler();
    void data_mode_fusion_absolute_quaternion();
    void data_mode_fusion_relative_euler();
    void data_mode_fusion_relative_quaternion();
    void get_acc_data(uint8_t *data, JsonArray& array);
    void get_mag_data(uint8_t *data, JsonArray& array);
    void get_gyro_data(uint8_t *data, JsonArray& array);
    void get_euler_hrp(uint8_t *data, JsonArray& array);
    void get_quant(uint8_t *data, JsonArray &array);
    size_t publish_sensor_data(JsonDocument& doc);
    void read_reg(uint8_t *data, uint8_t addr, uint8_t len);
    void write_reg(uint8_t *data, uint8_t addr, uint8_t len);
};

#endif