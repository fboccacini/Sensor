#include "Sensor.h"


Sensor::Sensor(short int inputPin, sensor_params params, const char label[] = "", ControlChannel* startingControlChannels[] = {}, PrintChannel* startingPrintChannels[] = {})
{
  /* Set configuration */
  short int pin = inputPin;
  sensor_params sensorType = params;

  if(sizeof(label) == 0)
  {
    const char* _label = this->_sensorType.name;
  }
  else
  {
    const char* _label = label;
  }
  

  /* Set default calibration */
  float* calibrationPoints = this->_sensorType.calibrationPoints;
  float intercept = this->_sensorType.intercept;
  float slope = this->_sensorType.slope;
  short int numReadings = this->_sensorType.numReadings;
  short int readDelay = this->_sensorType.readDelay;


  /* Set interaction channels */
  ControlChannel** controlChannels = startingControlChannels;
  PrintChannel** printChannels = startingPrintChannels;

}

///////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                       */
/* Operational methods																																	 */
/*                                                                                       */
///////////////////////////////////////////////////////////////////////////////////////////

float Sensor::collectInput()
{
  /* Do the reading and onvert the result to a useful form, using sensor type properties */
  float value = convertInputLinear(collectRawInput());

  /* Record the value in the readings history. */
  pushLastReadings(value);

  return value;
}

float Sensor::collectRawInput()
{
  return this->_sensorType.readingFunction(this->pin,this->numReadings);
}

void Sensor::printReading(PrintChannel* channels = NULL)
{
	
  /* Prepare message */
  char message[50];
  //sprintf(message,"%s: %.3f%s",this->_label,this->collectInput(),this->getMeasureUnit());
  sprintf(message,"%s: %.3f%s",_label,this->collectRawInput(),getMeasureUnit());
	Serial.println("test");
	Serial.println(message);
  /* If print channel is null printAll or else cycle through the channels */
  if(channels == NULL)
  {
   // printAll(message);
  }
  else
  {
    for(short int i = 0; i < sizeof(channels); i++)
    {
     // this->printChannels[i]->print(message);
    }
  }
}

void Sensor::calibrate()
{

}

/* setValues: resets calibration to specified values, if intercept and slope are not given, they will not be modified */
void Sensor::setValues(float calibrationPoints[], float intercept, float slope)
{
  this->_calibrationPoints = calibrationPoints;

  if(intercept)
  {
    this->_intercept = intercept;
  }
  if(slope)
  {
    this->_slope = slope;
  }
}

float Sensor::convertInputLinear(float inputRawValue)
{
  return this->_slope * inputRawValue + this->_intercept;
}

//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Interaction channels setup	and management																						*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

void Sensor::addPrintChannel(PrintChannel* printChannel)
{
  //this->printChannels++ = printChannel;
}

void Sensor::removePrintChannel(short int index)
{

}

void Sensor::addControlChannel(ControlChannel* controlChannel)
{
  //this->controlChannels++ = controlChannel;
}

void Sensor::removeControlChannel(short int index)
{

}

/* printAll: prints a message on all print channels */
void Sensor::printAll(const char* message)
{
  for(short int i = 0; i < sizeof(this->printChannels); i++)
  {
    this->printChannels[i]->print(message);
  }
}

/* readCharAnyControl: reads on all control channels and returns the input 							*/
/* of the first one to respond.																													*/
char readCharAnyControl()
{

}

/* readSentenceAnyControl: reads on all control channels and returns the 							  */
/* input sentence (read chars until a accept char is given) of the first one to respond.*/
char* readSentenceAnyControl()
{

}

//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Getters																																							*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

float* Sensor::getCalibrationPoints()
{
  return this->_calibrationPoints;
}

float Sensor::getIntercept()
{
  return this->_intercept;
}

const char* Sensor::getLabel()
{
  return this->_label;
}

float* Sensor::getLastReadings()
{
  return this->_lastReadings;
}

short int Sensor::getPin()
{
  return this->pin;
}

float Sensor::getSlope()
{
  return this->_slope;
}

const char* Sensor::getMeasureUnit()
{
  return this->_sensorType.measureUnit;
}
/* Readings history rotation */
void Sensor::pushLastReadings(float value)
{
  /* Record the value in last readings. */
  //this->_lastReadings->push(value);

  /* If the buffer is full, drop the oldest value */
  if(sizeof(this->_lastReadings) > MAX_LAST_READINGS){
    //this->_lastReadings->pop();
  }
}
