/******************************************************************//**
* @file MAX31856.h
*
* @author Devin Alexander
*
* @version 1.0
*
* Started: SEPTEMBER 14th 2017
*
* Updated:
*
* @brief Header file for MAX31856 class
*
***********************************************************************
*
* @copyright
* Copyright (C) 2017 Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
**********************************************************************/


#ifndef MAX31856_h
#define MAX31856_h
#include "mbed.h"

//*****************************************************************************
//Define all the addresses of the registers in the MAX31856
//*****************************************************************************
#define ADDRESS_CR0_READ                   0x00         //Factory Default 00h
#define ADDRESS_CR0_WRITE                  0x80
#define ADDRESS_CR1_READ                   0x01         //Factory Default 03h
#define ADDRESS_CR1_WRITE                  0x81
#define ADDRESS_MASK_READ                  0x02         //Factory Default FFh
#define ADDRESS_MASK_WRITE                 0x82
#define ADDRESS_CJHF_READ                  0x03         //Factory Default 7Fh
#define ADDRESS_CJHF_WRITE                 0x83
#define ADDRESS_CJLF_READ                  0x04         //Factory Default C0h
#define ADDRESS_CJLF_WRITE                 0x84
#define ADDRESS_LTHFTH_READ                0x05         //Factory Default 7Fh
#define ADDRESS_LTHFTH_WRITE               0x85
#define ADDRESS_LTHFTL_READ                0x06         //Factory Default FFh
#define ADDRESS_LTHFTL_WRITE               0x86
#define ADDRESS_LTLFTH_READ                0x07         //Factory Default 80h
#define ADDRESS_LTLFTH_WRITE               0x87
#define ADDRESS_LTLFTL_READ                0x08         //Factory Default 00h
#define ADDRESS_LTLFTL_WRITE               0x88
#define ADDRESS_CJTO_READ                  0x09         //Factory Default 00h
#define ADDRESS_CJTO_WRITE                 0x89
#define ADDRESS_CJTH_READ                  0x0A         //Factory Default 00h
#define ADDRESS_CJTH_WRITE                 0x8A
#define ADDRESS_CJTL_READ                  0x0B         //Factory Default 00h
#define ADDRESS_CJTL_WRITE                 0x8B
#define ADDRESS_LTCBH_READ                 0x0C
#define ADDRESS_LTCBM_READ                 0x0D
#define ADDRESS_LTCBL_READ                 0x0E
#define ADDRESS_SR_READ                    0x0F

//*****************************************************************************
//Define parameters for control register zero (CR0)
//*****************************************************************************
#define CR0_CONV_MODE_NORMALLY_OFF         0x00    //Power On Default value
#define CR0_CONV_MODE_NORMALLY_ON          0x80

#define CR0_1_SHOT_MODE_NO_CONVERSION      0x00    //defaults to this value
#define CR0_1_SHOT_MODE_ONE_CONVERSION     0x40    //^

#define CR0_OC_DETECT_DISABLED             0x00
#define CR0_OC_DETECT_ENABLED_R_LESS_5k    0x10
#define CR0_OC_DETECT_ENABLED_TC_LESS_2ms  0x20
#define CR0_OC_DETECT_ENABLED_TC_MORE_2ms  0x30

#define CR0_COLD_JUNC_ENABLE               0x00    //Power On Default value
#define CR0_COLD_JUNC_DISABLE              0x08    //speed of conversion is sped up by 25ms when this optionis selected (Disable the cold junc)

#define CR0_FAULT_MODE_COMPARATOR          0x00    //Power On Default value
#define CR0_FAULT_MODE_INTERUPT            0x04

#define CR0_FAULTCLR_DEFAULT_VAL           0x00    //defaults to this value
#define CR0_FAULTCLR_RETURN_FAULTS_TO_ZERO 0x02    //^

#define CR0_FILTER_OUT_60Hz                0x00    //Preset value
#define CR0_FILTER_OUT_50Hz                0x01    //^


//*****************************************************************************
//Define parameters for control register one (CR1)
//*****************************************************************************
#define CR1_AVG_TC_SAMPLES_1               0x00    //Power on default value
#define CR1_AVG_TC_SAMPLES_2               0x10
#define CR1_AVG_TC_SAMPLES_4               0x20
#define CR1_AVG_TC_SAMPLES_8               0x30
#define CR1_AVG_TC_SAMPLES_16              0x40

