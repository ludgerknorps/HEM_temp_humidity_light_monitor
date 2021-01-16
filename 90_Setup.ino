/*
 * ###############################################################################################
 * ###############################################################################################
 * SETUP
 * ###############################################################################################
 * ###############################################################################################
 */


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	standard arduino setup function
 * Does:			setup
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		not by itself, maybe called subroutines
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void setup() {

	system_wellbeing = ALL_IS_WELL;

	is_phase_in = true;

	// setup Main incl. Clock
	init_serial_subsystem();
	init_clock_subsystem(); // this sets global rtcNow datetime for first time
	init_sdcard_subsystem();
	// if success: setup dht
	init_dht_subsystem();
	// if success: setup ldr
	init_ldr_subsystem();
	DebugSerial.println(F("System is set up; all subsystems initialized."));
	
}


//EOF
