#include <BluetoothSerial.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <EEPROM.h>
#include<esp_wifi.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
static String data_app;
BluetoothSerial SerialBT;
#define SET_PIN 13 //此引脚用于删除已保存的WiFi信息，删除后重新进行网络配置
bool init_wifi()
{
  int count = 0;
  WiFi.begin();
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    count++;
    if (count == 20)
    {
      return false;
    }
  }
  return true;
}

void setup()
{
  Serial.begin(115200);
  WiFi.hostname("connect_wifi");
  SerialBT.begin("connect_wifi");
  Serial.println("现在可进行蓝牙配对!");
  if (init_wifi())
  {
    Serial.println();
    Serial.println("wifi连接成功");
    Serial.println(WiFi.localIP().toString());
  }
  else
  {
    Serial.println();
    Serial.println("wifi连接失败");
  }
//    pinMode(SET_PIN,INPUT_PULLUP);
 }
bool wifiPW(String sid, String pwd)
{ 
  WiFi.begin(sid.c_str(), pwd.c_str());
  int count = 0;
  delay(1000);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    count++;
    if (count > 20)
    {
      Serial.println();
      Serial.println("配网失败");
      return false;
    }
  }
  Serial.println();
  Serial.println("配网成功");
  return true;
}

String ipmac()
{ 
  if (WiFi.isConnected())
  {
    
    String ip = WiFi.localIP().toString();
    String mac = WiFi.macAddress();
    return "{'ip':'" + ip + "','mac':'" + mac + "'}";
  }
  else
  {
    return "Not connected to wifi";
  }
}
void bluetooth()
{ 
  if (Serial.available())
  { 
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available())
  {
    data_app = "";
    data_app = SerialBT.readString();
    Serial.print("接收数据为："); 
    Serial.println(data_app);     
    if (data_app == "ip_mac")
    {
      String ip_mac = ipmac();
      SerialBT.println(ip_mac);
      Serial.println(ip_mac);
    }
    else if (data_app.indexOf("ssid") != -1)
    {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data_app);
      if (error)
      {
        Serial.println("数据格式错误"); 
        SerialBT.print("Incorrect format of incoming data");
      }
      else
      {
        String sid = doc["ssid"];
        String pwd = doc["password"];
        if (sid != "null")
        {
          Serial.printf("SSID:%s\r\n", sid); 
          if (pwd == "null")
          {
            pwd = "";
          }
          Serial.printf("PAWD:%s\r\n", pwd); 
          if (wifiPW(sid, pwd))
          {
            SerialBT.println("True");
          }
          else
          {
            SerialBT.println("False");
          }
        }
        else
        {
          Serial.println("未识别到wifi数据"); 
          SerialBT.print("Incorrect format of incoming data");
        }
      }
    }
    // else
    // {
    //   Serial.println("未接收到指令"); 
    //   SerialBT.print("Incorrect format of incoming data");
    // }
  }
}

void loop()
{
  bluetooth();
//  if(digitalRead(SET_PIN)==LOW){
//   esp_wifi_restore();
//   delay(100);
//   ESP.restart();
//  }

}