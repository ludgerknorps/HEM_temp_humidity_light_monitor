/*
 * ###############################################################################################
 * ###############################################################################################
 * LOOP
 * ###############################################################################################
 * ###############################################################################################
 */

 
/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	standard arduino loop function
 * Does:			loops around the clock
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		not by itself, maybe called subroutines
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void loop(){
	

	// get current datetime and make char-buffer from it
	rtcNow = rtc.now();	

	// every 12 seconds get one light measure froom LDR
	// every 12 seconds get one temperature and humidity measure each from DHT subsystem
	// every 60 seconds get medians of last 5 measures each and write them to SDC

	if ( rtcNow.second() % 12 == 0 ) {
		print_rtcNow_to_serial();
		get_ldr_measure();
		get_dht_measures();
	}

	if ( rtcNow.second() == 0 ) {
    // if it is 00:00:00 before this first write action of the day, close current daily logfile (which belongs to yesterday) and open new one
    if ( rtcNow.hour() == 0 &&  rtcNow.minute() == 0 ){
      system_wellbeing = touch_or_open_daily_logfile();
    }
		
	write_one_csv_line_to_sdc();
    dht_temp_samples.clear();
    dht_humidity_samples.clear();
    ldr_samples.clear();
    
	}

  if ( rtcNow.second() % 12 == 0 ) {
    delay(1100); // slow down, otherwise loop is way to fast
  }
  
}




//EOF
