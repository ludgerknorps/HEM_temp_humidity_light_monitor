/*
 * ###############################################################################################
 * ###############################################################################################
 * Functions around Serial subsystem for logging / debugging
 * ###############################################################################################
 * ###############################################################################################
 */

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	init_serial_subsystem
 * Does:			setup standard arduino serial port for read and write use
 * Parameters:		none
 * 					some constants are used
 * Returns:			errorcode/wellbeingcode
 * Blocking:		yes, if serial port does not come online/get ready; goes into endless loop
 * Minimum time:	200ms
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
uint8_t init_serial_subsystem(){

	uint8_t my_wellbeing = SERIAL_IS_ERROR; // default, reset at end iff successful
	
	// start serial subsystem
	DebugSerial.begin(SERIAL_SPEED_BAUD);
	
	// Wait for USB Serial 
	while (!Serial) {
		; // this is potentially blocking!
	}
	delay(400);

  DebugSerial.println(F("Start"));
  DebugSerial.println(F("Info on complied file and date/time..."));
  //DebugSerial.println(COMPILEFILE);
  DebugSerial.println((__FlashStringHelper*)(COMPILEDATE));
  DebugSerial.println((__FlashStringHelper*)(COMPILETIME));
  

	my_wellbeing = ALL_IS_WELL;
	return my_wellbeing;
	
}




//EOF
