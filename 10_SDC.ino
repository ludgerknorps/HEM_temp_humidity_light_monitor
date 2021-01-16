/*
 * ###############################################################################################
 * ###############################################################################################
 * Functions around SDcard subsystem for storing measurments permanently
 * ###############################################################################################
 * ###############################################################################################
 */


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	init_sdcard_subsystem
 * Does:			setup sdcard for read use
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Blocking:		yes, in case of error; goes into endless loop
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t init_sdcard_subsystem(){

	uint8_t my_wellbeing = SDC_IS_ERROR; // default, reset at end iff successful
	
	// Initialize at the highest speed supported by the board that is
	// not over 50 MHz. Try a lower speed if SPI errors occur.
	if (!sd.begin(CS_SDC_PIN, SD_SCK_MHZ(50))) {
		sd.initErrorHalt(); // this is blocking!
		
	}

	// write information to system-status-file on SDC
	update_statusfile(" : System boot");

	check_headerfile();

	my_wellbeing = touch_or_open_daily_logfile();
	return my_wellbeing;

}

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	touch_or_open_daily_logfile
 * Does:			generate current date in correct format frmo RTC, open corresponding logfile on SDC
 * 					called at startup and at every midnight
 * 					closes the previously opened file if any 
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Serial:			errormessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t touch_or_open_daily_logfile(){

	uint8_t my_wellbeing = SDC_IS_ERROR; // default, reset at end iff successful
	
	close_logfile();
	
	// get current daily filename from RTC date
	char fileName[13];
	strcpy_P(fileName,CSV_FILE_NAME_PATTERN); 
	rtcNow = rtc.now();
	rtcNow.toString(fileName); // fileName now holds the "rtcNow" year, month, day
	// add suffix
	strcat_P(fileName, CSV_FILE_SUFFIX);
	DebugSerial.print(F("Daily logfile is "));
  DebugSerial.println(fileName);
	// check if this file exists
	// if yes, use it
	// if no, create it
	my_wellbeing = open_logfile(fileName);
	return my_wellbeing;
}


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	check_headerfile
 * Does:			(re)create headerfile (it is recreated at each startup)
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Serial:			errormessage / infomessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
int8_t check_headerfile(){

	uint8_t my_wellbeing = SDC_IS_ERROR; // default, reset at end iff successful

  char charbuffer[max(sizeof(CSV_HEADERFILE_CONTENT), sizeof(CSV_HEADERFILE_NAME))];
	
	// open or create file
	if (
		(open_logfile(strcpy_P(charbuffer,CSV_HEADERFILE_NAME)) == ALL_IS_WELL) && 
		(file.seekSet(0) == true) && 
		(file.write(strcpy_P(charbuffer,CSV_HEADERFILE_CONTENT)) != -1) && 
		(close_logfile() == ALL_IS_WELL)
	){
		my_wellbeing = ALL_IS_WELL;
	} else {
		my_wellbeing = SDC_IS_ERROR;
		sd.errorPrint(F("header file"));
	}
	return my_wellbeing;
}

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	update_statusfile
 * Does:			open (or create) system-status-file
 * 					append one new line with current DateTime from RTC and a message
 * Parameters:		an informative additional free-text message
 * 					some constants are used
 * Returns:			void
 * Serial:			errormessage / infomessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void update_statusfile(const char *msg){

	// get current datetime and make char-buffer from it
	rtcNow = rtc.now();
  char timebuffer[CLOCK_DATETIME_STRING_FORMAT_LENGTH]; 
  strcpy_P(timebuffer, CLOCK_DATETIME_STRING_FORMAT); 
  rtcNow.toString(timebuffer);

  char statusfile_name[CSV_STATUSFILE_NAME_LENGTH];
  
	// open or create filecharbuffer
	if (
		(open_logfile(strcpy_P(statusfile_name, CSV_STATUSFILE_NAME)) == ALL_IS_WELL) && 
		(file.seekEnd(0) == true) && 
		(file.write(timebuffer) != -1) &&
		(file.println(msg) != -1) &&
		(close_logfile() == ALL_IS_WELL)
	){
		;
	} else {
		sd.errorPrint(F("status file append one line"));
	}
}


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	open_logfile
 * Does:			opens a logfile for write access; if it does not exist, create it and open it
 * Parameters:		fileName
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Serial:			errormessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t open_logfile(char *fileName){
	if (!file.open(fileName, FILE_WRITE)) {
		sd.errorPrint(fileName);
		sd.errorPrint(F("file.open"));
		return SDC_IS_ERROR;
	} else {
		return ALL_IS_WELL;
	}
}



/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	sync_logfile
 * Does:			syncs an open logfile
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Serial:			errormessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t sync_logfile(){
	// Force data to SD and update the directory entry to avoid data loss.
	if (!file.sync() || file.getWriteError()) {
		sd.errorPrint(F("write/sync error"));
		return SDC_IS_ERROR;
	} else {
		return ALL_IS_WELL;
	}
}




/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	close_logfile
 * Does:			syncs an open logfile and closes it afterwards
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Serial:			errormessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t close_logfile(){
	// Force data to SD and update the directory entry to avoid data loss.
	if ( ! file.isOpen() ){
		return ALL_IS_WELL;
	}
	
	if ( (sync_logfile() != ALL_IS_WELL)  || !file.close() ) {
		sd.errorPrint(F("close error"));
		return SDC_IS_ERROR;
	} else {
		return ALL_IS_WELL;
	}
}


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	write_one_csv_line_to_sdc
 * Does:			take readings from sensors and write them as one csv line to logfile
 * Parameters:		none
 * 					some constants are used
 * 					some globals are used
 * Returns:			void
 * Serial:			errormessage
 * Blocking:		no
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void write_one_csv_line_to_sdc(){
	float temperature_median = dht_temp_samples.getMedian();
	float humidity_median = dht_humidity_samples.getMedian();
	float light_median = ldr_samples.getMedian();

	//macroDateTimeToCharBuffer(rtcNow,utc_now_datetime);
  char utc_now_datetime[CLOCK_DATETIME_STRING_FORMAT_LENGTH]; 
  strcpy_P(utc_now_datetime, CLOCK_DATETIME_STRING_FORMAT); 
  rtcNow.toString(utc_now_datetime);

  if ( is_phase_in && (rtcNow - startUpTime).minutes() > PHASE_IN_TIME ){
    // reset phase_in flag --> phase in is now completed until next startup
    is_phase_in = false;
  }
  	
  	file.print(utc_now_datetime);	file.write(CSV_SEPARATOR);
  	if (is_phase_in) {
  	  file.print(WARNING_PHASE_IN_AFTER_STARTUP); 
  	} else {
      file.print(system_wellbeing);
  	}
  	file.write(CSV_SEPARATOR);
  	file.print(temperature_median);	file.write(CSV_SEPARATOR);
  	file.print(humidity_median);	file.write(CSV_SEPARATOR);
  	file.print(light_median);		file.write('\n');

   sync_logfile();

  	DebugSerial.print(utc_now_datetime);	 	DebugSerial.print(CSV_SEPARATOR);
	  if (is_phase_in) {
      DebugSerial.print(WARNING_PHASE_IN_AFTER_STARTUP); 
    } else {
      DebugSerial.print(system_wellbeing);
    }
    DebugSerial.print(CSV_SEPARATOR);
  	DebugSerial.print(temperature_median);	DebugSerial.print(CSV_SEPARATOR);
  	DebugSerial.print(humidity_median);	DebugSerial.print(CSV_SEPARATOR);
  	DebugSerial.print(light_median);		DebugSerial.print('\n');
}




//EOF