// Define which type of thermocouple the MAX31856 is using. This is for lineariztion purposes
#define CR1_TC_TYPE_B                      0x00
#define CR1_TC_TYPE_E                      0x01
#define CR1_TC_TYPE_J                      0x02
#define CR1_TC_TYPE_K                      0x03         //Power on default value
#define CR1_TC_TYPE_N                      0x04
#define CR1_TC_TYPE_R                      0x05
#define CR1_TC_TYPE_S                      0x06
#define CR1_TC_TYPE_T                      0x07
#define CR1_TC_TYPE_VOLT_MODE_GAIN_8       0x08
#define CR1_TC_TYPE_VOLT_MODE_GAIN_32      0x0C

//*****************************************************************************
//Define parameters for the mask register (MASK)
//*****************************************************************************
#define MASK_CJ_FAULT_THRESHOLD_HIGH       0x20
#define MASK_CJ_FAULT_THRESHOLD_LOW        0x10
#define MASK_TC_FAULT_THRESHOLD_HIGH       0x08
#define MASK_TC_FAULT_THRESHOLD_LOW        0x04
#define MASK_OVER_UNDER_VOLT_FAULT         0x02
#define MASK_OPEN_CIRCUIT_FAULT            0x01

//*****************************************************************************
//If these defined values are &= (bitwise ANDed) with the contents of a register, it will reset the bits pertaing to the specific bitfields to zero
//*****************************************************************************
#define CR0_CLEAR_BITS_7                   ~(0x80)
#define CR0_CLEAR_BITS_6                   ~(0x40)
#define CR0_CLEAR_BITS_5_4                 ~(0x30)
#define CR0_CLEAR_BITS_3                   ~(0x08)
#define CR0_CLEAR_BITS_2                   ~(0x04)
#define CR0_CLEAR_BITS_1                   ~(0x02)
#define CR0_CLEAR_BITS_0                   ~(0x01)

#define CR1_CLEAR_BITS_6_4                 ~(0x70)
#define CR1_CLEAR_BITS_3_0                 ~(0x0F)

#define MASK_CLEAR_BITS_5                  ~(0x20)
#define MASK_CLEAR_BITS_4                  ~(0x10)
#define MASK_CLEAR_BITS_3                  ~(0x08)
#define MASK_CLEAR_BITS_2                  ~(0x04)
#define MASK_CLEAR_BITS_1                  ~(0x02)
#define MASK_CLEAR_BITS_0                  ~(0x01)

//*****************************************************************************
///Parameters that are used throughout the library
//*****************************************************************************
#define TC_MAX_VAL_FAULT                   1800
#define TC_MIN_VAL_FAULT                   -210
#define CJ_MAX_VAL_FAULT                   125
#define CJ_MIN_VAL_FAULT                   -55



/**
 * @brief Library for the MAX31856\n
 * The MAX31856 thermocouple temperature sensor accurately measures temperature
 * and provides a vast amount of features such as:
 * //FEATURE
 * //FEATURE
 * //FEATURE
 * //FEATURE
 * //FEATURE
*  //FEATURE
 * Communication is through an SPI-compatible interface.
 *
 * @code
 * #include "mbed.h"
 * #include "MAX31856.h"
 *
 *
 * // Hardware serial port
 * Serial serial(USBTX, USBRX);
 *
 * //SPI communications
 * SPI spi(SPIO MOSI,SPIO MISO,SPIO SCK);
 *
 * //Thermocouples
 * MAX31856 Thermocouple1(spi, CHIPSELECT);
 *
 *
 * int main(void)
 * {
 *      float temperature_TC_1, temperature_CJ_1;
 *      while(true)
 *      {
 *          temperature_TC_1=Thermocouple1.readTC();
 *          temperature_CJ_1=Thermocouple1.readCJ();
 *          serial.printf("MAX31856 TC = %f Celsius   MAX31856 CJ = %f Celsius  \n\r",temperature_TC_1,temperature_CJ_1);
 *          wait(1.0);
 *      }
 * }
 * @endcode
 */



