#ifndef INA260_H
#define INA260_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <main.h>

#define configurationRegister   0x00
#define currentRegister         0x01
#define voltageRegister         0x02
#define powerRegister           0x03
#define maskEnableRegister      0x06
#define alertLimitRegister      0x07
#define manufacturerID          0xFE

// LSBs: (multiply this by raw count to get voltage reading)
//  - current register, 1.25 mA
//  - voltage register, 1.25 mV
//  - power register, 10 mW




#endif