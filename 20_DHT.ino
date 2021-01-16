/*
 * ###############################################################################################
 * ###############################################################################################
 * Functions around DHT sensor for temp/humidity measurement
 * ###############################################################################################
 * ###############################################################################################
 */

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	init_dht_subsystem
 * Does:			setup dht sensor 
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		no
 * Minimum time:	n/a
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void init_dht_subsystem(){
	// nothin to do, really
}

/* 
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 * Function:	 	get_dht_measure
 * Does:			take one dht measure of time and humidity each, then put them to ring-buffers for later median                                        
 * Parameters:		none
 * 					some constants are used
 * Returns:			nothing/void 
 * Blocking:		no
 * Minimum time:	n/a (tbd?)
 * :::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
 */
void get_dht_measures(){
	int dht_measurement = DHT.read22(IN_DHT_PIN);
	switch (dht_measurement)
    {
    case DHTLIB_OK:
        break;
    case DHTLIB_ERROR_CHECKSUM:
        DebugSerial.println(F("error: DHT22 checksum"));
        break;
    case DHTLIB_ERROR_TIMEOUT:
        DebugSerial.println(F("error: DHT22 time out"));
        break;
    case DHTLIB_ERROR_CONNECT:
        DebugSerial.println(F("error: DHT22 Connect"));
        break;
    case DHTLIB_ERROR_ACK_L:
        DebugSerial.println(F("error: DHT22 acknowledge low"));
        break;
    case DHTLIB_ERROR_ACK_H:
        DebugSerial.println(F("error: DHT22 acknowledge high"));
        break;
    default:
        DebugSerial.println(F("error: DHT22 unknown error"));
        break;
    }

    if (dht_measurement == DHTLIB_OK){
    	// only do rest, if measurement was successful
  		DebugSerial.print(F("New DHT22 value: "));
  		DebugSerial.print(DHT.temperature);
  		DebugSerial.print(F(" °C at "));
  		DebugSerial.print(DHT.humidity);
  		DebugSerial.println(F(" % humidity."));
  		dht_temp_samples.add(DHT.temperature);
  		dht_humidity_samples.add(DHT.humidity);
    }
}


//EOF