/** Please see pages 18-26 in the MAX31856 data sheet to see what register bit masks are needed to be set
to achieve functionality desired. The data sheet can be found at

           ***     https://datasheets.maximintegrated.com/en/ds/MAX31856.pdf      ***
*/


/**
* MAX31856 Class
*/
class MAX31856
{

public:
	//*****************************************************************************
	//Constructor and Destructor for the class
	//*****************************************************************************
	/**
	* @brief Constructor to create MAX31856 object with SPI information as well as preconfiguration parameter settings in configuration registers Zero and One
	* @param _spi - Reference to SPI object
	* @param _ncs - Chip Select for SPI comunications with the oject
	* @param _type - Type of thermocouple used
	* @param _fltr - Feature of the MAX31856 to filter out either 50Hz/60Hz from signal
	* @param _samples - How many samples are averaged for one conversion
	* @param _conversion_mode - Choose between always on and making conversions and off in between requests for a reading
	*/
	MAX31856(SPI& _spi, PinName _ncs, uint8_t _type=CR1_TC_TYPE_K, uint8_t _fltr=CR0_FILTER_OUT_60Hz, uint8_t _samples=CR1_AVG_TC_SAMPLES_1, uint8_t _conversion_mode=CR0_CONV_MODE_NORMALLY_OFF);


	/** @brief Destructor */
	~MAX31856(void);


	//*****************************************************************************
	//Temperature Functions
	//*****************************************************************************
	/**
	* @brief  Requests read of the thermocouple temperature
	* @return float of the converted thermocouple reading based on current configurations
	*/
	float readTC();


	/**
	* @brief  Requests read of the cold junction temperature
	* @return float of the converted artificial cold junction reading based on current configurations
	*/
	float readCJ();


	//*****************************************************************************
	//Functions for register CR0
	//*****************************************************************************
	/**
	* @brief  Sets bits in the configuration register zero for setting the rate of conversions
	* @param val    \li  CR0_CONV_MODE_NORMALLY_OFF      (Power On Default value)
	*               \li  CR0_CONV_MODE_NORMALLY_ON
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setConversionMode(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for enabling one conversion to take place
	* @param val    \li   CR0_1_SHOT_MODE_NO_CONVERSION      (Power On Default value)
	*               \li   CR0_1_SHOT_MODE_ONE_CONVERSION      (This bit self clears itself to default back to CR0_1_SHOT_MODE_NO_CONVERSION after singular conversion takes place)
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setOneShotMode(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for configuring open circuit fault detection
	* @param val    \li   CR0_OC_DETECT_DISABLED      (Power On Default value)
	*               \li   CR0_OC_DETECT_ENABLED_R_LESS_5k
	*               \li   CR0_OC_DETECT_ENABLED_TC_LESS_2ms
	*               \li   CR0_OC_DETECT_ENABLED_TC_MORE_2ms
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setOpenCircuitFaultDetection(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for disabling or enabling the Cold Junction
	* @param val    \li   CR0_COLD_JUNC_ENABLE      (Power On Default value)
	*               \li   CR0_COLD_JUNC_DISABLE
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setColdJunctionDisable(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for setting fault mode status
	* @param val    \li   CR0_FAULT_MODE_COMPARATOR      (Power On Default value)
	*               \li   CR0_FAULT_MODE_INTERUPT
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setFaultMode(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for clearing fault status
	* @param val    \li   CR0_FAULTCLR_DEFAULT_VAL      (Power On Default value)
	*               \li   CR0_FAULTCLR_RETURN_FAULTS_TO_ZERO      (This bit self clears itself to default back to CR0_FAULTCLR_DEFAULT_VAL after fault status is cleared)
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setFaultStatusClear(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register zero for setting which of the two filter modes either 50Hz or 60Hz cancelation
	* @param val    \li   CR0_FILTER_OUT_60Hz      (Power On Default value)
	*               \li   CR0_FILTER_OUT_50Hz
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setEmiFilterFreq(uint8_t val);


	//*****************************************************************************
	//Functions for register CR1
	//*****************************************************************************
	/**
	* @brief  Sets bits in the configuration register one for setting how many readings are taken
	* @param val    \li   CR1_AVG_TC_SAMPLES_1      (Power On Default value)
	*               \li   CR1_AVG_TC_SAMPLES_2
	*               \li   CR1_AVG_TC_SAMPLES_4
	*               \li   CR1_AVG_TC_SAMPLES_8
	*               \li   CR1_AVG_TC_SAMPLES_16
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setNumSamplesAvg(uint8_t val);


	/**
	* @brief  Sets bits in the configuration register one for setting which thermocouple type is going to be programmed into the MAX31856 for linearization of thermovoltage produced and temperature
	* @param val    \li   CR1_TC_TYPE_B
	*               \li   CR1_TC_TYPE_E
	*               \li   CR1_TC_TYPE_J
	*               \li   CR1_TC_TYPE_K      (Power On Default value)
	*               \li   CR1_TC_TYPE_N
	*               \li   CR1_TC_TYPE_R
	*               \li   CR1_TC_TYPE_S
	*               \li   CR1_TC_TYPE_T
	*               \li   CR1_TC_TYPE_VOLT_MODE_GAIN_8
	*               \li   CR1_TC_TYPE_VOLT_MODE_GAIN_32
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setThermocoupleType(uint8_t val);


	//*****************************************************************************
	//Functions for register MASK
	//*****************************************************************************
	/**
	* @brief  Sets bits in the configuration register one for setting fault masks
	* @param val    \li   MASK_CJ_FAULT_THRESHOLD_HIGH
	*               \li   MASK_CJ_FAULT_THRESHOLD_LOW
	*               \li   MASK_TC_FAULT_THRESHOLD_HIGH
	*               \li   MASK_TC_FAULT_THRESHOLD_LOW
	*               \li   MASK_OVER_UNDER_VOLT_FAULT
	*               \li   MASK_OPEN_CIRCUIT_FAULT
	* @param enable \li  0 for disabling the mask in whichever option is selcted in parameter val
	*               \li  1 for enabling the mask in whichever option is selcted in parameter val
	* @return       \li 1 on success
	*               \li 0 if there is an incorrect parameter that is passed in as parameter val
	*/
	bool setFaultMasks(uint8_t val, bool enable);


