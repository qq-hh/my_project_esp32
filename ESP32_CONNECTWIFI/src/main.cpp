#include <Arduino.h>
#include <WiFi.h> //导入WIFI功能库文件
#include <EEPROM.h> //导入Flash库文件

String ssid;
String psw;

#define MAX_SRV_CLIENTS 4 //定义TCP服务器最大允许4个连接数

//创建server 端口号是9601
WiFiServer server(9601);

WiFiClient serverClients[MAX_SRV_CLIENTS];

struct config_type
{
  char stassid[32];//定义配网得到的WIFI名长度(最大32字节)
  char stapsw[64];//定义配网得到的WIFI密码长度(最大64字节)
};

config_type config;//声明定义内容

void saveConfig()//保存函数
{
 EEPROM.begin(1024);//向系统申请1024kb ROM
 //开始写入
 uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    EEPROM.write(i, *(p + i)); //在闪存内模拟写入
  }
  EEPROM.commit();//执行写入ROM
}

void loadConfig()//读取函数
{
  EEPROM.begin(1024);
  uint8_t *p = (uint8_t*)(&config);
  for (int i = 0; i < sizeof(config); i++)
  {
    *(p + i) = EEPROM.read(i);
  }
  EEPROM.commit();
  ssid = config.stassid;
  psw = config.stapsw;
}

void smartConfig()//配网函数
{
  // 等待配网
  WiFi.beginSmartConfig();
 while(1)
  {
    digitalWrite(2, LOW);  //加个LED快闪，确认配网是否成功！成功就不闪了。
    delay(200);                    
    digitalWrite(2, HIGH);   
    delay(200);
    if (WiFi.smartConfigDone())
    {
      strcpy(config.stassid,WiFi.SSID().c_str());//名称复制
      strcpy(config.stapsw,WiFi.psk().c_str());//密码复制
      saveConfig();//调用保存函数
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    //启动server
    server.begin();
    //关闭小包合并包功能，不会延时发送数据
    server.setNoDelay(true);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  delay(500);
  pinMode(2, OUTPUT);//设定D4号脚为LED
  pinMode(0, INPUT);//设定按钮输入脚为D3
  //ESP.wdtEnable(10000);//设定看门狗
  loadConfig();//读取ROM是否包含密码
  //判断ROM是否有密码
  if(ssid!=0&&psw!=0){
    WiFi.begin(ssid.c_str(),psw.c_str());//如果有密码则自动连接
    while(WiFi.status()!= WL_CONNECTED){
    if(digitalRead(0)== LOW){
    smartConfig();//如果配网按钮被按下则停止当前连接开始配网
    break;//跳出所有循环进入主程序
    }
    digitalWrite(2, LOW);  //加个LED慢闪，确认联网是否成功！成功就不闪了。
    delay(1000);                    
    digitalWrite(2, HIGH);   
    delay(1000);
    }
    //启动server
    server.begin();
    //关闭小包合并包功能，不会延时发送数据
    server.setNoDelay(true);
    }else{
      smartConfig();//如果ROM没有密码则自动进入配网模式
    }
}

void loop() { 
 //ESP.wdtFeed();//先喂狗释放资源
 uint8_t i;
 if(digitalRead(0)== 0)delay(2000);if(digitalRead(0)==0)smartConfig();//如果配网按钮被按下则停止所有任务开始重新配网
  //检测是否有新的client请求进来
  //ESP.wdtFeed();//再喂狗释放资源
  if (server.hasClient()) {
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      //释放旧无效或者断开的client
      if (!serverClients[i] || !serverClients[i].connected()) {
        if (serverClients[i]) {
          serverClients[i].stop();
        }
        //分配最新的client
        serverClients[i] = server.available();
        break;
      }
    }
    //当达到最大连接数 无法释放无效的client，需要拒绝连接
    if (i == MAX_SRV_CLIENTS) {
      WiFiClient serverClient = server.available();
      serverClient.stop();
    }
  }
  //检测client发过来的数据
  for (i = 0; i < MAX_SRV_CLIENTS; i++) {
    if (serverClients[i] && serverClients[i].connected()) {
      if (serverClients[i].available()) {
        while (serverClients[i].available()) {
          //发送到串口调试器
          Serial.write(serverClients[i].read());
        }
      }
    }
  }
 
  if (Serial.available()) {
    //把串口调试器发过来的数据 发送给client
    size_t len = Serial.available();
    uint8_t sbuf[len];
    Serial.readBytes(sbuf, len);
    for (i = 0; i < MAX_SRV_CLIENTS; i++) {
      if (serverClients[i] && serverClients[i].connected()) {
        serverClients[i].write(sbuf, len);
        delay(1);
      }
    }
  }
}
