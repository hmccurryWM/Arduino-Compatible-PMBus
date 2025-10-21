// Structures n Enums n Stuff
#include <cstdint>
#include <stdint.h>

/**
 * @brief The maximum number of bytes we could possibly expect to receive from the charger
 */
 #define MAX_RECEIVE_BYTES 12

 #define N_EXPONENT_MASK 0xF800 // Bitmask for pulling out the first 5 bytes of the payload (the N exponent value)
 #define MANTISSA_MASK 0x07FF   // Bitmask for pulling out the last 11 bytes of the payload (the Mantissa)
 #define N_EXPONENT_LENGTH 5
 #define MANTISSA_LENGTH 11
 #define N_EXPONENT_SHIFT MANTISSA_LENGTH

// System Config High Byte bits
#define SYSTEM_CONFIG_EEP_OFF 2
#define SYSTEM_CONFIG_EEP_CONFIG 0

// System Config Low Byte bits
#define SYSTEM_CONFIG_OPERATION_INIT 1
#define SYSTEM_CONFIG_PM_CTRL 0

// System Config High Byte bitmask
#define SYSTEM_CONFIG_EEP_OFF_MASK 1
#define SYSTEM_CONFIG_EEP_CONFIG_MASK 3

// System Config Low Byte bitmask
#define SYSTEM_CONFIG_OPERATION_INIT_MASK 3
#define SYSTEM_CONFIG_PM_CTRL_MASK 1

// STATUS_BYTE (bits 0–7 of STATUS_WORD)
#define STATUS_WORD_BIT_7_BUSY                7
#define STATUS_WORD_BIT_6_OFF                 6
#define STATUS_WORD_BIT_5_VOUT_OV             5
#define STATUS_WORD_BIT_4_IOUT_OC             4
#define STATUS_WORD_BIT_3_VIN_UV              3
#define STATUS_WORD_BIT_2_TEMPERATURE         2
#define STATUS_WORD_BIT_1_CML                 1
#define STATUS_WORD_BIT_0_NONE_OF_THE_ABOVE   0

// Upper byte of STATUS_WORD (bits 8–15)
#define STATUS_WORD_BIT_15_VOUT               15
#define STATUS_WORD_BIT_14_IOUT               14
#define STATUS_WORD_BIT_13_INPUT              13
#define STATUS_WORD_BIT_12_MFR_SPECIFIC       12
#define STATUS_WORD_BIT_11_POWER_GOOD_N       11
#define STATUS_WORD_BIT_10_FANS               10
#define STATUS_WORD_BIT_9_OTHER               9
#define STATUS_WORD_BIT_8_UNKNOWN             8

#define STATUS_WORD_BIT_MASK 1

// Operation Byte bits
#define OPERATION_ON_OFF 7
#define OPERATION_OFF_TYPE 6 // Only active when 7 is low (PSU is turned off). 0 is immediate, 1 is soft
#define OPERATION_MARGIN 4 // 01 watches for under- conditions and 10 watches for over-
#define OPERATION_FAULT_ACTION 2 // 01 ignores faults, 10 acts on fault conditions

// Operation Byte bit mask
#define OPERATION_ON_OFF_MASK 1
#define OPERATION_OFF_TYPE_MASK 1
#define OPERATION_MARGIN_MASK 3
#define OPERATION_FAULT_ACTION_MASK 3

// Operation Field Values
#define OPERATION_ONOFF_ON 1
#define OPERATION_ONOFF_OFF 0
#define OPERATION_OFFTYPE_SOFT 1
#define OPERATION_OFFTYPE_HARD 0
#define OPERATION_MARGINCALL_OFF 0
#define OPERATION_MARGINCALL_ON 1
#define OPERATION_MARGINCALL_UPPER 2
#define OPERATION_FAULTACTION_IGNORE 1
#define OPERATION_FAULTACTION_ACTED 2

// On/Off Config Byte bits
#define ON_OFF_CONFIG_DEFAULT_POWER 4
#define ON_OFF_CONFIG_SERIAL 3
#define ON_OFF_CONFIG_CONTROL_PIN 2
#define ON_OFF_CONFIG_CONTROL_POLARITY 1
#define ON_OFF_CONFIG_CONTROL_ACTION 0

// On/Off Config Byte bit mask
#define ON_OFF_CONFIG_MASK 1

// Charger Definition Macros
// Curve Config
// Hi Byte
#define FVTIMEOUT 2
#define CVTIMEOUT 1
#define CCTIMEOUT 0
// Lo Byte
#define CURVE_ENABLE 7
#define CHARGE_STAGES 6
#define TEMP_COMPENSATION 2
#define CURVE_SELECTION 0
// Bit Field Size
#define CURVE_TIMEOUT_SIZE 1
#define CURVE_ENABLE_SIZE 1
#define CHARGE_STAGES_SIZE 1
#define TEMP_COMP_SIZE 2
#define CURVE_SEL_SIZE 2

// ON_OFF_CONFIG register bits
typedef struct
{
    uint8_t power_default;
    uint8_t serial_command;
    uint8_t analog_command;
    uint8_t control_polarity;
    uint8_t control_action;
} OnOffConfigByte;

// STATUS_VOUT register bits
typedef struct {
    uint8_t vout_ov_fault;
    uint8_t vout_uv_fault;
    uint8_t vout_ov_warning;
    uint8_t vout_uv_warning;
    uint8_t vout_out_of_range;
    uint8_t vout_not_programmed;
    uint8_t reserved1;
    uint8_t reserved2;
} PMBusStatusVOUT;

