#include "ina260.h"

/**
 * Reads a 16 bit register using the HAL_I2C_Mem_Read function.
 *
 * @param sensor the ina260 to be read from
 * @param register the register to be read from
 * @param value a pointer to the empty uint16_t variable to where the value read
 *              will be stored
 *
 * @return success if the operation completes successfully, i2c_error if not
 */
static ina260_status readRegister16Bit(ina260 *sensor, uint8_t reg, uint16_t *value)
{
    uint8_t buffer[2] = {0, 0};
    uint32_t timeout = 5; // can be changed, this is just a placeholder

    if (HAL_I2C_Mem_Read(sensor -> hi2c, (sensor -> address) << 1, reg, 
                            I2C_MEMADD_SIZE_8BIT, buffer, 2, timeout) != HAL_OK)
        {
            return i2c_error;
        }

    *value = ((uint16_t) buffer[0] << 8) | buffer[1];

    return success;
}

/**
 * Writes to a 16 bit register using the HAL_I2C_Mem_Write function.
 *
 * @param sensor the ina260 to be written to
 * @param register the register to be written to
 * @param value a pointer to the empty uint16_t variable to where the value read
 *              will be stored
 *
 * @return success if the operation completes successfully, i2c_error if not
 */
static ina260_status writeRegister16Bit(ina260 *sensor, uint8_t reg, uint16_t value)
{
    uint8_t buffer[2] = {0, 0};
    uint32_t timeout = 5; // can be changed, this is just a placeholder

    buffer[0] = (uint8_t)((value >> 8) & 0xFF);
    buffer[1] = (uint8_t)(value & 0xFF);

    if (HAL_I2C_Mem_Write(sensor -> hi2c, (sensor -> address) << 1, reg, 
                            I2C_MEMADD_SIZE_8BIT, buffer, 2, timeout) != HAL_OK)
        {
            return i2c_error;
        }

    return success;
}

/**
 * Initializes an ina260 instance.
 *
 * @param sensor a pointer to the instance of ina 260 to be initialized
 * @param hi2c the peripheral being controlled by this instance
 * @param address the 7-bit I2C address of the chip
 *
 * @return success if the operation completes successfully, i2c_error if not, 
 *         invalid_id if it completes but the manufacturer's id is not 0x5449
 */
ina260_status ina260_init(ina260 *sensor, I2C_HandleTypeDef *hi2c, uint8_t address)
{
    sensor -> hi2c = hi2c;
    sensor -> address = address;

    uint16_t mfg_id = 0;
    if (readRegister16Bit(sensor, manufacturerIDRegister, &mfg_id) != success)
    {
        return i2c_error;
    }
    if (mfg_id != manufacturerIDValue)
    {
        return invalid_id;
    }

    return success;
}

/**
 * Reads the current off an instance of ina260
 *
 * @param ina260 the sensor to be read from
 * @param current a pointer to the empty float variable for current to where 
 *                the value read will be stored
 *
 * @return success if the operation completes successfully, i2c_error if not
 */
ina260_status ina260_readCurrent(ina260 *sensor, float *current)
{
    uint16_t rawCurrent = 0;
    if (readRegister16Bit(sensor, currentRegister, &rawCurrent) != success)
    {
        return i2c_error;
    }

    *current = ((int16_t) rawCurrent * currentLSB_mA) / 1000.0f; // convert to Amps

    return success;
}

/**
 * Reads the bus voltage off an instance of ina260
 *
 * @param ina260 the sensor to be read from
 * @param voltage a pointer to the empty float variable for voltage to where 
 *                the value read will be stored
 *
 * @return success if the operation completes successfully, i2c_error if not
 */
ina260_status ina260_readVoltage(ina260 *sensor, float *voltage)
{
    uint16_t rawVoltage = 0;
    if (readRegister16Bit(sensor, voltageRegister, &rawVoltage) != success)
    {
        return i2c_error;
    }

    *voltage = (rawVoltage * voltageLSB_mV) / 1000.0f; // convert to Volts

    return success;
}

/**
 * Reads the power off an instance of ina260
 *
 * @param ina260 the sensor to be read from
 * @param power a pointer to the empty float variable for power to where 
 *                the value read will be stored
 *
 * @return success if the operation completes successfully, i2c_error if not
 */
ina260_status ina260_readPower(ina260 *sensor, float *power)
{
    uint16_t rawPower = 0;
    if (readRegister16Bit(sensor, powerRegister, &rawPower) != success)
    {
        return i2c_error;
    }

    *power = (rawPower * powerLSB_mW) / 1000.0f; // convert to Watts

    return success;
}