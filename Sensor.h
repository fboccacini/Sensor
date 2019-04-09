#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"

#define MAX_LAST_READINGS 20

#define HYGROMETER 			0x01
#define AIR_THERMOMETER 0x02

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
		float (*readingFunction)(short int pin, short int numReadings);

	};


class Sensor
{
	public:
		/* Constructor, takes connected pin, params of the correct sensor type, and an optional name for display */
		Sensor(short int inputPin,short int sensorType, const char label[]);

		///////////////////////////////////////////////////////////////////////////////////////////
		/*                                                                                       */
		/* Operational methods																																	 */
		/*                                                                                       */
		///////////////////////////////////////////////////////////////////////////////////////////

		/* collectInput: does one reading and returns a transformed value. It also pushes it in lastReadings */
		float collectInput();

		/* collectRawInput: does a reading and returns a raw value */
		float collectRawInput();

		/* calibrate: loops through all the calibration points and reads values to calibrate.		*/
		/* Then it resets slope and intercept of the sensor.																		*/
		void calibrate();

		/* setValues: resets calibration to specified values, if intercept and slope are not given, they will not be modified */
		void setValues(float calibrationPoints[], float intercept, float slope);

		/* convertInputLinear: transforms a raw value to one in the correct measure unit */
		float convertInputLinear(float input);


		/* printReading: prints formatted reading on specified printChannels. Null forall channels. */
		String printReading();

		//////////////////////////////////////////////////////////////////////////////////////////
		/*																																											*/
		/* Getters																																							*/
		/*																																											*/
		//////////////////////////////////////////////////////////////////////////////////////////

		short int getPin();
		float getIntercept();
		float getSlope();
		const char* getLabel();
		const char* getMeasureUnit();
		const char* getSensorType();
		short int getNumRedings();
		float* getCalibrationPoints();
		float* getLastReadings();
		float (*readingFunction)(short int pin, short int numReadings);

		short int numReadings;
		short int pin;							// the pin the sensor is connected to
	private:
		float _intercept;						// TODO: complete comments
		float _slope;								// TODO: complete comments
		float* _calibrationPoints;	// TODO: complete comments
		float _lastReadings[MAX_LAST_READINGS];			// Last taken readings of the sensor
		char* _label;							// Label, for the display
		sensor_params _sensorType;	// Sensor type
		int _readDelay;							// Delay between readings

		/* Add to the redings history. If the buffer is full rotate */
		void pushLastReadings(float value);
};


#endif
