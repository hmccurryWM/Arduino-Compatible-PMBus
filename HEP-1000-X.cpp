#include <Arduino.h>
#include <HardwareSerial.h>
#include <cstdint>
//#include <cmath>
//#include <cstdint>
//#include "PMBus_ABIO.h"
#include "HEP-1000-commands.h"
#include "HEP-1000-X.h"

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
    #define dprintf(...) Serial.printf(__VA_ARGS__)
#else
    #define dprintf(...)
#endif

// Constructor
HEP_1000::HEP_1000(uint8_t rating, TwoWire *wire, uint32_t clkDuring, uint32_t clkAfter)
    : PMBus_ABIO(wire, clkDuring, clkAfter), rating(rating) 
    { 
        
    }

// Destructor
HEP_1000::~HEP_1000() 
{

}
//
// Will need to properly propagate consts and move away from temp assignments
//

// Operations Stuff
bool HEP_1000::setOperation(const operationByte* ops)
{
    if (!ops) return false;

    uint8_t txBuilder[1] = { 0 };

    txBuilder[0] = 0x0000000F & ((ops->faultAction << OPERATION_FAULT_ACTION) | (ops->marginCall << OPERATION_MARGIN) 
                        | (ops->offType << OPERATION_OFF_TYPE) | (ops->onOff << OPERATION_ON_OFF));

    dprintf("Operations buff is 0x%02x\n", txBuilder[0]);

    if(!writeTwoBytes(CMD_CODE_OPERATION, txBuilder , 1))
    {
        dprintf("Error writing value");
        return false;
    }

    return true;
}

bool HEP_1000::setOperation(const uint8_t* ops)
{
    if (!ops) return false;

    uint8_t txBuilder[1] = {*ops};

    dprintf("Operations buff is 0x%02x\n", txBuilder[0]);

    if(!writeTwoBytes(CMD_CODE_OPERATION, txBuilder , 1))
    {
        dprintf("Error writing value");
        return false;
    }

    return true;
}

bool HEP_1000::setVoutTrim(const float * trim, float lowerBound, float upperBound)
{
    if (!trim) return false;

    float temptrim = *trim;
    
    dprintf("Trim value is %.3f\n", temptrim);

    if (!writeVoutTrim(temptrim, CMD_N_VALUE_VOUT_TRIM, lowerBound, upperBound))
    {
        dprintf("ERROR Writing vout trim");

        return false;
    }

    return true;
}

bool HEP_1000::setIoutOCFaultLimit(const float * ilim)
{
    if (!ilim) return false;
    
    float ioclim = *ilim;

    Serial.printf("Setting the current limit to %.4f A\n", ioclim);

    if (!setOCFaultLimit(ioclim, CMD_N_VALUE_IOUT_OC_FAULT_LIMIT))
    {
        dprintf("ERROR WRITING LIMIT");
        return false;
    }

  dprintf("Success");

  return true;
}
bool HEP_1000::getOperation(operationByte* ops)
{
    if (!ops) return false;

    if (!runOperation(ops))
    {
        dprintf("Error getting operations\n");
        return false;
    }

    return true;
}

bool HEP_1000::getOperation(uint8_t* ops)
{
    if (ops) *(uint16_t*)ops = 0;
    return false;
}

bool HEP_1000::getOnOffConfig(const OnOffConfigByte *oocb)
{
    if (!oocb) return false;

    OnOffConfigByte config = *oocb;
    return false;
}

bool HEP_1000::getOnOffConfig(const uint8_t *reg)
{
    if (!reg) return false;
    

    return false;
}

/*bool HEP_1000::getCapability(const capability* caps)
{
    (void)caps;
    return false;
}*/

