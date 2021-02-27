#include "sSense_BME680.h" 

#define SERIAL_SPEED  115200
BME680_Class BME680;


float altitude(const float seaLevel=1013.25) 
{
  /*wikipedia equation - original Zanshin code*/
  static float Altitude;
  int32_t temp, hum, press, gas;
  BME680.getSensorData(temp,hum,press,gas); // Get the most recent values from the device
  Altitude = 44330.0*(1.0-pow(((float)press/100.0)/seaLevel,0.1903)); // Convert into altitude in meters
  return(Altitude);
} 

float calculate_altitude( float pressure, bool metric = true, float seaLevelPressure = 101325)
{
  /*Equations courtesy of NOAA - code ported from BME280*/;
  float altitude = NAN;
  if (!isnan(pressure) && !isnan(seaLevelPressure)){
    altitude = 1000.0 * ( seaLevelPressure - pressure ) / 3386.3752577878;
  }
  return metric ? altitude * 0.3048 : altitude;
}

float temperatureCompensatedAltitude(int32_t pressure, float temp=21.0 /*Celsius*/, float seaLevel=1013.25) 
{
  /*Casio equation - code written by itbrainpower.net*/
  float Altitude;
  Altitude = (pow((seaLevel/((float)pressure/100.0)), (1/5.257))-1)*(temp + 273.15) / 0.0065; // Convert into altitude in meters
  return(Altitude);	//this are metric value
} 


void setup()
{
  DebugPort.begin(SERIAL_SPEED); // Start serial port at Baud rate

  while(!DebugPort) {delay(10);} // Wait

  DebugPort.println("s-Sense BME680 I2C sensor.");
  DebugPort.print("- Initializing BME680 sensor\n");
  while (!BME680.begin(I2C_STANDARD_MODE)) // Start BME680 using I2C protocol
  {
    DebugPort.println("-  Unable to find BME680. Waiting 1 seconds.");
    delay(1000);
  } // of loop until device is located
  DebugPort.println("- Setting 16x oversampling for all sensors");
  BME680.setOversampling(TemperatureSensor,Oversample16); // Use enumerated type values
  BME680.setOversampling(HumiditySensor,   Oversample16);
  BME680.setOversampling(PressureSensor,   Oversample16);
  DebugPort.println("- Setting IIR filter to a value of 4 samples");
  BME680.setIIRFilter(IIR4);
  DebugPort.println("- Setting gas measurement to 320C for 150ms");
  BME680.setGas(320,150); // 320�c for 150 milliseconds
  DebugPort.println();
} // of method setup()


void loop() 
{
  static int32_t temperature, humidity, pressure, gas;        // Variable to store readings
  BME680.getSensorData(temperature,humidity,pressure,gas);    // Get most recent readings

  String temp = String(temperature/100.0,2);
  String hum = String(humidity/1000.0,2);
  String pres = String(pressure/100.0,2);
  String GR = String(gas/100.0,2);
  
  
  send_to_serial_bt("START", String("1")); //# Begin of the message
  send_to_serial_bt("GR", GR); //# Gas resistance
  send_to_serial_bt("HUM", hum); //# Humidity in perc
  send_to_serial_bt("PSI", pres); //# Pressure in Hpa
  send_to_serial_bt("TMP", temp); //# Temp in C                
  send_to_serial_bt("END", String("1")); //# End of the message
  


  delay(1000);
} // of method loop()

void send_to_serial_bt(String prop_name, String value){
    String data = prop_name+":"+value+"_";
    int n = data.length();
    char char_array[n + 1];
    strcpy(char_array, data.c_str());
    Serial.write(char_array, n+1);
    Serial.println();
    delay(100);
  }
