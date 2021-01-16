/*
 * ###############################################################################################
 * ###############################################################################################
 * System options
 * ###############################################################################################
 * ###############################################################################################
 */

#define DEBUG_SERIAL true
#define DebugSerial if ( DEBUG_SERIAL ) Serial

// uncomment to set RTC to compiletime
//#define SETRTCTOCOMPILETIMEUTC

// uncomment if compile-time was during aylight saving time (Sommerzeit)
//#define USEDAYLIGHTSAVINGTIME

 // Serial speed
#define SERIAL_SPEED_BAUD 115200



/*
 * ###############################################################################################
 * ###############################################################################################
 * PINS
 * ###############################################################################################
 * ###############################################################################################
 */

// SD chip select pin for SD Card
const uint8_t CS_SDC_PIN = 10;


// analog input from LDR
const uint8_t IN_LDR_PIN = A0;


// input from DHT22
const uint8_t IN_DHT_PIN = 7;



/*
 * ###############################################################################################
 * ###############################################################################################
 * Constants
 * ###############################################################################################
 * ###############################################################################################
 */

/* 
* -----------------------------------------------------------------------------------------------
* Constants for Main System
* -----------------------------------------------------------------------------------------------
*/

// Values to write into second column of CSV file
// providing info on system status
#define WARNING_PHASE_IN_AFTER_STARTUP 1  // first minutes after startup could be non-representative values,
                                          //e.g. because housing of system is warmer than real room if system was just installed etc.
#define PHASE_IN_TIME 30 // in minutes


/* 
* -----------------------------------------------------------------------------------------------
* Constants for SDCard SubSystem
* -----------------------------------------------------------------------------------------------
*/

// Filename format of daily files on SD card
const char CSV_FILE_NAME_PATTERN[] PROGMEM = "YYYYMMDD";
const uint8_t CSV_FILE_NAME_PATTERN_LENGTH = sizeof(CSV_FILE_NAME_PATTERN);

const static char CSV_SEPARATOR = ',';

// Suffix of daily files on SD card
const char CSV_FILE_SUFFIX[] PROGMEM = ".csv";
const uint8_t CSV_FILE_SUFFIX_LENGTH = sizeof(CSV_FILE_SUFFIX);

// HeaderFile (this is a separate file with filename "smaller" than all daily logfiles
// thus all files can easily be concattenated and then you have a single (big) csv file
const char CSV_HEADERFILE_NAME[] PROGMEM = "00000000.csv";
const static char CSV_HEADERFILE_CONTENT[] PROGMEM = "UTC DateTime,Error,Temperature,Humidity,Light";

// System Status file (this is a separate file with filename "bigger" than all daily logfiles - so it's always at the file-system "end")
const char CSV_STATUSFILE_NAME[] PROGMEM = "sysstat.txt";
const uint8_t CSV_STATUSFILE_NAME_LENGTH = sizeof(CSV_STATUSFILE_NAME);

/* 
 * -----------------------------------------------------------------------------------------------
 * Constants for LDR Light Measurement
 * -----------------------------------------------------------------------------------------------
 */

// ring buffer size - how many single samples are taken for median calculation (median is given to main system as LDR measurement) 
// should be odd number
const uint8_t LDR_RING_BUFFER_SIZE = 5;

/* 
 * -----------------------------------------------------------------------------------------------
 * Constants for DHT Temperature and Humidity Measurement
 * -----------------------------------------------------------------------------------------------
 */

// ring buffer size - how many single samples are taken for median calculation (median is given to main system as LDR measurement) 
// should be odd number
const uint8_t DHT_RING_BUFFER_SIZE = 5;

/* 
 * --------------------------------------------------
// this is a macro which creates a new char[] of length CLOCK_DATETIME_STRING_FORMAT_LENGTH and fill it with the CLOCK_DATETIME_STRING_FORMAT
// this char[] is then used (filled) with a DateTime using DateTime::toString(CLOCK_DATETIME_STRING_FORMAT)
//#define macroDateTimeToCharBuffer(datetime,charbuffer)  char charbuffer[CLOCK_DATETIME_STRING_FORMAT_LENGTH]; strcpy(charbuffer, CLOCK_DATETIME_STRING_FORMAT); datetime.toString(charbuffer);---------------------------------------------
 * Constants for Clock Subsystem
 * -----------------------------------------------------------------------------------------------
 */

const char CLOCK_DATETIME_STRING_FORMAT[] PROGMEM = "YYYY-MM-DDThh:mm:ss";
const uint8_t CLOCK_DATETIME_STRING_FORMAT_LENGTH = sizeof(CLOCK_DATETIME_STRING_FORMAT);

// EOF
