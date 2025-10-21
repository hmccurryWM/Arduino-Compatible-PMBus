//#include <cstdint>
#include <cstdint>
#include <filesystem>
#include <stdio.h>
#include <stddef.h>
//#include <stdint.h>
#include <stdlib.h>
#include <Wire.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_I2CDevice.h>
#include "PMBus_Struct.h"

#ifndef PMBus_ABIO_H
#define PMBus_ABIO_H

// Uncomment the below #define to enable debugging print statements.
// NOTE: You must call Serial.being(<baud rate>) in your setup() for this to work
// #define RPB_1600_DEBUG

#define GOOD 0x01
#define NOTBEGUNERR 0x02
#define NOWRITEERR 0x04
#define NOREADERR 0x08
#define BADBUFFER 0x10
/* 
// PMBus v1.1 Capability Bits
#define CRCCAPABILITY 0x80
#define BUSCAPABILITY 0x60
#define ALERTCAPABILITY 0x10

// PMBus v1.1 Operation bits
#define OPERATIONIMMOFF 0X00
#define OPERATIONSOFTOFF 0X40
#define OPERATIONON 0X80 */


class PMBus_ABIO
{
protected:
    Adafruit_I2CDevice *i2c_dev;
    buffer_data *rxbuffer;
    uint8_t runningClk;
    mfr_data *data;
    capability *capa;
    status *stats;
    PMBusStatus *internalStatus;
    operationByte *Operations;
#ifdef USECHARGER
    curve_config *CConfig;
    curve_parameters *CParms;
    charge_status *ChgStat;
#endif
public:
    // Constructor, twi is the 2-wire interface, the clk during and after is to keep compatibility
    // with other devices that might use different frequencies
    PMBus_ABIO(TwoWire *wire = &Wire, uint32_t clkDuring = 100000UL, uint32_t clkAfter = 100000UL);

    // Destructor
    ~PMBus_ABIO(void);

    /**
     * @brief Buffer to hold bytes received over i2c
     */
    uint8_t my_rx_buffer[MAX_RECEIVE_BYTES];

    uint8_t Init(uint8_t i2caddr);

    /**
     * @brief Query charger for voltage & current readings, populate a "readings" struct
     * @return true on successful read, false otherwise
     */
    bool getReadings(readings *data);
#ifdef USECHARGER
    /**
     * @brief Query charger for status bytes, populate a "charge_status" struct
     * @return true on successful read, false otherwise
     */
    bool getChargeStatus(charge_status *status);

    
#endif
    /**
     * @brief Write arbitrary bytes with commandID
     * @return true on successful write, false otherwise
     */
    uint8_t writeTwoBytes(uint8_t commandID, uint8_t *data, uint8_t length = 2);

    // val is desired value (ignored if nothing being written)
    // Item selects the thing being written
    bool setControl(uint8_t val, SystemConfig item);

    /**
     * @brief Write 16b linear value with specified commandID & N
     * @details See the PMBus 1.1 Spec for more info on how the linear data format works
     * @param N the exponent
     * @param value the value you want to write (NOT the Y value)
     * @return True on success, false on failure
     */
    bool writeLinearSixteenBitDataCommand(uint8_t commandID, int8_t N, float value);

    /**
     * @brief Write 11b linear value with specified commandID & N
     * @details See the PMBus 1.1 Spec for more info on how the linear data format works
     * @param N the exponent
     * @param value the value you want to write (NOT the Y value)
     * @return True on success, false on failure
     */
    bool writeLinearElevenBitDataCommand(uint8_t commandID, int8_t N, float value);

    /**
     * @brief Sends commandID to the charger, and reads the receiveLength byte(s) long response into my_rx_buffer[]
     * @return true if we received the number of bytes we were expecting, false otherwise.
     */
    uint8_t readWithCommand(uint8_t commandID, uint8_t receiveLength);

    /**
    * @brief Pulls the mfgr information
    * @return true if we received the number of bytes we were expecting, false otherwise
    */
    bool readManufData(mfr_data *mfrdata);