	/**
	* @brief  Sets bits in the configuration register one for setting thresholds that corespond to the fault mask settings
	* @param val    \li   MASK_CJ_FAULT_THRESHOLD_HIGH
	*               \li   MASK_CJ_FAULT_THRESHOLD_LOW
	*               \li   MASK_TC_FAULT_THRESHOLD_HIGH
	*               \li   MASK_TC_FAULT_THRESHOLD_LOW
	* @param temperature value that you want to program into a threshold register for temperatre
	* @return return value that was programmed into the threshold register
	*/
	bool setFaultThresholds(uint8_t val, float temperature);


	//*****************************************************************************
	//Check Fault Status Functions
	//*****************************************************************************
	/**
	* @brief  Check the fault stautus register to see if there is anything wrong with range of thermocouple temperature
	*         whether outside opperating temperatures or if above/below thresholds that are set
	* @return       \li 0 if no faults are present
	*               \li 1 if Thermocouple temp is higher than the threshold
	*               \li 2 if Thermocouple temp is lower  than the threshold
	*               \li 3 if Thermocouple temp is outside operating range of termocouple type
	*               \li 4 if Thermocouple temp is higher than the threshold && is outside operating range of termocouple type
	*               \li 5 if Thermocouple temp is lower  than the threshold && is outside operating range of termocouple type
	*/
	uint8_t checkFaultsThermocoupleThresholds();


	/**
	* @brief  Check the fault stautus register to see if there is anything wrong with range of cold junction temperature
	*         whether outside opperating temperatures or if above/below thresholds that are set
	* @return       \li 0 if no faults are present
	*               \li 1 if Cold Junction temp is higher than the threshold
	*               \li 2 if Cold Junction temp is lower  than the threshold
	*               \li 3 if Cold Junction temp is outside operating range of termocouple type
	*               \li 4 if Cold Junction temp is higher than the threshold && is outside operating range of termocouple type
	*               \li 5 if Cold Junction temp is lower  than the threshold && is outside operating range of termocouple type
	*/
	uint8_t checkFaultsColdJunctionThresholds();


