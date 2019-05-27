// #define ARDUINO_DUE 0x01
#include "Arduino.h"
#include "Sensor.h"
#include "SensorTypes.h"

Sensor::Sensor(short int inputPin, short int sensorType, const char* label)
{
  /* Set configuration */
  this->pin = inputPin;
  switch(sensorType)
  {
    case HYGROMETER:
      this->_sensorType = hygrometer_params;
      break;
    case AIR_THERMOMETER:
      this->_sensorType = air_thermometer_params;
      break;
    case CO_SENSOR:
      this->_sensorType = co_sensor_params;
      break;
    case ETOH_SENSOR:
      this->_sensorType = etoh_sensor_params;
      break;
    case NOX_SENSOR:
      this->_sensorType = nox_sensor_params;
      break;
    case VOLUME_SENSOR:
      this->_sensorType = volume_params;
      break;
    case LIGHT_SENSOR:
      this->_sensorType = light_sensor_params;
      break;
    case BLUE_LIGHT_SENSOR:
      this->_sensorType = blue_light_sensor_params;
      break;
    case RED_LIGHT_SENSOR:
      this->_sensorType = red_light_sensor_params;
      break;
    case ULTRAVIOLET_LIGHT_SENSOR:
      this->_sensorType = ultraviolet_light_sensor_params;
      break;
    case ANALOGIC_THERMOMETER:
      this->_sensorType = analogic_thermometer_params;
      break;
    case SOIL_MOISTURE_METER:
      this->_sensorType = soil_moisture_params;
      break;
    default:
      this->_sensorType = ec_meter_params;
  }

  this->readingFunction = this->_sensorType.readingFunction;

  if(label == NULL)
  {
    this->_label = this->_sensorType.name;
  }
  else
  {
    this->_label = label;
  }


  /* Set default calibration */
  this->_numCalibrationPoints = this->_sensorType.numCalibrationPoints;
  for(int i = 0; i < this->_numCalibrationPoints; i++)
  {
    this->_calibrationPoints[i] = this->_sensorType.calibrationPoints[i];
  }

  this->_intercept = this->_sensorType.intercept;
  this->_slope = this->_sensorType.slope;
  this->numReadings = this->_sensorType.numReadings;
  this->_readDelay = this->_sensorType.readDelay;

  this->streams[0] = NULL;

}

Sensor::Sensor(short int inputPin, sensor_params sensorType, const char* label)
{
  /* Set configuration */
  this->pin = inputPin;

  this->_sensorType = sensorType;

  this->readingFunction = this->_sensorType.readingFunction;

  if(label == NULL)
  {
    this->_label = this->_sensorType.name;
  }
  else
  {
    this->_label = label;
  }


  /* Set default calibration */
  this->_numCalibrationPoints = this->_sensorType.numCalibrationPoints;
  for(int i = 0; i < this->_numCalibrationPoints; i++)
  {
    this->_calibrationPoints[i] = this->_sensorType.calibrationPoints[i];
  }

  this->_intercept = this->_sensorType.intercept;
  this->_slope = this->_sensorType.slope;
  this->numReadings = this->_sensorType.numReadings;
  this->_readDelay = this->_sensorType.readDelay;

  /* Initialize streams array */
  for(int i; i < MAX_IO_STREAMS; i++)
  {
    this->streams[i] = NULL;
  }


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

  return this->readingFunction(this->pin,this->numReadings);
  // return basicReading(this->pin,this->numReadings);
  // return this->_sensorType.readingFunction(this->pin,this->numReadings);
  // return basicReading(this->pin,this->numReadings);
}

String Sensor::formattedReading()
{

  /* Prepare message */
  char message[50];
  for(int i; i < 50; i++)
  {
    message[i] = 0x00;
  }

  const char* mu = this->getMeasureUnit();
  const char* label = this->_label;
  float val = this->collectInput();

  #ifdef ARDUINO_DUE

  sprintf(message,"%-20s: %f%s",label,val,mu);

  #else

  // Arduino inplementation does not include %f, therefore transform the value to string
  char str_val[8];
  dtostrf(val, 4, 3, str_val);

  sprintf(message,"%-7s: %s%s",label,str_val,mu);

  #endif

  // Convert it to String, otherwise the pointer will be discarded
  return (String)message;

}

void Sensor::printReading(int stream)
{
  this->streams[stream]->println(this->formattedReading());
}

void Sensor::printAll()
{
  for(int i = 0; i < MAX_IO_STREAMS; i++)
  {

    if(this->streams[i] != NULL)
    {
      this->printReading(i);
    }
  }

}

