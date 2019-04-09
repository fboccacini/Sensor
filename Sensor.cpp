#include "Sensor.h"
#include "Arduino.h"
#include "SensorTypes.h"

Sensor::Sensor(short int inputPin, short int sensorType, const char* label = "")
{
  /* Set configuration */
  this->pin = inputPin;
  switch(sensorType)
  {
    case HYGROMETER:
      this->_sensorType = hygrometer_params;
      break;
    case AIR_THERMOMETER:
      this->_sensorType = thermometer_air_params;
      break;
    default:
      this->_sensorType = ec_meter_params;
  }

  this->readingFunction = this->_sensorType.readingFunction;

  if(sizeof(label) == 0)
  {
    this->_label = this->_sensorType.name;
  }
  else
  {
    this->_label = label;
  }


  /* Set default calibration */
  this->_calibrationPoints = this->_sensorType.calibrationPoints;
  this->_intercept = this->_sensorType.intercept;
  this->_slope = this->_sensorType.slope;
  this->numReadings = this->_sensorType.numReadings;
  this->_readDelay = this->_sensorType.readDelay;

}

///////////////////////////////////////////////////////////////////////////////////////////
/*                                                                                       */
/* Operational methods																																	 */
/*                                                                                       */
///////////////////////////////////////////////////////////////////////////////////////////

float Sensor::collectInput()
{
  /* Do the reading and onvert the result to a useful form, using sensor type properties */
  float val = this->collectRawInput();
  float value = this->convertInputLinear(val);

  /* Record the value in the readings history. */
  //pushLastReadings(value);

  return value;
}

float Sensor::collectRawInput()
{
  float val = this->readingFunction(this->pin,this->numReadings);
  return val;
  // return basicReading(this->pin,this->numReadings);
  // return this->_sensorType.readingFunction(this->pin,this->numReadings);
  // return basicReading(this->pin,this->numReadings);
}

String Sensor::printReading()
{

  /* Prepare message */
  char* message = malloc(20 * sizeof(char));

  char* mu = this->getMeasureUnit();
  char* label = this->_label;
  float val = this->collectRawInput();

  // Arduino inplementation does not include %f, therefore transform the value to string
  char str_val[7];
  dtostrf(val, 4, 3, str_val);

  sprintf(message,"%s: %s%s",label,str_val,mu);

  // Convert it to String, otherwise the pointer will be discarded
  return (String)message;

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