	/**
	* @brief  Check the fault stautus register to see if there is anything wrong with thermocouple connection to the MAX31856
	* @return       \li 1 if no faults are present
	*               \li 0 if there is a fault and there needs to be information printed to the console to help diagnose issues
	*/
	bool checkFaultsThermocoupleConnection();


//*****************************************************************************
//General Functions
//*****************************************************************************
	/**
	* @brief This function is to read current contents of register, manipulate the contents, then rewrite the specific register\n
	*               \li Read the value of a register from contents of register matching the parameter read_address
	*               \li Clear the bits needed to be changed by bitwise ANDing the read value with the 8 bit parameter clear_bits
	*               \li Set the bits of interest in the 8 bit value by bitwise ORing the value from step two with parameter val
	*               \li Rewrite to the register with the new 8 bit value to the register with the address with parameter write_address
	* @param read_address - Address of register to read the data before it's changed
	* @param write_address - Address of register to rewrite the changed data
	* @param clear_bits - Parameter that is
	* @param val - Bitfield that contains bits related to function specific settings
	* @return       \li 1 on success
	*/
	bool registerReadWriteByte(uint8_t read_address, uint8_t write_address, int clear_bits, uint8_t val);


	/**
	* @brief This function is to read current contents of register, manipulate the contents, then rewrite the specific register\n
	*               \li Read the value of a register from contents of register matching the parameter read_address
	*               \li Clear the bits needed to be changed by bitwise ANDing the read value with the 8 bit parameter clear_bits
	*               \li Set the bits of interest in the 8 bit value by bitwise ORing the value from step two with parameter val
	*               \li Write to the register with the new 8 bit value to the register with the address with parameter write_address
	* @param write_address - Address of register to rewrite the changed data
	* @param val - Byte of information that is going to be written to the regitser with the address that matches the parameter write_address
	* @return   \li 1 on success
	*/
	bool registerWriteByte(uint8_t write_address, uint8_t val);


	/**
	* @brief This function is to read current contents of register by passing in the address of the read address and return contents of the register
	* @param read_address - Address of register to read data from
	* @return   \li byte contained in the address
	*/
	uint8_t registerReadByte(uint8_t read_address);


	/**
	* @brief This function is to read current contents of register by passing in the address of the read address and return contents of the register
	* @param temperature - Float of value to offest the value of the cold junction offset by (must be between -8°C to +7.9375°C)
	* @return   \li 1 on successfully updated coldjunction offset
	*           \li 0 if parameter temperature does not fall between range -8°C to +7.9375°C
	*/
	bool coldJunctionOffset(float temperature);


private:

//*****************************************************************************
//Private Functions
//*****************************************************************************
	/** @brief  Writes the chip seleect pin low to begin SPI communications */
	void spiEnable();


	/** @brief  Writes the chip seleect pin high to end SPI communications */
	void spiDisable();

	/** @brief  Calculates minimum wait time for a conversion to take place */
	void calculateDelayTime();


//*****************************************************************************
//Private Members
//*****************************************************************************
	/// SPI object
	SPI& spi;

	/// Chip select pin for SPI communications
	DigitalOut ncs;

	/// Number of samples the thermocouple is configured to average
	uint8_t samples;

	/// 0=thermocouple is set to one of 8 thermocouple types   and   1=Thermocouple is configured to report in voltage mode
	bool voltage_mode;

	/// 0=60Hz   and   1=50Hz
	bool filter_mode;

	/// 0=MAX31856 is off, so no conversion is taking place currently   and   1=Always On and converting
	bool conversion_mode;

	/// 0=cold junction is disabled   and   1=cold junction is enabled
	bool cold_junction_enabled;

	///Define a return val for all boolean functions
	bool return_val;

	///Used to figure out when a new conversion is ready to go
	uint32_t lastReadTime;

	///How many conversions have taken place since conversion mode was switched into auto mode
	///Also this value should be 0 if the mode is in oneshot mode
	uint32_t thermocouple_conversion_count;

	///time in milliseconds that is needed minimum for a new conversion to take place
	uint32_t conversion_time;
};

#endif  /* __MAX31856_H_ */
