// #include <BLEDevice.h> // 蓝牙Ble设备库
// #include <BLEUtils.h> 
// #include <BLEScan.h> // 蓝牙ble设备的扫描功能库 本篇重点
// #include <BLEAdvertisedDevice.h> // 扫描到的蓝牙设备（广播状态）
// #include <Arduino.h>
// int scanTime = 5; //扫描时间
// BLEScan* pBLEScan; // 扫描对象

// // 扫描广播设备结果回调
// class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
//     void onResult(BLEAdvertisedDevice advertisedDevice) {
//       Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
//     }
// };

// void setup() {
//   Serial.begin(115200);
//   Serial.println("Scanning...");

//   // ESP32首先得是一个BLEDevice
//   BLEDevice::init(""); 
//   //  获取 BLEDevice下的Scan子功能
//   pBLEScan = BLEDevice::getScan(); 
//   // 配置一系列扫描设置
//   pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());// 注册扫描结果回调
//   pBLEScan->setActiveScan(true); //active scan uses more power, but get results faster // 配置主动扫描
//   pBLEScan->setInterval(100); // 配置扫描PDU间隔
//   pBLEScan->setWindow(99);  // less or equal setInterval value // 设置扫描窗口大小，需要小于扫描间隔
// }

// void loop() {
//   // 同步扫描，等待结果
//   BLEScanResults foundDevices = pBLEScan->start(scanTime, false);// 开始扫描 等待扫描结果
//   Serial.print("Devices found: ");
//   Serial.println(foundDevices.getCount());
//   Serial.println("Scan done!");
//   //  清除缓存
//   pBLEScan->clearResults();   // delete results fromBLEScan buffer to release memory，释放扫描缓存消耗
//   delay(2000);
// }

#include <Arduino.h>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
//软件蓝牙配对发送消息测试
void setup()
{
    Serial.begin(115200);
    SerialBT.begin("xiaoqiu"); // 如果没有参数传入则默认是蓝牙名称是: "ESP32"
    SerialBT.setPin("1234");   // 蓝牙连接的配对码
    Serial.printf("BT initial ok and ready to pair. \r\n");
}
   String  mess;
void loop()
{
    // if (Serial.available())
    // {
    //     SerialBT.write(Serial.read());
    // }
    if (SerialBT.available())
    {
      while (SerialBT.available())
      {
        
     
        mess=mess+char(SerialBT.read());
        // Serial.write(SerialBT.read());

      }
      Serial.println(mess);
     
        if (mess=="hello")
        {
         Serial.println("123");
        }
         mess="";
    }
    delay(1);

}