    /**
     * @brief Parses the first two bytes of my_rx_buffer[] in the "Linear Data" format outlined int the PMBus Specification
     * @details see the PMBus V1.1 Section 7.1 "Linear Data Format" for more info
     */
     uint16_t parseLinearData(void);

     /**
      * @brief Returns the RX buffer
      * @return full RX buffer of type buffer_data
      */
    bool returnBufferData(buffer_data *bufferhandle);

    /**
     * @brief Pulls the capabilities from the device to an internal structure
     * @return returns an error code
     */
    uint8_t getCapability();

    /**
     * @brief Returns the capabilities from the device for nebulous future uses
     * @return returns the capability data object
     */
    uint8_t returnCapability(capability* data);

    bool pullStatus();

    bool print_status_bits(PMBusStatus *status);

    bool runOperation(operationByte* ops);

    bool runOperation(OperationFields opfield, uint8_t bitPosition, uint8_t bitMask, uint8_t value);

    float parseOutputCurrent();

    float readOutputCurrent();

    bool writeVoutTrim(float trim, int8_t N, float lowerBound, float upperBound);

    bool setOCFaultLimit(float currentLimit, int8_t N);

    bool parseOperation(operationByte* ops);

    bool runArbitraryOp(char *stringin);

    bool parseOnOffConfig(OnOffConfigByte *oocb, uint8_t *reg);

    bool clearLocalBuffer(buffer_data* RXBUFFER);

    void printBinary(buffer_data *value);

    bool writeSyllableLinearDataHelper(uint8_t commandID, int16_t Y);

    bool returnChargeConfig(curve_config *config);

private:
    #if ARDUINO >= 157
  uint32_t wireClk;    ///< Wire speed for SSD1306 transfers
  uint32_t restoreClk; ///< Wire speed following SSD1306 transfers
#endif

    /**
     * @brief The address of the charger we're communicating with
     * @details This is set using the A0, A1, and A2 pins on the RPB-1600. These three pins control
     * the lowest 3 bits of the 7 bit address, and the MSB is always 1. For example, if all the
     * pins are tied high, the address would be 0x47.
     * @note Address 0 is a reserved address.
     */
    uint8_t pmbus_addr;

    /**
     * @brief Helper for writing linear data with a specified commandID
     * @details See the PMBus 1.1 Spec for more info on how the linear data format works
     * @param N the exponent
     * @param Y the mantissa
     * @return True on success, false on failure
     */
    bool writeLinearDataHelper(uint8_t commandID, int8_t N, int16_t Y);

    /**
     * @brief Parse a voltage reading in the linear format
     * @details See the PMBus 1.1 spec section 8.3.1 for more info
     */
    float parseLinearVoltage(int8_t N);
#ifdef USECHARGER
    /**
     * @brief Parses the first two bytes of my_rx_buffer[] into a curve_config struct and returns it via argument.
     * @details Meant to be called after calling readWithCommand(CMD_CODE_CURVE_CONFIG, CMD_LENGTH_CURVE_CONFIG)
     */
    void parseCurveConfig(curve_config *config);

    /**
     * @brief Parses the first two bytes of my_rx_buffer[] into a charge_status struct and returns it via argument.
     * @details Meant to be called after calling readWithCommand(CMD_CODE_CHG_STATUS, CMD_LENGTH_CHG_STATUS)
     */
    void parseChargeStatus(charge_status *status);
#endif
    /**
     * @brief Takes in a twos complement number that's length bits and converts it to a 16 bit twos complement number
     * @details Slightly modified version of this https://www.codeproject.com/Tips/1079637/Twos-Complement-for-Unusual-Integer-Sizes
     */
    int16_t UpscaleTwosComplement(int16_t value, size_t length);

    /**
     * @brief Zeros my_rx_buffer
     */
    void clearRXBuffer(void);

    bool readFromWrapper(uint8_t len);

    bool parse_status_word(buffer_data *status_bytes, PMBusStatus *status);


};

#endif // RPB_1600_H