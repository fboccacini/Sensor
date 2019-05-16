#ifndef Sensor_h
#define Sensor_h

#include "Arduino.h"
#include "SoftwareSerial.h"

#define MAX_LAST_READINGS 20
#define MAX_IO_STREAMS		10

#define CUSTOM					0x00
#define HYGROMETER 			0x01
#define AIR_THERMOMETER 0x02
#define CO_SENSOR				0x03
#define NOX_SENSOR			0x04
#define ETOH_SENSOR			0x05
#define VOLUME_SENSOR		0x06
#define LIGHT_SENSOR		0x07
#define RED_LIGHT_SENSOR		0x08
#define BLUE_LIGHT_SENSOR		0x09
#define ULTRAVIOLET_LIGHT_SENSOR		0x0A
#define ANALOGIC_THERMOMETER				0x0B
#define SOIL_MOISTURE_METER					0x0C

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
		short int decimals;
		float (*readingFunction)(short int pin, short int numReadings);

	};

class CustomSerial {

private:
  Stream &stream;

public:

  CustomSerial(Stream &_stream) : stream(_stream) { }

	CustomSerial(Stream &_stream,char (*getKey)()) : stream(_stream) {
		this->readFunction = getKey;
	}

	CustomSerial(Stream &_stream,char (*getKey)(),void (*writeChar)(String s)) : stream(_stream) {
		this->readFunction = getKey;
		this->printFunction = writeChar;
	}

	char (*readFunction)() = NULL;
	void (*printFunction)(String s) = NULL;

  char read() {

		if(this->readFunction == NULL)
		{
			return stream.read();
		} else {
			return this->readFunction();
		}

  }

  int available() {
    return stream.available();
  }

  int peek() {
    return stream.peek();
  }

  void write(byte b) {
    stream.write(b);
  }

  void println(String string) {

		if(this->printFunction == NULL)
		{
			stream.println(string);
		} else {
			this->printFunction(string);
			// int l = string.length();
			// Serial.println(string);
			// Serial.println(l);
			// for(int i = 0; i < l; i++)
			// {
			// 	Serial.println(i);
			// 	this->printFunction(string.charAt(i));
			//
			// }
		}

  }

	void println(int n) {
    stream.println(n);
  }

	void println(char c) {

		if(this->printFunction == NULL)
		{
			stream.print(c);
		} else {
			this->printFunction(String(c));
		}

  }

  void print(String string) {

		if(this->printFunction == NULL)
		{
			stream.print(string);
		} else {
			this->printFunction(string);
		}

  }

	void print(char c) {

		if(this->printFunction == NULL)
		{
			stream.print(c);
		} else {
			this->printFunction(String(c));
		}

  }

};

class Sensor
{
	public:
		/* Constructor, takes connected pin, params of the correct sensor type, and an optional name for display */
		Sensor(short int inputPin,short int sensorType, const char label[]);
		Sensor(short int inputPin, sensor_params sensorType, const char label[]);

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
		void setValues(float calibrationPoints[10], float intercept, float slope);

		/* convertInputLinear: transforms a raw value to one in the correct measure unit */
		float convertInputLinear(float input);

		/* formattedReading: returns a formatted string with sensor's name, reading and measure unit */
		String formattedReading();

		/* printReading: prints formatted reading on specified printChannels. Null forall channels. */
		void printReading(int stream);

		/* printAll: prints formatted reading on all printChannels. */
		void printAll();

		//////////////////////////////////////////////////////////////////////////////////////////
		/*																																											*/
		/* I/O management																																				*/
		/*																																											*/
		//////////////////////////////////////////////////////////////////////////////////////////

		/* Add an I/O stream to the sensor */
		int streamAdd(Stream &stream);
		int streamAdd(Stream &stream,char (*getKey)(),void (*writeChar)(String s));

		/* Test a stream (prints input to the output of an I/O stream) */
		void streamTest(int stream);

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
		void (*printingFunction)(char* message);
		void (*controlFunction)(char* message);

		short int numReadings;
		short int pin;							// the pin the sensor is connected to
	private:
		float _intercept;						// TODO: complete comments
		float _slope;								// TODO: complete comments
		float _calibrationPoints[10];	// TODO: complete comments
		short int _numCalibrationPoints;
		float _lastReadings[MAX_LAST_READINGS];			// Last taken readings of the sensor
		const char* _label;							// Label, for the display
		sensor_params _sensorType;	// Sensor type
		int _readDelay;							// Delay between readings
		CustomSerial* streams[MAX_IO_STREAMS];
		// CustomSerial* defaultStream;
		/* Add to the redings history. If the buffer is full rotate */
		void pushLastReadings(float value);
};


#endif
