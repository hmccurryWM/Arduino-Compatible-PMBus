//#include <cstdint>
//#include <filesystem>
//#include <stdio.h>
//#include <stddef.h>
//#include <stdint.h>
//#include <stdlib.h>
#include "PMBus_ABIO.h"
//#include "PMBus_Struct.h"
#include "HEP-1000-commands.h"
#include <cstdint>

#ifndef HEP_1000_X
#define HEP_1000_X

// Linear data N values
#define CMD_N_VALUE_VOUT_MODE -7
#define CMD_N_VALUE_VOUT_COMMAND -7
#define CMD_N_VALUE_VOUT_TRIM -7
#define CMD_N_VALUE_IOUT_OC_FAULT_LIMIT -6
#define CMD_N_VALUE_READ_VIN -1
#define CMD_N_VALUE_READ_VOUT -7
#define CMD_N_VALUE_READ_IOUT -6
#define CMD_N_VALUE_READ_FAN_SPEED_1 5
#define CMD_N_VALUE_READ_FAN_SPEED_2 5
#define CMD_N_VALUE_CURVE_CC -6
#define CMD_N_VALUE_CURVE_CV -7
#define CMD_N_VALUE_CURVE_FV -7
#define CMD_N_VALUE_CURVE_TC -6
#define CMD_N_VALUE_CURVE_CC_TIMEOUT 0
#define CMD_N_VALUE_CURVE_CV_TIMEOUT 0
#define CMD_N_VALUE_CURVE_FLOAT_TIMEOUT 0

#define PSU_ADD 0x40

class HEP_1000 : public PMBus_ABIO
{
protected:
    float vtrim;
    float vout_set;
    uint8_t rating;

public:
    HEP_1000(uint8_t rating, TwoWire *wire = &Wire, uint32_t clkDuring = 100000UL, uint32_t clkAfter = 100000UL);

    ~HEP_1000();

    // Operations Stuff
    bool setOperation(const operationByte* ops);
    bool getOperation(operationByte* ops); 
    bool setOperation(const uint8_t* ops);
    bool getOperation(uint8_t* ops); 
    bool getOnOffConfig(const OnOffConfigByte *oocb);
    bool getOnOffConfig(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?    
    //bool getCapability(const capability* caps);
    bool getVoutMode(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getVoutCommand(const float *vout);
    bool setVoutTrim(const float *trim, float lowerBound = TRIM_LOWER_BOUND, float upperBound = TRIM_UPPER_BOUND);
    bool getVoutTrim(float * const trim);
    bool setIoutOCFaultLimit(const float *ilim);
    bool getIoutOCFaultLimit(float * const ilim);
    bool getIoutOCFaultResponse(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?

    // Status Stuff
    bool getStatusWord(const PMBusStatus* stats);
    bool getStatusWord(const uint16_t *reg);
    bool getStatusVout(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getStatusIout(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getStatusInput(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getStatusTemperature(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getStatusCML(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?
    bool getStatusMFRSpecific(const uint8_t *reg); // This ought to be moved to a structure as well, or given an option?

    // Values
    bool getVin(uint8_t * const vin);
    bool getVout(float * const vout);
    bool getIout(float * const iout);
    bool getReadTemp1(float * const temp);

    // Metadata Stuff
    bool getPMBusRevision(const uint8_t *rev);
    bool getMfrID(const char* id);
    bool getMfrModel(const char* model);
    bool getMfrRevision(const char* revision);
    bool getMfrLocation(const char* location);
    bool getMfrDate(const char* date);
    bool getMfrSerial(const char* serial);
    bool setMfrLocation(const char* location);
    bool setMfrDate(const char* date);
    bool setMfrSerial(const char* serial);

    // Charger Stuff
    bool setCurveCC(const float *cccurve);
    bool setCurveCV(const float *cvcurve);
    bool setCurveFV(const float *fvcurve);
    bool setCurveTC(const float *tccurve);
    bool getCurveCC(const float *cccurve);
    bool getCurveCV(const float *cvcurve);
    bool getCurveFV(const float *fvcurve);
    bool getCurveTC(const float *tccurve);
    bool setCurveConfig(curve_config *ccfg);
    bool getCurveConfig();
    bool setCurveCCTimeout(const uint16_t *cctimeout);
    bool setCurveCVTimeout(const uint16_t *cvtimeout);
    bool setCurveFloatTimeout(const uint16_t *floattimeout);
    bool getCurveCCTimeout(const uint16_t *cctimeout);
    bool getCurveCVTimeout(const uint16_t *cvtimeout);
    bool getCurveFloatTimeout(const uint16_t *floattimeout);

    // Misc Stuff
    bool getChargeStatus(const uint16_t *chargestatus);
    bool setSystemConfig(const uint16_t *systemconfig);
    bool getSystemConfig(const uint16_t *systemconfig);
    bool getSystemStatus(const uint16_t *systemstatus);   

    // Custom Stuff
    bool getMfrData(mfr_data* data);
    bool setVout(const float* vout);
    bool getSetVoutCommand(const float* vout);

#define USECHARGER
#ifdef USECHARGER
    /**
     * @brief Query charger for curve parameter bytes, populate a "curve_parameters" struct
     * @return true on successful read, false otherwise
     */
    //bool getCurveParams(curve_parameters *params);
    bool EnableCharger(bool enable);
#endif

private:
    uint8_t voltageRating;
};

#endif