void Sensor::calibrate()
{
  short int pointsNumber = this->_numCalibrationPoints;
  float arrayY[pointsNumber];
  float arrayY2[pointsNumber];
  float arrayX[pointsNumber];
  float arrayX2[pointsNumber];
  float arrayXY[pointsNumber];

  float inputRawValue;
  char customKey;


  String message = "";
  message = String("sensor N° " + String(this->_label) + F("; Calibration points ") + String(pointsNumber) + F("; readings per  point ") + String(this->numReadings)); // put the sensor in the reference...
  this->streams[0]->println(message);
  // HC06.println(message);

  for (int i = 0; i < pointsNumber; i++)                   // for every calibration point do:
  {
    message = "";
    message = String("Insert the sensor in calibration point " + String(i + 1) + ". Press C to acquire"); // put the sensor in the reference...
    this->streams[0]->println(message);
    // HC06.println(message);

    customKey = NULL;
    while (customKey != 'c' && customKey != 'C')                                  //read and print the sensor value and
    {
      customKey = this->streams[0]->read();

      inputRawValue = this->collectRawInput();

      arrayX[i] = inputRawValue;
      arrayY[i] = this->_calibrationPoints[i];
      message = "";
      message = String("Raw value = " + String(inputRawValue) + "; Current value = " + String(this->convertInputLinear(inputRawValue))); //
      this->streams[0]->println(message);

      delay(100);
    }
    delay(3000);
    message = "";
    message = String("Reference point " + String(i + 1) + " acquired");        // AFTER data acquisition calculate the arrays
    this->streams[0]->println(message);

    arrayX2[i] = arrayX[i] * arrayX[i];
    arrayY2[i] = arrayY[i] * arrayY[i];
    arrayXY[i] = arrayX[i] * arrayY[i];

#ifdef DEBUG_linearCalibration
    message = String("x = " + String(arrayX[i]));
    this->streams[0]->println(message);
    // HC06.println(message);
    message = String("x2 = " + String(arrayX2[i]));
    this->streams[0]->println(message);
    // HC06.println(message);
    message = String("y = " + String(arrayY[i]));
    this->streams[0]->println(message);
    // HC06.println(message);
    message = String("y2 = " + String(arrayY2[i]));
    this->streams[0]->println(message);
    // HC06.println(message);
    message = String("xy = " + String(arrayXY[i]));
    this->streams[0]->println(message);
    // HC06.println(message);
#endif
  }

  float sumX, sumY, sumX2, sumY2, sumXY = 0;        // sum of array's elements
  for (int ii = 0; ii < pointsNumber; ii++)
  {
    sumX = sumX + arrayX[ii];
    sumY = sumY + arrayY[ii];
    sumX2 = sumX2 + arrayX2[ii];
    sumY2 = sumY2 + arrayY2[ii];
    sumXY = sumXY + arrayXY[ii];
  }

  this->_intercept = (sumY * sumX2 - sumX * sumXY) / ((pointsNumber) * sumX2 - sumX * sumX);             //calculation of slope and intercept
  this->_slope = ((pointsNumber) * sumXY  - sumX * sumY) / ((pointsNumber) * sumX2 - sumX * sumX);
  // sensorsMatrix[sensorNumber][slopeCol] = slope;
  // sensorsMatrix[sensorNumber][interceptCol] = intercept;

#ifdef DEBUG_EEPROM
  EEPROM.write(sensorNumber, this->_slope);
  EEPROM.write(sensorNumber + 100, this->_intercept);
#endif

  message = String("Slope =  " + String(this->_slope, 4) + "; Intercept = " + String(this->_intercept, 4));
  this->streams[0]->println(message);
  // HC06.println(message);

#ifdef DEBUG_linearCalibration
  message = String("sum X = " + String(sumX, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("sum X2 = " + String(sumX2, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("sum Y = " + String(sumY, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("sum Y2 = " + String(sumY2, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("sum XY = " + String(sumXY, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("slope = " + String(this->_slope, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
  message = String("intercept = " + String(this->_intercept, 4));
  this->streams[0]->println(message);
  // HC06.println(message);
#endif

}

/* setValues: resets calibration to specified values, if intercept and slope are not given, they will not be modified */
void Sensor::setValues(float calibrationPoints[10], float intercept, float slope)
{
  // this->_calibrationPoints = calibrationPoints;
  for(int i = 0; i < sizeof(calibrationPoints); i++)
  {
    this->_calibrationPoints[i] = calibrationPoints[i];
  }

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
/* I/O management																																				*/
/*																																											*/
//////////////////////////////////////////////////////////////////////////////////////////

int Sensor::streamPush(InteractionChannel* ioChannel)
{

  int p = 0;
  while(this->streams[p] != NULL && p < MAX_IO_STREAMS) { p++; }

  /* If there's room for a new stream add it */
  if(p < MAX_IO_STREAMS)
  {
    this->streams[p] = ioChannel;
  } else {
    // Probably best would be to throw an exception
  }
  return p;

}

int Sensor::streamAdd(Stream &stream)
{

  InteractionChannel* ioChannel = new InteractionChannel(stream);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}

int Sensor::streamAdd(Stream &stream,char (*getKey)())
{

  InteractionChannel* ioChannel = new InteractionChannel(stream,getKey);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}

int Sensor::streamAdd(Stream &stream,void (*writeChar)(String s))
{

  InteractionChannel* ioChannel = new InteractionChannel(stream,writeChar);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}

int Sensor::streamAdd(char (*getKey)(),void (*writeChar)(String s))
{

  InteractionChannel* ioChannel = new InteractionChannel(getKey,writeChar);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}

/* Additional stream constructors provided by InteractionChannel */
// #ifdef InteractionChannel_h

/* Keypad and LCD constructors */
int Sensor::streamAdd(Stream &_stream,Keypad* keypad)
{
  InteractionChannel* ioChannel = new InteractionChannel(_stream,keypad);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}
int Sensor::streamAdd(Stream &_stream,LiquidCrystal &lcd)
{
  InteractionChannel* ioChannel = new InteractionChannel(_stream,lcd);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}
int Sensor::streamAdd(Keypad* keypad,LiquidCrystal* lcd)
{
  InteractionChannel* ioChannel = new InteractionChannel(keypad,lcd);
  int i = this->streamPush(ioChannel);
  this->streamTest(i);

  return i;

}

// #endif

void Sensor::streamTest(int stream)
{
  this->streams[stream]->println("Testing stream " + String(stream) + ". Press 'c' to exit.");

  /* Read and print on the stream until exit character is pressed */
  char c;

  while(c != 'c' && c != 'C')
  {

    while((c = this->streams[stream]->read()) < 1) { }
    this->streams[stream]->println(c);

  }

  this->streams[stream]->println("Testing of stream " + String(stream) + " complete.\n");

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
