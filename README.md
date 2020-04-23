## Sensirion SFM3200 and SFM3400 library for Arduino

This library provides a lightweight interface to the Sensirion SFM3400 and SFM3200 flow meters. It is loosely based on the Sensirion SFM driver library for embedded platforms.  For easy interfacing within the Arduino environment, the Sensirioin I2C implementation has been replaced by the Arduino Wire library in order to communicate with the sensors.

### Useage

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


### Additional Documentation

Original Sensirion code can be found here: https://github.com/Sensirion/embedded-sfm

For more information on the SFM3X00 series operation and I2C commands see the application note:  
[SFM3X00 I2C Application Note](https://www.sensirion.com/fileadmin/user_upload/customers/sensirion/Dokumente/5_Mass_Flow_Meters/Sensirion_Mass_Flow_Meters_Application_Note_SFM3xxx.pdf)

For more information on adding a library to your Arduino IDE: https://www.arduino.cc/en/guide/libraries
