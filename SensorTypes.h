#ifndef SensorTypes_h
#define SensorTypes_h

#include <SimpleDHT.h>

//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Definition of reading functions for specific sensors.  															*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

float basicAnalogicReading(short int pin, short int numReadings)
{
	pinMode(pin,INPUT);
	/* When using Arduino Due, change resolution */
	#ifdef ARDUINO_DUE
		analogWriteResolution(12);
		analogReadResolution(12);
	#endif

	float rawValuesSum = 0;

	for (short int i = 0; i < numReadings; i++)
	{
		/* Read input and add it to the values sum */
		rawValuesSum += analogRead(pin);
	}

	/* Reset resolution, when using Arduino Due */
	#ifdef ARDUINO_DUE
		analogWriteResolution(10);
		analogReadResolution(10);
	#endif

	/* Return the average of the readings */
	return rawValuesSum / numReadings;

}

float basicDigitalReading(short int pin, short int numReadings)
{
	pinMode(pin,INPUT);

	return (float)digitalRead(pin);

}

float dhtHumidityReading(short int pin, short int numReadings)
{
	pinMode(pin,INPUT);
	float rawValuesSum = 0;
	byte data[40];
	float humidity2 = 0;
	float temperature2 = 0;
	SimpleDHT11 dht((int)pin);

	// for (short int i = 0; i < numReadings; i++)
	// {
	// 	/* Read input and add it to the values sum */
	// 	dht.read2(&temperature,&humidity,data);
	// 	rawValuesSum += humidity;
	// }
	dht.read2(&temperature2,&humidity2,NULL);
	/* Return the average of the readings */
	// return rawValuesSum / numReadings;
	return humidity2;

}


float dhtTemperatureReading(short int pin, short int numReadings)
{

	pinMode(pin,INPUT);
	float rawValuesSum = 0;
	byte data[40];
	float humidity = 0;
	float temperature = 0;
	SimpleDHT11 dht((int)pin);

	// for (short int i = 0; i < numReadings; i++)
	// {
	// 	/* Read input and add it to the values sum */
	// 	dht.read2(&temperature,&humidity,data);
	// 	rawValuesSum += temperature;
	// }
	dht.read2(&temperature,&humidity,data);
	/* Return the average of the readings */
	// return rawValuesSum / numReadings;
	return temperature;

}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/*																																																								*/
	/* Definition of sensor_params for specific sensors.  																														*/
	/*																																																								*/
	/* Values describe, respectively: 																																								*/
	/* slope (float)																																																	*/
	/* intercept (float)																																															*/
	/* readDelay (milliseconds between readings for average, in a single reading, integer)														*/
	/* numReadings (total readings to privide average, in a single reading, integer)																	*/
	/* numCalibrationPoints (amoiunt of calibration points, integer)                																	*/
	/* calibrationPoints (reference calibration points, minimum two, array of floats)																	*/
	/* name (string containing the name of the sensor type, array of chars)																						*/
	/* measure_unit (string containing a measure unit symbol, array of chars)																					*/
	/* decimals (presentation decimals numbers, short int)																														*/
	/* readingFunction (function to be used to read, float (*readingFunction)(short int pin, short int numReadings))	*/
	/*																																																								*/
	/* TODO: set correct defaults for all sensor types																																*/
	/*																																																								*/
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// sensor_params anemometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Anemometer", "m/s", &basicReading };
	sensor_params ec_meter_params = { 1, 0, 100, 10, 2, {400.0, 2000.0}, "EC meter", "uS/cm",0, &basicAnalogicReading };						// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params hygrometer_params = { 1, 0, 100, 10, 2, {4.0, 7.0}, "Hygrometer", "%",1, &dhtHumidityReading };
	sensor_params light_sensor_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "Light intensity", "xx",0, &basicAnalogicReading };
	sensor_params red_light_sensor_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "Red light intensity", "xx",0, &basicAnalogicReading };
	sensor_params blue_light_sensor_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "Blue light intensity", "xx",0, &basicAnalogicReading };
	sensor_params ultraviolet_light_sensor_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "Ultraviolet light intensity", "xx",0, &basicAnalogicReading };
	// sensor_params lux_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Lux meter", "pH", &basicReading };
	// sensor_params ph_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "PH meter", "pH", &basicReading };
	sensor_params soil_moisture_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil moisture meter", "%",2, &basicAnalogicReading };
	sensor_params analogic_thermometer_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "Thermometer", "C",1, &basicAnalogicReading  };		// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params air_thermometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Air thermometer", "C",1, &dhtTemperatureReading  };		// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params co_sensor_params = { 1, 0, 100, 10, 2, {0.0, 100.0}, "CO sensor", "ppm",0, &basicAnalogicReading  };		// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params nox_sensor_params = { 1, 0, 100, 10, 2, {4.0, 7.0}, "NOx sensor", "ppm",0, &basicAnalogicReading  };		// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params etoh_sensor_params = { 1, 0, 100, 10, 2, {4.0, 7.0}, "EtOH sensor", "ppm",0, &basicAnalogicReading  };		// TODO: use utf-8 characters for maths and chemistry symbols
	// sensor_params thermometer_soil_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil thermometer", "C", &basicReading };  // TODO: use utf-8 characters for maths and chemistry symbols
	// sensor_params thermometer_water_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Water thermometer", "C", &basicReading };// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params volume_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Volume meter", "Db",1, &basicAnalogicReading };

#endif
