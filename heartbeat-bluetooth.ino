
#include <DFRobot_MAX30102.h>
#include <SoftwareSerial.h>

DFRobot_MAX30102 particleSensor;
SoftwareSerial hc06(2,3);

/*
Macro definition opions in sensor configuration 
sampleAverage: SAMPLEAVG_1 SAMPLEAVG_2 SAMPLEAVG_4 
               SAMPLEAVG_8 SAMPLEAVG_16 SAMPLEAVG_32
ledMode:       MODE_REDONLY  MODE_RED_IR  MODE_MULTILED
sampleRate:    PULSEWIDTH_69 PULSEWIDTH_118 PULSEWIDTH_215 PULSEWIDTH_411
pulseWidth:    SAMPLERATE_50 SAMPLERATE_100 SAMPLERATE_200 SAMPLERATE_400
               SAMPLERATE_800 SAMPLERATE_1000 SAMPLERATE_1600 SAMPLERATE_3200
adcRange:      ADCRANGE_2048 ADCRANGE_4096 ADCRANGE_8192 ADCRANGE_16384
*/
void setup()
{
  //Initialize serial 
  Serial.begin(115200);
  
  //Initialize Bluetooth Serial Port
  hc06.begin(115200);
  
  /*!
   *@brief Init sensor 
   *@param pWire IIC bus pointer object and construction device, can both pass or not pass parameters (Wire in default)
   *@param i2cAddr Chip IIC address (0x57 in default)
   *@return true or false
   */
  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
  }

  /*!
   *@brief Use macro definition to configure sensor
   *@param ledBrightness LED brightness, default value: 0x1F（6.4mA), Range: 0~255（0=Off, 255=50mA）
   *@param sampleAverage Average multiple samples then draw once, reduce data throughput, default 4 samples average
   *@param ledMode LED mode, default to use red light and IR at the same time 
   *@param sampleRate Sampling rate, default 400 samples every second 
   *@param pulseWidth Pulse width: the longer the pulse width, the wider the detection range. Default to be Max range
   *@param adcRange Measurement Range, default 4096 (nA), 15.63(pA) per LSB
   */
  particleSensor.sensorConfiguration(/*ledBrightness=*/0x1F, /*sampleAverage=*/SAMPLEAVG_4, \
                                  /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_400, \
                                  /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_4096);
}

int32_t SPO2; //SPO2
int8_t SPO2Valid; //Flag to display if SPO2 calculation is valid
int32_t heartRate; //Heart-rate
int8_t heartRateValid; //Flag to display if heart-rate calculation is valid 

void loop()
{
  Serial.println(F("Wait about four seconds"));
  particleSensor.heartrateAndOxygenSaturation(/**SPO2=*/&SPO2, /**SPO2Valid=*/&SPO2Valid, /**heartRate=*/&heartRate, /**heartRateValid=*/&heartRateValid);
  
  //Print result 

  if (heartRateValid) {
    hc06.write(heartRate%128);
    hc06.write((heartRate/128)%128);
  } else {
    Serial.println("Failed to read heart rate");
  }


  if (SPO2Valid) {
    hc06.write(SPO2%128);
    hc06.write((SPO2/128)%128);
  } else {
    Serial.println("Failed to read spo2");
  }

}
