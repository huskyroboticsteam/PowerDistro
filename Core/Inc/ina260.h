#ifndef INA260_H
#define INA260_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "main.h"

// register addresses
#define configurationRegister   0x00
#define currentRegister         0x01
#define voltageRegister         0x02
#define powerRegister           0x03
#define maskEnableRegister      0x06
#define alertLimitRegister      0x07
#define manufacturerIDRegister  0xFE

#define manufacturerIDValue 0x5449

// LSBs: (multiply this by raw count to get voltage reading)
//  - current register, 1.25 mA
//  - voltage register, 1.25 mV
//  - power register, 10 mW

#define currentLSB_mA       1.25f
#define voltageLSB_mV       1.25f
#define powerLSB_mW         10.0f

// masks for mask enable register
#define overcurrentEnable       (1u << 15)
// #define undercurrentEnable      (1u << 14)
#define overvoltageEnable       (1u << 13)
// #define undervoltageEnable      (1u << 12)
// #define powerOverLimitEnable    (1u << 11)
#define alertFunctionFlag       (1u << 4)
#define latchEnable             (1u << 0)

// contains an instance of the ina260 sensor, with the handle type (for HAL) and
// an address (7-bit)
typedef struct 
{
    I2C_HandleTypeDef *hi2c;
    uint8_t address;
} ina260;

// holds all possible error types
typedef enum
{
    success,
    i2c_error,
    invalid_id
} ina260_status;

// initialize instance of ina260 that holds necessary details
ina260_status ina260_init(ina260 *sensor, I2C_HandleTypeDef *hi2c, uint8_t address);

// read current in amps
ina260_status ina260_readCurrent(ina260 *sensor, float *current);

// read voltage in volts
ina260_status ina260_readVoltage(ina260 *sensor, float *voltage);

// read power value in watts
ina260_status ina260_readPower(ina260 *sensor, float *power);

#endif