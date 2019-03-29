#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "InteractionChannel.h"


#define MAX_LAST_READINGS 20


/* This struct defines the params a sensor needs to translate raw signal into a known measure unit */
typedef struct sensor_params {

		float slope;
		float intercept;
		short int readDelay;
		short int numReadings;
		short int numCalibrationPoints;
		float calibrationPoints[10];
		const char* name;
		const char* measureUnit;
		float (*readingFunction)();

	};


class Sensor
{
	public:
		/* Constructor, takes connected pin, params of the correct sensor type, and an optional name for display */
		Sensor(short int inputPin,sensor_params params, char label[], ControlChannel* controlChannels[], PrintChannel* printChannels[]);

		///////////////////////////////////////////////////////////////////////////////////////////
		/*                                                                                       */
		/* Operational methods																																	 */
		/*                                                                                       */
		///////////////////////////////////////////////////////////////////////////////////////////

		/* collectInput: does one reading and returns a transformed value. It also pushes it in lastReadings */
		float collectInput();

		/* calibrate: loops through all the calibration points and reads values to calibrate.		*/
		/* Then it resets slope and intercept of the sensor.																		*/
		void calibrate();

		/* setValues: resets calibration to specified values, if intercept and slope are not given, they will not be modified */
		void setValues(float calibrationPoints[], float intercept = NULL, float slope = NULL);

		/* collectRawInput: does a reading and returns a raw value */
		float (*collectRawInput)();

		/* convertInputLinear: transforms a raw value to one in the correct measure unit */
		float convertInputLinear(float input);

		//////////////////////////////////////////////////////////////////////////////////////////
		/*																																											*/
		/* Interaction channels setup	and management																						*/
		/*																																											*/
		//////////////////////////////////////////////////////////////////////////////////////////

		void addPrintChannel(PrintChannel* printChannel);
		void removePrintChannel(short int index);
		void addControlChannel(ControlChannel* controlChannel);
		void removeControlChannel(short int index);

		/* printReading: prints formatted reading on specified printChannels. Null forall channels. */
		void printReading(PrintChannel* channels);

		/* printAll: prints a message on all print channels */
		void printAll(char* messsage);

		/* readCharAnyControl: reads on all control channels and returns the input 							*/
		/* of the first one to respond.																													*/
		char readCharAnyControl();

		/* readSingleIntAnyControl: like readCharAnyControl but it returns an integer						*/
		short int readSingleIntAnyControl();

		/* readSentenceAnyControl: reads on all control channels and returns the 							  */
		/* input sentence (read chars until a accept char is given) of the first one to respond.*/
		char* readSentenceAnyControl();

		/* readIntAnyControl: like readSentenceAnyControl, but it converts the output to an integer */
    int readIntAnyControl();

		/* readFloatAnyControl: same again, but it converts to float. 													*/
		/* The dot button marks the decimal point 																							*/
    float readFloatAnyControl();

		//////////////////////////////////////////////////////////////////////////////////////////
		/*																																											*/
		/* Getters																																							*/
		/*																																											*/
		//////////////////////////////////////////////////////////////////////////////////////////

		short int getPin();
		float getIntercept();
		float getSlope();
		char* getLabel();
		char* getMeasureUnit();
		char* getSensorType();
		short int getNumRedings();
		float* getCalibrationPoints();
		float* getLastReadings();


		short int numReadings;
		short int pin;							// the pin the sensor is connected to
	private:
		float _intercept;						// TODO: complete comments
		float _slope;								// TODO: complete comments
		float* _calibrationPoints;	// TODO: complete comments
		float _lastReadings[MAX_LAST_READINGS];			// Last taken readings of the sensor
		char* _label;							// Label, for the display
		sensor_params _sensorType;	// Sensor type

		/* Interaction channels */
	  ControlChannel** controlChannels;
	  PrintChannel** printChannels;

		char* currentChar;
		char** currentSentence;

		/* Add to the redings history. If the buffer is full rotate */
		void pushLastReadings(float value);
};

class ECmeter: public Sensor
{
	public:
		ECmeter(short int pin, short int onOffPin, ControlChannel controlChannels[], PrintChannel printChannels[], char name[]);
		float collectRawInput();

	private:
		short int _onOffPin;
};

//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Definition of reading functions for specific sensors.  															*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

float basicReading(short int pin, short int numReadings)
{
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
	sensor_params anemometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Anemometer", "m/s", &basicReading };
	sensor_params ec_meter_params = { 1.77, 458.71, 100, 10, 2, {400.0, 2000.0}, "EC meter", "uS/cm", &basicReading };						// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params hygrometer_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Hygrometer", "pH", &basicReading };
	sensor_params lux_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Lux meter", "pH", &basicReading };
	sensor_params ph_meter_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "PH meter", "pH", &basicReading };
	sensor_params soil_moisture_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil moisture meter", "pH", &basicReading };
	sensor_params thermometer_air_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Air thermometer", "C", &basicReading };		// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params thermometer_soil_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Soil thermometer", "C", &basicReading };  // TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params thermometer_water_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Water thermometer", "C", &basicReading };// TODO: use utf-8 characters for maths and chemistry symbols
	sensor_params volume_params = { -0.0273, 19.655, 100, 10, 2, {4.0, 7.0}, "Volume meter", "Db", &basicReading };

#endif
