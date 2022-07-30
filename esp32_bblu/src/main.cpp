#include <Arduino.h>

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test");
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {  
  if (Serial.available()) {
  //int message = Serial.read();//注意print是将数据转换成字符再将对应的ASCLL码发送出去，串口监视器接收到ASCLL码，则会显示对应的字符。因此，使用print（）函数是以ASCLL码的形式输出数据到串口。
    SerialBT.write(Serial.read());
    delay(300);
 //write()函数时，Arduino发送的是数值本身。但是串口监视器接收到数据后，会将数值当做ASCLL码而显示其对应的字符。
  }
  if (SerialBT.available()) {
    //int ysl = SerialBT.read();
    Serial.write(SerialBT.read());
    delay(300);
  }
  delay(20);
}