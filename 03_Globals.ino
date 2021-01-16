/*
 * ###############################################################################################
 * ###############################################################################################
 * GLOBAL VARIABLES
 * ###############################################################################################
 * ###############################################################################################
 */

static const char COMPILEDATE[] PROGMEM = __DATE__;
static const char COMPILETIME[] PROGMEM = __TIME__;
//static const char COMPILEFILE[]  = __FILE__;

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Enum for System Wellbeing state and/or Error of subsystems
 * 
 * non-error is 0
 * error values must be non-zero and 2^x
 * error values for x: from 1 .. 7 allowed (if more needed, change using variables datatype from uint8_t to bigger.
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
enum system_wellbeing_type{
	ALL_IS_WELL = 0,
	MAIN_IS_ERROR = 1,
	CLOCK_IS_ERROR = 2,
	LDR_IS_ERROR = 4,
	DHT_IS_ERROR = 8,
	SDC_IS_ERROR = 16,
	SERIAL_IS_ERROR = 32 //,
	//reserved = 64,
	//reserved = 128	
};

static system_wellbeing_type system_wellbeing;

// before first iteration of loop set a flag
static bool flag_first_time_in_loop;

// at startup system is in a phase in 
static bool is_phase_in;


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * SDcard objects
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
// File system object.
SdFat sd;

// Log file.
SdFile file;



/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * RTC objects
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
RTC_DS3231 rtc;
DateTime rtcNow;
DateTime startUpTime;



/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * DHT objects
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
dht DHT;
// odd-sized ring buffer is best-suited for median-calculation
RunningMedian dht_temp_samples = RunningMedian(5);
RunningMedian dht_humidity_samples = RunningMedian(5);

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * LDR objects
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
// odd-sized ring buffer is best-suited for median-calculation
RunningMedian ldr_samples = RunningMedian(5);


/*
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Declaration of all functions
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */

extern uint8_t init_clock_subsystem();
extern void set_rtc_to_utc_of_compiletime();

extern uint8_t init_sdcard_subsystem();
extern uint8_t touch_or_open_daily_logfile();
extern int8_t check_headerfile();
extern uint8_t open_logfile(char *fileName);
extern uint8_t sync_logfile();
extern uint8_t close_logfile();
extern void write_one_csv_line_to_sdc();

extern uint8_t init_serial_subsystem();

void init_dht_subsystem();
void get_dht_measures();

void get_ldr_measure();

extern void setup();

extern void loop();
