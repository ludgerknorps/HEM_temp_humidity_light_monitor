/*
 * ###############################################################################################
 * ###############################################################################################
 * Functions around Clock system
 * ###############################################################################################
 * ###############################################################################################
 */

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	init_clock_subsystem
 * Does:			setup RTC, set system time to RTC time, apply timezone
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Blocking:		yes, in case of error; goes into endless loop
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t init_clock_subsystem(){

	uint8_t my_wellbeing = CLOCK_IS_ERROR; // default, reset at end iff successful
	
	if (! rtc.begin()) {
		DebugSerial.println(F("Couldn't find RTC"));
		DebugSerial.flush();
		abort();
	}

	#ifdef SETRTCTOCOMPILETIMEUTC
		set_rtc_to_utc_of_compiletime();
	#endif 

	if (rtc.lostPower()) {
		DebugSerial.println(F("RTC lost power, this is a critical error in a production system, I am using 1980-01-01T00:00:00 instead!"));
		rtc.adjust(DateTime(1980, 1, 1, 0, 0, 0));
	}
	
  rtcNow = rtc.now();
  startUpTime = DateTime(rtcNow);

	print_rtcNow_to_serial();
}

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	print_rtcNow_to_serial
 * Does:			take global rtcNow and print it to serial
 * Parameters:		none
 * 					some constants are used
 * 					uses global rtcNow and Serial
 * Returns:			void
 * Blocking:		notimebuffer
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void print_rtcNow_to_serial(){

	//macroDateTimeToCharBuffer(rtcNow,timebuffer);
  char charbuffer[CLOCK_DATETIME_STRING_FORMAT_LENGTH]; 
  strcpy_P(charbuffer, CLOCK_DATETIME_STRING_FORMAT); 
  rtcNow.toString(charbuffer);
  
  DebugSerial.print(F("RTC is setup to (UTC) time "));
  DebugSerial.println(charbuffer);
	DebugSerial.flush();

}


//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	set_rtc_to_utc_of_compiletime
 * Does:			set RTC to UTC of __DATE__ and __TIME__; it respects timezones 
 * Parameters:		none
 * 					some constants are used
 * Returns:			void
 * Blocking:		no
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void set_rtc_to_utc_of_compiletime(){

	uint8_t number_of_hours_to_adjust = 0;
	
	DateTime compile_time = DateTime(F(__DATE__), F(__TIME__));
  char timebuffer[CLOCK_DATETIME_STRING_FORMAT_LENGTH]; 
  strcpy_P(timebuffer, CLOCK_DATETIME_STRING_FORMAT); 
  compile_time.toString(timebuffer);
	//macroDateTimeToCharBuffer(compile_time,timebuffer);

	DebugSerial.print(F("CompileTime is "));
  	DebugSerial.println(timebuffer);
	DebugSerial.flush();

	// now correct for daylight saving time and timezone - we use Berlin timezone hardcoded!
	// this means:
	// 		standardtime 	= UTC+01:00
	//		DST				= UTC+02:00
	//		DST starts last sunday in March UTC..-..-..T03:00:00+01:00
	// 		DST ends last sunday in October UTC..-..-..T04:00:00+02:00
	
	
	#ifndef USEDAYLIGHTSAVINGTIME
		// use STD time --> set RTC to compile time -1h
		number_of_hours_to_adjust = 1;
		DebugSerial.print(F("Now adjusting CompileTime using Berlin STD time (UTC-01:00)"));
		DebugSerial.flush();
	#else
		// use DST time --> set RTC to compile time -2h
		number_of_hours_to_adjust = 2;
		DebugSerial.print(F("Now adjusting CompileTime using Berlin DST (UTC-02:00)"));
		DebugSerial.flush();
	#endif

	// now actually set RTC
	rtc.adjust(compile_time - TimeSpan(0,number_of_hours_to_adjust,0,0));
}


//EOF