bool HEP_1000::getVoutMode(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getVoutCommand(const float *vout)
{
    if (vout) *(float*)vout = 0.0f;
    return false;
}

bool HEP_1000::getVoutTrim(float * const trim)
{
    int16_t rx = 0;
    float trimbuffer = 0.0f;

    if (!readWithCommand(CMD_CODE_VOUT_TRIM, CMD_LENGTH_VOUT_TRIM))
    {
        Serial.println("Error reading");
        return false;
    }

    rx = (rxbuffer->buffer[1] << 8) | (rxbuffer->buffer[0]);

    Serial.printf("RX[1]: 0x%02x  RX[0]: 0x%02x\n", rxbuffer->buffer[1], rxbuffer->buffer[0]);
    Serial.printf("RX: %d\n", rx);

    trimbuffer = rx / (float)(1 << abs(CMD_N_VALUE_VOUT_TRIM));

    Serial.printf("Trim is set to %0.4fV\n", trimbuffer);

    *trim = trimbuffer;

    return true;
}

bool HEP_1000::getIoutOCFaultLimit(float * const ilim)
{
    int16_t rx = 0;
    float ilimbuffer = 0.0f;

    if (!readWithCommand(CMD_CODE_IOUT_OC_FAULT_LIMIT, CMD_LENGTH_IOUT_OC_FAULT_LIMIT))
    {
        Serial.println("Error reading");
        return false;
    }

    ilimbuffer = parseOutputCurrent();

    Serial.printf("Io fault limit is set to %0.4fA\n", ilimbuffer);

    *ilim = ilimbuffer;

    return true;
}

bool HEP_1000::getIoutOCFaultResponse(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}


// Status Stuff
bool HEP_1000::getStatusWord(const PMBusStatus* stats)
{
    (void)stats;
    return false;
}

bool HEP_1000::getStatusWord(const uint16_t *reg)
{
    if (reg) *(uint16_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusVout(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusIout(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusInput(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusTemperature(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusCML(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}

bool HEP_1000::getStatusMFRSpecific(const uint8_t *reg)
{
    if (reg) *(uint8_t*)reg = 0;
    return false;
}


// Values
bool HEP_1000::getVin(uint8_t * const vin)
{
    int16_t rx = 0;
    uint16_t vinbuffer = 0;

    if (!readWithCommand(CMD_CODE_READ_VIN, CMD_LENGTH_READ_VIN))
    {
        Serial.println("Error reading");
        return false;
    }

    vinbuffer = parseLinearData();

    Serial.printf("Vin is %dV\n", vinbuffer);

    *vin = vinbuffer;

    return true;
}

//These read 16 bit linear messages should be generalized into a function this calls
bool HEP_1000::getVout(float * const vout)
{
    int16_t rx = 0;
    float voutbuffer = 0.0f;

    if (!readWithCommand(CMD_CODE_READ_VOUT, CMD_LENGTH_READ_VOUT))
    {
        Serial.println("Error reading");
        return false;
    }

    rx = (rxbuffer->buffer[1] << 8) | (rxbuffer->buffer[0]);

    Serial.printf("RX[1]: 0x%02x  RX[0]: 0x%02x\n", rxbuffer->buffer[1], rxbuffer->buffer[0]);
    Serial.printf("RX: %d\n", rx);

    voutbuffer = rx / (float)(1 << abs(CMD_N_VALUE_READ_VOUT));

    Serial.printf("Vout is currently %0.4fV\n", voutbuffer);

    *vout = voutbuffer;

    return true;
}

bool HEP_1000::getIout(float * const iout)
{
    int16_t rx = 0;
    float ioutbuffer = 0.0f;

    if (!readWithCommand(CMD_CODE_READ_IOUT, CMD_LENGTH_READ_IOUT))
    {
        Serial.println("Error reading");
        return false;
    }

    ioutbuffer = readOutputCurrent();

    Serial.printf("Iout is currently %0.4fA\n", ioutbuffer);

    *iout = ioutbuffer;

    return true;
}

bool HEP_1000::getReadTemp1(float * const temp)
{
    if (temp) *(float*)temp = 0.0f;
    return false;
}


// Metadata Stuff
bool HEP_1000::setMfrLocation(const char* location)
{
    (void)location;
    return false;
}

bool HEP_1000::setMfrDate(const char* date)
{
    (void)date;
    return false;
}

bool HEP_1000::setMfrSerial(const char* serial)
{
    (void)serial;
    return false;
}
bool HEP_1000::getPMBusRevision(const uint8_t *rev)
{
    if (rev) *(uint8_t*)rev = 0;
    return false;
}

bool HEP_1000::getMfrID(const char* id)
{
    (void)id;
    return false;
}

bool HEP_1000::getMfrModel(const char* model)
{
    (void)model;
    return false;
}

bool HEP_1000::getMfrRevision(const char* revision)
{
    (void)revision;
    return false;
}

bool HEP_1000::getMfrLocation(const char* location)
{
    (void)location;
    return false;
}

bool HEP_1000::getMfrDate(const char* date)
{
    (void)date;
    return false;
}

bool HEP_1000::getMfrSerial(const char* serial)
{
    (void)serial;
    return false;
}


// Charger Stuff
bool HEP_1000::setCurveCC(const float *cccurve)
{
    if (cccurve) *(float*)cccurve = 0.0f;
    return false;
}

bool HEP_1000::setCurveCV(const float *cvcurve)
{
    if (cvcurve) *(float*)cvcurve = 0.0f;
    return false;
}

bool HEP_1000::setCurveFV(const float *fvcurve)
{
    if (fvcurve) *(float*)fvcurve = 0.0f;
    return false;
}

bool HEP_1000::setCurveTC(const float *tccurve)
{
    if (tccurve) *(float*)tccurve = 0.0f;
    return false;
}

bool HEP_1000::setCurveConfig(const uint16_t *reg)
{
    if (reg) *(uint16_t*)reg = 0;
    return false;
}

bool HEP_1000::setCurveCCTimeout(const uint16_t *cctimeout)
{
    if (cctimeout) *(uint16_t*)cctimeout = 0;
    return false;
}

bool HEP_1000::setCurveCVTimeout(const uint16_t *cvtimeout)
{
    if (cvtimeout) *(uint16_t*)cvtimeout = 0;
    return false;
}

bool HEP_1000::setCurveFloatTimeout(const uint16_t *floattimeout)
{
    if (floattimeout) *(uint16_t*)floattimeout = 0;
    return false;
}
bool HEP_1000::getCurveCC(const float *cccurve)
{
    if (cccurve) *(float*)cccurve = 0.0f;
    return false;
}

bool HEP_1000::getCurveCV(const float *cvcurve)
{
    if (cvcurve) *(float*)cvcurve = 0.0f;
    return false;
}

bool HEP_1000::getCurveFV(const float *fvcurve)
{
    if (fvcurve) *(float*)fvcurve = 0.0f;
    return false;
}

bool HEP_1000::getCurveTC(const float *tccurve)
{
    if (tccurve) *(float*)tccurve = 0.0f;
    return false;
}

bool HEP_1000::getCurveconfig(const uint16_t *reg)
{
    if (reg) *(uint16_t*)reg = 0;
    return false;
}

bool HEP_1000::getCurveCCTimeout(const uint16_t *cctimeout)
{
    if (cctimeout) *(uint16_t*)cctimeout = 0;
    return false;
}

bool HEP_1000::getCurveCVTimeout(const uint16_t *cvtimeout)
{
    if (cvtimeout) *(uint16_t*)cvtimeout = 0;
    return false;
}

bool HEP_1000::getCurveFloatTimeout(const uint16_t *floattimeout)
{
    if (floattimeout) *(uint16_t*)floattimeout = 0;
    return false;
}


// Misc Stuff
bool HEP_1000::setSystemConfig(const uint16_t *systemconfig)
{
    if (systemconfig) *(uint16_t*)systemconfig = 0;
    return false;
}
bool HEP_1000::getChargeStatus(const uint16_t *chargestatus)
{
    if (chargestatus) *(uint16_t*)chargestatus = 0;
    return false;
}

bool HEP_1000::getSystemConfig(const uint16_t *systemconfig)
{
    if (systemconfig) *(uint16_t*)systemconfig = 0;
    return false;
}

bool HEP_1000::getSystemStatus(const uint16_t *systemstatus)
{
    if (systemstatus) *(uint16_t*)systemstatus = 0;
    return false;
}


// Custom Stuff
bool HEP_1000::setVout(const float* vout)
{
    if (vout) *(float*)vout = 0.0f;
    return false;
}
bool HEP_1000::getMfrData(mfr_data* data)
{
    (void)data;
    return false;
}

bool HEP_1000::getSetVoutCommand(const float* vout)
{
    if (vout) *(float*)vout = 0.0f;
    return false;
}
