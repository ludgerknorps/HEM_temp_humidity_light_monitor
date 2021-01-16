/*
 * A logger/monitor for
 * 		temperature
 * 		air humidity
 * 		light
 * 		
 * writes to an sd card
 * 		one file YYYYMMDD.csv per day
 * 		columns see below
 * 		
 * Systemdesign:
 * 		1x Arduino Pro Mini
 * 		1x DS3231 RealTimeClock with own CR3032 battery
 * 		1x SD-Card 
 * 		1x DHT22 sensor
 * 		1x LDR sensor
 * 		
 * 		1x power supply for 230V
 * 		
 * options for latter
 * 		OPT-A	extension with NRF24 radio-module for sending current data / and or dump saved csv files
 * 		OPT-B 	extension for photo transistor to monitor an electricity meter
 * 		
 * CSV Format:
 *    UTC DateTime,Error,Temperature,Humidity,Light
 * 		COL1	YYYY-MM-DDTHH;mm:SS				19 Byte 	Timedate UTC (z.B. 1985-04-12T23:20:50)
 * 		COL2	-TT.TT							6 Byte		measured temp
 * 		COL3	HH.HH							5 Byte		measured humidity
 * 		COL4	LLLL							4 Byte		measured light
 * 		
 * 		Payload is therefore  34 Byte + 4 "," + Newline (2 Byte) each row --> 40 bytes ~= 50 bytes / minute
 * 		
 * 		1 row per minute --> each day = 50 * 1440 = 59040 Byte ~= 60kB / day
 * 		
 * 		each year = 60kB * 365 ~ 22MB
 * 	
 * 		
 * 		
 * libraries used:
 * 		SdFat				SD-Card
 * 		
 * 		RTClib				DS3231 RealTimeClock Modul
 * 		
 * 		SPI
 * 		TinyWire			(via RTClib)
 * 		
 * 		DHTLib				DHT22 sensor
 * 		
 * 		RunningMedian		ring buffer with median-filter
 * 		
 * PINs:
 * 		A4 / SDA			I2C Wire SDA		--> SDA from DS3231 Modul
 *		A5 / SCL			I2C Wire SCL		--> SCL from DS3231 Modul
 *		
 *		A0					Analog Input		--> 10k to Vcc; LDR from GND; Input for light measurement
 *		
 *		7					Digital In			--> Data from DHT22 module
 *		
 *		10					CS					--> CS from SDCardReader
 *		11					MOSI				--> MOSI from SDCardReader
 *		12					MISO				--> MISO from SDCardReader
 *		13					SCK					--> SCK from SDCardReader
 *		
 *		6					CS2 (Reserved)		--> reserved for OPT-A CS for NRF24 module
 *		2					ExINT (Reserved)	--> reserved for OPT-B connector with ext. interrupt for photo transistor 
 *		
 * 		
 * 		
 */


/*
 * ###############################################################################################
 * ###############################################################################################
 * INCLUDES
 * ###############################################################################################
 * ###############################################################################################
 */
//#include <avr/pgmspace.h>

// SPI für SDCard
#include <SPI.h>
// SDCard
#include "SdFat.h"

// RTC
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

// DHT
#include <dht.h>

// Ring buffer for measurement-values
#include <RunningMedian.h>


// EOF
