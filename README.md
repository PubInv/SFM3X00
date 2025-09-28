[![DOI](https://zenodo.org/badge/257937618.svg)](https://zenodo.org/badge/latestdoi/257937618)

# Sensirion SFM3200 and SFM3400 library for Arduino

This library provides a lightweight interface to the Sensirion SFM3400 and SFM3200 flow sensors. It is loosely based on the Sensirion SFM driver library for embedded platforms.  For easy interfacing within the Arduino environment, the Sensirion I2C implementation has been replaced by the Arduino Wire library in order to communicate with the sensors.

Flow sensors are essential components of machines, such as ICU Ventilators and BPAP machines, that provide respiratory support
for COVID-19 patients. At the time of this writng, there is global interest in flow sensors and the Sensirion sensors are out-of-stock worldwide, although Sensirion is ramping up production.

To address supply-chain shortages, some of use have used the SFM3400 neonatal flow sensor because the SFM3200 cannot be obtained. A library that encapsulates both of these provides supply chain resilience and makes them easier to use.

### Usage

See the examples directory for a sample Arduino sketch. Basic useage is below.

>SFM3X00 flowSensor(SENSOR_ADDRESS);
>
>void setup() {
>  Serial.begin(9600);
>  flowSensor.begin();
>}
>
> void loop() {
>  float flow = flowSensor.readFlow();
>  Serial.println(flow);
>}

## SFM4X00 provided by Ben Kokes

Ben Kokes (@benkokes) has provided the SFM4x00 library, which has an example test file. We have committed it even thought it is not fully
integrated; it may be better to integrate the libraries better. Ideally one would read flow in the same way from any sensor
(though initialization and features may vary.) That is a todo item.

### A note on ranges

Particularly in the time of the COVID-19 pandemic, supply chain disruptions may force the usage of sensors outside the designed specification. We have there for provided a function, checkRange, which returns true if the flow sensor has exceeded its range as measured by our own devices, as exemplified here:

>  if(flowSensor.checkRange(flow)) {
>    Serial.print("flow exceeded sensor limits:  ");
>    Serial.print(flow);
>    Serial.println(" slm");
>  }  else  {
>    Serial.print("flow : ");
>    Serial.print(flow);
>    Serial.println(" slm");
>  }



## A Photo of the Two Sensors

The SFM3400 is a neonatal flow sensors; it is smaller than the SFM3200 and supports a smaller flow range.

![image](https://github.com/PubInv/SFM3X00/blob/master/extras/SFM3200andSFM3400.jpg)

## Additional Documentation

Original Sensirion code can be found here: https://github.com/Sensirion/embedded-sfm

For more information on the SFM3X00 series operation and I2C commands see the application note:
[SFM3X00 I2C Application Note](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_Application_Note_SFM3xxx.pdf)

For more information on adding a library to your Arduino IDE: https://www.arduino.cc/en/guide/libraries
