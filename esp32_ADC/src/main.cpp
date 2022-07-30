#include <Arduino.h>
#include <WiFi.h>
#include <Ticker.h>
#define SSID "Bean"          // 填入自己的WiFi账号
#define PASSWORD "Bean@#Tech"      // 填入自己的WiFi密码
void Restart(float t)
{
  delay(3000);
  ESP.restart();
}
void WifiConnect(void)
{
  Serial.printf("Connecting to WiFi:%s\n",SSID);// 串口打印当前WiFi热点的名字
  WiFi.disconnect();   // 默认断开之前的连接，回归初始化非连接状态
  WiFi.mode(WIFI_STA); // 设置ESP工作模式为Station模式
  WiFi.begin(SSID, PASSWORD);  // 连接到WiFi

  int cnt = 0; // 记录重试次数
  while (WiFi.status() != WL_CONNECTED) // 当还没有连接上WiFi热点时
  {
     delay(1000);       // 延时等待1秒
     cnt++;             // 累计次数+1
     Serial.print("."); // 串口输出.表示设备正在连接WiFi热点
     if(cnt++ >40){
       Restart();
       cnt=0;
     }
  }

  Serial.println(WiFi.localIP()); // 打印当前IP地址
}

void setup()
{
  Serial.begin(115200);
  pinMode(4, INPUT);
  WifiConnect();


}

void loop()
{
  // int analog_value = 0;
  // analog_value = analogRead(4);
  // Serial.println( analog_value);
  // delay(3000);
}
