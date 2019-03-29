#include "Sensor.h"


Sensor::Sensor(short int inputPin, sensor_params params, char label[] = "", ControlChannel* startingControlChannels[] = {}, PrintChannel* startingPrintChannels[] = {},float (*readFunction)())
{
  /* Set configuration */
  short int pin = inputPin;
  sensor_params sensorType = params;

  if(sizeof(label) == 0)
  {
    char* label = this->sensorType.name;
  }
  else
  {
    char* label = label;
  }


  /* Set default calibration */
  float* calibrationPoints = this->sensorType.calibrationPoints;
  float intercept = this->sensorType.intercept;
  float slope = this->sensorType.slope;
  short int numReadings = this->sensorType.numReadings;
  short int readDelay = this->sensorType.readDelay;
  float (*collectRawInput)(short int pin, short int numReadings) = this->sensorType.readingFunction(pin,numReadings);

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

// float Sensor::collectRawInput()
// {
//   /* When using Arduino Due, change resolution */
//   #ifdef ARDUINO_DUE
//     analogWriteResolution(12);
//     analogReadResolution(12);
//   #endif
//
//   float rawValuesSum = 0;
//
//   for (short int i = 0; i < this->numReadings; i++)
//   {
//     /* Read input and add it to the values sum */
//     rawValuesSum += analogRead(this->pin);
//   }
//
//   /* Reset resolution, when using Arduino Due */
//   #ifdef ARDUINO_DUE
//     analogWriteResolution(10);
//     analogReadResolution(10);
//   #endif
//
//   /* Return the average of the readings */
//   return rawValuesSum / numReadings;
//
//   }
// }

void Sensor::printReading(PrintChannels* channels = NULL)
{
  /* Prepare message */
  char message[50];
  sprintf(message,"%s: %f.3%s",label,collectInput(),get_measure_unit());

  /* If print channel is null printAll or else cycle through the channels */
  if(channels == NULL)
  {
    printAll(message);
  }
  else
  {
    for(short int i = 0; i < sizeof(channels); i++)
    {
      this->printChannels[i].print(message);
    }
  }
}

void Sensor::calibrate()
{

}

/* setValues: resets calibration to specified values, if intercept and slope are not given, they will not be modified */
void Sensor::setValues(float calibrationsPoints[], float intercept = NULL, float slope = NULL)
{
  this->calibrationPoints = calibrationPoints;

  if(intercept)
  {
    this->intercept = intercept;
  }
  if(slope)
  {
    this->slope = slope;
  }
}

float Sensor::convertInputLinear(float inputRawValue)
{
  return this->slope * inputRawValue + this->intercept;
}

//////////////////////////////////////////////////////////////////////////////////////////
/*																																											*/
/* Interaction channels setup	and management																						*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

void addPrintChannel(PrintChannel* printChannel)
{
  this->printChannels.push(printChannel);
}

void removePrintChannel(short int index)
{

}

void addControlChannel(ControlChannel* controlChannel)
{
  this->controlChannels.push(controlChannel);
}

void removeControlChannel(short int index)
{

}

/* printAll: prints a message on all print channels */
void printAll(char* message)
{
  for(short int i = 0; i < sizeof(this->printChannels); i++)
  {
    this->printChannels[i].print(message);
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
  return this->calibrationPoints
}

float Sensor::getIntercept()
{
  return this->intercept;
}

char* Sensor::getLabel()
{
  return this->label;
}

float* Sensor::getLastReadings()
{
  return this->lastReadings;
}

short int Sensor::getPin()
{
  return this->pin;
}

float Sensor::getSlope()
{
  return this->slope;
}

char* getMeasureUnit()
{
  return this->sensorType.measureUnit;
}
/* Readings history rotation */
void Sensor::pushLastReadings(float value)
{
  /* Record the value in last readings. */
  this->lastReadings.push(value);

  /* If the buffer is full, drop the oldest value */
  if(sizeof(this->lastReadings) > MAX_LAST_READINGS){
    this->lastReadings.pop();
  }
}

ECmeter::ECmeter(int pin, int onOffPin, ControlChannel controlChannels[], PrintChannel printChannels[], char name[] = [])
{
  /* Set configuration */
  int _pin = pin;
  int _onOffPin = onOffPin;
  float _sensorType = ec_meter_params;
  float _name = name;

  /* Set default calibration */
  float _calibrationPoints[] = _sensorType.calibrationPoints
  float _intercept = _sensorType.intercept
  float _slope = _sensorType.slope

  /* Set interaction channels */
  ControlChannel controlChannels[] = controlChannels;
  PrintChannel printChannels[] = printChannels;

}

ECmeter::collectRawInput()
{
  #ifdef ARDUINO_DUE
    analogWriteResolution(12);
    analogReadResolution(12);
  #endif

  /* Activate sensor */
  digitalWrite(this->onOffPin, HIGH);
  delay(100);

  float inputRawValue;
  int numReadings = getNumReadings();

  for (int i = 0; int i < numReadings; i++)
  {
    inputRawValue += analogRead(this->pin);
  }

  /* Deactivate sensor */
  digitalWrite(this->onOffPin, LOW);

  #ifdef ARDUINO_DUE
    analogWriteResolution(10);
    analogReadResolution(10);
  #endif

  /* Return the average of the readings */
  return inputRawValue / numReadings;
}
