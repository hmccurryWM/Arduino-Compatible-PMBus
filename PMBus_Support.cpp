#include "PMBus_ABIO.h"
#include "PMBus_commands.h"

// Hasve master list of command codes and lengths for pmbus here, and for each subclass have another list of command codes compatible with the device
// Call the getcommandlength function, which will get the applicable command length, but that will only be done from the device level

uint8_t getCommandLength(uint8_t cmd_code)
{
    switch (cmd_code)
    {
        // Basic command codes
        case CMD_CODE_OPERATION:               return CMD_LENGTH_OPERATION;
        case CMD_CODE_ON_OFF_CONFIG:           return CMD_LENGTH_ON_OFF_CONFIG;
        case CMD_CODE_CAPABILITY:              return CMD_LENGTH_CAPABILITY;
        case CMD_CODE_VOUT_MODE:               return CMD_LENGTH_VOUT_MODE;
        case CMD_CODE_VOUT_COMMAND:            return CMD_LENGTH_VOUT_COMMAND;
        case CMD_CODE_VOUT_TRIM:               return CMD_LENGTH_VOUT_TRIM;
        case CMD_CODE_IOUT_OC_FAULT_LIMIT:     return CMD_LENGTH_IOUT_OC_FAULT_LIMIT;
        case CMD_CODE_IOUT_OC_FAULT_RESPONSE:  return CMD_LENGTH_IOUT_OC_FAULT_RESPONSE;
        case CMD_CODE_STATUS_WORD:             return CMD_LENGTH_STATUS_WORD;
        case CMD_CODE_STATUS_VOUT:             return CMD_LENGTH_STATUS_VOUT;
        case CMD_CODE_STATUS_IOUT:             return CMD_LENGTH_STATUS_IOUT;
        case CMD_CODE_STATUS_INPUT:            return CMD_LENGTH_STATUS_INPUT;
        case CMD_CODE_STATUS_TEMPERATURE:      return CMD_LENGTH_STATUS_TEMPERATURE;
        case CMD_CODE_STATUS_CML:              return CMD_LENGTH_STATUS_CML;
        case CMD_CODE_STATUS_MFR_SPECIFIC:     return CMD_LENGTH_STATUS_MFR_SPECIFIC;
        case CMD_CODE_STATUS_FANS_1_2:         return CMD_LENGTH_STATUS_FANS_1_2;
        case CMD_CODE_READ_VIN:                return CMD_LENGTH_READ_VIN;
        case CMD_CODE_READ_VOUT:               return CMD_LENGTH_READ_VOUT;
        case CMD_CODE_READ_IOUT:               return CMD_LENGTH_READ_IOUT;
        case CMD_CODE_READ_FAN_SPEED_1:        return CMD_LENGTH_READ_FAN_SPEED_1;
        case CMD_CODE_READ_FAN_SPEED_2:        return CMD_LENGTH_READ_FAN_SPEED_2;
        case CMD_CODE_PMBUS_REVISION:          return CMD_LENGTH_PMBUS_REVISION;
        case CMD_CODE_MFR_ID:                  return CMD_LENGTH_MFR_ID;
        case CMD_CODE_MFR_MODEL:               return CMD_LENGTH_MFR_MODEL;
        case CMD_CODE_MFR_REVISION:            return CMD_LENGTH_MFR_REVISION;
        case CMD_CODE_MFR_LOCATION:            return CMD_LENGTH_MFR_LOCATION;
        case CMD_CODE_MFR_DATE:                return CMD_LENGTH_MFR_DATE;
        case CMD_CODE_MFR_SERIAL:              return CMD_LENGTH_MFR_SERIAL;

        // Charging curve config command codes
        case CMD_CODE_CURVE_CC:                return CMD_LENGTH_CURVE_CC;
        case CMD_CODE_CURVE_CV:                return CMD_LENGTH_CURVE_CV;
        case CMD_CODE_CURVE_FV:                return CMD_LENGTH_CURVE_FV;
        case CMD_CODE_CURVE_TC:                return CMD_LENGTH_CURVE_TC;
        case CMD_CODE_CURVE_CONFIG:            return CMD_LENGTH_CURVE_CONFIG;
        case CMD_CODE_CURVE_CC_TIMEOUT:        return CMD_LENGTH_CURVE_CC_TIMEOUT;
        case CMD_CODE_CURVE_CV_TIMEOUT:        return CMD_LENGTH_CURVE_CV_TIMEOUT;
        case CMD_CODE_CURVE_FLOAT_TIMEOUT:     return CMD_LENGTH_CURVE_FLOAT_TIMEOUT;
        case CMD_CODE_CHG_STATUS:              return CMD_LENGTH_CHG_STATUS;

        // System config command codes
        case CMD_CODE_SYSTEM_CONFIG:           return CMD_LENGTH_SYSTEM_CONFIG;
        case CMD_CODE_SYSTEM_STATUS:           return CMD_LENGTH_SYSTEM_STATUS;

        default: return 0; // Unknown or unsupported command
    }
}