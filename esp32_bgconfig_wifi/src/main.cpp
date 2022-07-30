#include <Arduino.h>
// #include <BGWiFiConfig.h>
// BGWiFiConfig wifipw;

// void setup() {
//   Serial.begin(115200);
//   pinMode(0, INPUT);
//   wifipw.begin();
// }

// void loop() {
//   wifipw.Loop();
//   if (!digitalRead(0))
//     wifipw.clearWiFi();
// }

#include <WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "********";//wifi账号
const char* password = "********";//wifi密码

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // 本地监听端口
char incomingPacket[255];  // 存储Udp客户端发过来的数据
char  replyPacket[] = "Hi there! Got the message :-)";  // 应答信息


void setup()
{
  Serial.begin(115200);
  Serial.println();

  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");

  //启动Udp监听服务
  Udp.begin(localUdpPort);
  //打印本地ip地址，udp client端会使用到
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  //解析Udp数据包
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // 收到Udp数据包
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    // 读取Udp数据包
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0;
    }
    //向串口调试器打印信息
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    //往udp 远端发送应答信息
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    int i=0;
    while(replyPacket[i++])
    Udp.write((uint8_t)replyPacket[i++]);
    //Udp.write(replyPacket);
    Udp.endPacket();
  }
}