// STATUS_IOUT register bits
typedef struct {
    uint8_t iout_oc_fault;
    uint8_t iout_oc_warning;
    uint8_t iout_uc_fault;
    uint8_t reserved1;
    uint8_t power_limit_exceeded;
    uint8_t current_share_fault;
    uint8_t reserved2;
    uint8_t reserved3;
} PMBusStatusIOUT;

// STATUS_INPUT register bits
typedef struct {
    uint8_t vin_uv_fault;
    uint8_t vin_uv_warning;
    uint8_t vin_ov_warning;
    uint8_t unit_off_for_insufficient_input_voltage;
    uint8_t brownout;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
} PMBusStatusINPUT;

// STATUS_TEMPERATURE register bits
typedef struct {
    uint8_t ot_fault;
    uint8_t ot_warning;
    uint8_t ut_warning;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t reserved4;
    uint8_t reserved5;
} PMBusStatusTEMPERATURE;

// STATUS_CML register bits
typedef struct {
    uint8_t invalid_command;
    uint8_t invalid_data;
    uint8_t pec_fault;
    uint8_t memory_fault;
    uint8_t processor_fault;
    uint8_t communication_fault;
    uint8_t reserved1;
    uint8_t reserved2;
} PMBusStatusCML;

// STATUS_MFR_SPECIFIC register bits (example - this is implementation specific)
typedef struct {
    uint8_t mfr_alert_1;
    uint8_t mfr_alert_2;
    uint8_t mfr_alert_3;
    uint8_t reserved1;
    uint8_t reserved2;
    uint8_t reserved3;
    uint8_t reserved4;
    uint8_t reserved5;
} PMBusStatusMFR_SPECIFIC;

struct PMBusStatus
{
    // STATUS_BYTE bits (lower 8 bits of STATUS_WORD)
    uint8_t busy;
    uint8_t off;
    uint8_t vout_ov;
    uint8_t iout_oc;
    uint8_t vin_uv;
    uint8_t temperature;
    uint8_t cml;
    uint8_t none_of_the_above;

    // Upper 8 bits of STATUS_WORD
    uint8_t vout;
    uint8_t iout;
    uint8_t input;
    uint8_t mfr_specific;
    uint8_t power_good_negated;
    uint8_t fans;
    uint8_t other;
    uint8_t unknown;

    // Extended status registers
    PMBusStatusVOUT status_vout;
    PMBusStatusIOUT status_iout;
    PMBusStatusINPUT status_input;
    PMBusStatusTEMPERATURE status_temperature;
    PMBusStatusCML status_cml;
    PMBusStatusMFR_SPECIFIC status_mfr_specific;
};

struct readings
{
    uint16_t v_in;
    float v_out;
    float i_out;
    //uint16_t fan_speed_1;
    //uint16_t fan_speed_2;
};

struct mfr_data
{
    char id[12];
    char model[12];
    char revision[24];
    char location[3];
    char date[6];
    char serial[12];
};

struct buffer_data
{
    uint8_t buffer[MAX_RECEIVE_BYTES];
    uint8_t bufferLength;
};

struct capability
{
    bool CRC;
    uint8_t BUS;
    bool ALT;
};

struct status
{
    uint8_t onOff;
    uint8_t onOffControl;
};

struct operationByte
{
    uint8_t onOff;
    uint8_t offType;
    uint8_t marginCall;
    uint8_t faultAction;
};

enum SystemConfig
{
    SC_NONE,
    PM_CTRL,
    OPERATION_INIT,
    EEP_CONFIG,
    EEP_OFF
};

enum OperationFields
{
    OF_NONE,
    ON_OFF,
    OFF_TYPE,
    MARGIN_CALL,
    FAULT_ACTION
};

#define USECHARGER
#ifdef USECHARGER
struct curve_config
{
    uint8_t charge_curve_type;
    uint8_t temp_compensation;
    uint8_t num_charge_stages;
    bool cc_timeout_indication_enabled;
    bool cv_timeout_indication_enabled;
    bool float_stage_timeout_indication_enabled;
    bool charge_curve_function_enable;
};

struct charge_status
{
    bool fully_charged;
    bool in_cc_mode;
    bool in_cv_mode;
    bool in_float_mode;
    // From datasheet:
    // When EEPROM Charge Parameter Error occurs, the charger stops
    // charging the battery and the LED indicator turns red. The
    // charger needs to re-power on to re-start charging the battery.
    bool EEPROM_error;
    // From datasheet: When Temperature Compensation Short occurs, the
    // charger output will shut down and the LED indicator will turn red.
    // The charger will automatically restart after the Temperature
    // Compensation Short condition is removed.
    bool temp_compensation_short_circuit;
    // From datasheet: When there is no battery detected, the charger
    // stops charging the battery and the LED indicator turns red. The
    // charger needs to re-power on to re-start charging the battery
    bool battery_detected;
    // From datasheet: When timeout arises in the Constant Current stage,
    // the charger stops charging the battery and the LED indicator turns
    // red. The charger needs to re-power on to re-start charging the
    // battery
    bool timeout_flag_cc_mode;
    // From datasheet: When timeout arises in the Constant Voltage stage,
    // the charger stops charging the battery and the LED indicator turns
    // red. The charger needs to re-power on to re-start charging the
    // battery
    bool timeout_flag_cv_mode;
    // From datasheet: When timeout arises in the Float stage, the
    // charger stops charging the battery and the LED indicator turns
    // green. This charging flow is finished; the charger needs to
    // re-power on to start charging a different battery
    bool timeout_flag_float_mode;
};

struct curve_parameters
{
    uint16_t cc;
    float cv;
    float floating_voltage;
    uint16_t taper_current;
    curve_config config;
    uint16_t cc_timeout;
    uint16_t cv_timeout;
    uint16_t float_timeout;
    charge_status status;
};
#endif