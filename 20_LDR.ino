/*#
 * wq#w
 * wsköä.aws#
 -d* 
 * w
 * 
 * 
 * ###############################################################################################
 * ###############################################################################################
 * Functions around LDR sensor for light measurement
 * ###############################################################################################
 * ###############################################################################################
 */

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	init_ldr_subsystem
 * Does:			setup ldr sensor 
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		yes, xxx
 * Minimum time:	xxx
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void init_ldr_subsystem(){
	// nothing to do, really
}


/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	get_ldr_measure
 * Does:			take an ldr measure, then put it to ring-buffer for later median                                        
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		no
 * Minimum time:	n/a (tbd?)
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void get_ldr_measure(){
	float sensorValue = analogRead(IN_LDR_PIN);
	DebugSerial.print(F("New LDR value: "));
	DebugSerial.println(sensorValue);
	ldr_samples.add(sensorValue);
}


//EOF
