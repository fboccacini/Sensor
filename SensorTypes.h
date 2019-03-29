//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Definition of reading functions for specific sensors.  															*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

float basicReading(short int pin, short int numReadings)
{
	/* When using Arduino Due, change resolution */
	// #ifdef ARDUINO_DUE
	// 	analogWriteResolution(12);
	// 	analogReadResolution(12);
	// #endif

	// float rawValuesSum = 0;
	//
	// for (short int i = 0; i < numReadings; i++)
	// {
	// 	/* Read input and add it to the values sum */
	// 	rawValuesSum += analogRead(pin);
	// }

	/* Reset resolution, when using Arduino Due */
	// #ifdef ARDUINO_DUE
	// 	analogWriteResolution(10);
	// 	analogReadResolution(10);
	// #endif

	/* Return the average of the readings */
	// return rawValuesSum / numReadings;
	return 0.5;

}


	//////////////////////////////////////////////////////////////////////////////////////////
	/*																																											*/
	/* Definition of sensor_params for specific sensors.  																	*/
	/*																																											*/
	/* Values describe, respectively: 																											*/
	/* slope (float)																																				*/
	/* intercept (float)																																		*/
	/* readDelay (milliseconds between readings for average, in a single reading, integer)	*/
	/* numReadings (total readings to privide average, in a single reading, integer)				*/
	/* numCalibrationPoints (amoiunt of calibration points, integer)                				*/
	/* calibrationPoints (reference calibration points, minimum two, array of floats)				*/
	/* name (string containing the name of the sensor type, array of chars)									*/
	/* measure_unit (string containing a measure unit symbol, array of chars)								*/
	/*																																											*/
	/* TODO: set correct defaults for all sensor types																			*/
	/*																																											*/
	//////////////////////////////////////////////////////////////////////////////////////////
	// sensor_params anemometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Anemometer", "m/s", &basicReading };
	// sensor_params ec_meter_params = { 1.77, 458.71, 100, 10, 2, {400.0, 2000.0}, "EC meter", "uS/cm", &basicReading };						// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params hygrometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Hygrometer", "pH", &basicReading };
	// sensor_params lux_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Lux meter", "pH", &basicReading };
	// sensor_params ph_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "PH meter", "pH", &basicReading };
	// sensor_params soil_moisture_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil moisture meter", "pH", &basicReading };
	sensor_params thermometer_air_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Air thermometer", "C", &basicReading };		// TODO: use utf-8 characters for maths and chemistry symbols
	// sensor_params thermometer_soil_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil thermometer", "C", &basicReading };  // TODO: use utf-8 characters for maths and chemistry symbols
	// sensor_params thermometer_water_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Water thermometer", "C", &basicReading };// TODO: use utf-8 characters for maths and chemistry symbols
	// sensor_params volume_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Volume meter", "Db", &basicReading };
