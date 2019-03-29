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
		float (*readingFunction)(short int pin, short int numReadings);

	};


class Sensor
{
	public:
		/* Constructor, takes connected pin, params of the correct sensor type, and an optional name for display */
		Sensor(short int inputPin,sensor_params params, const char label[], ControlChannel* controlChannels[], PrintChannel* printChannels[]);

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
		void setValues(float calibrationPoints[], float intercept = NULL, float slope = NULL);

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
		void printAll(const char* messsage);

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


#endif
