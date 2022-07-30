#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <esp_wifi.h>
const char* AP_SSID  = "test"; //热点名称
String wifi_ssid = "";
String wifi_pass = "";
String scanNetworksID = "";//用于储存扫描到的WiFi
#define ledr 22
#define  ledg 23
#define  ledb  21
// #define ledr 1
// #define  ledg 2
// #define  ledb 3
WiFiServer server1(80);
DynamicJsonDocument  doc(1024);
String clientRequest = "";
#define ROOT_HTML  "<!DOCTYPE html><html><head><title>WIFI Config </title><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"></head><style type=\"text/css\">.input{display: block; margin-top: 10px;}.input span{width: 100px; float: left; float: left; height: 36px; line-height: 36px;}.input input{height: 30px;width: 200px;}.btn{width: 120px; height: 35px; background-color: #000000; border:0px; color:#ffffff; margin-top:15px; margin-left:100px;}</style><body><form method=\"GET\" action=\"connect\"><label class=\"input\"><span>WiFi SSID</span><input type=\"text\" name=\"ssid\"></label><label class=\"input\"><span>WiFi PASS</span><input type=\"text\"  name=\"pass\"></label><input class=\"btn\" type=\"submit\" name=\"submit\" value=\"Submie\"> <p><span>"
int i = 0;

WebServer server(80);
#define RESET_PIN   13 //用于删除WiFi信息13
void HPPIP()
{
   String  ipa = WiFi.localIP().toString().c_str();
 String HTML_TITLE = "<!DOCTYPE html><head> <meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> <meta http-equiv=\"X-UA-Compatible\" content=\"ie=edge\"><title>ESP8266网页配网</title>";
    String HTML_SCRIPT_ONE = "<head><script type=\"text/javascript\">function socket(){var xmlhttp = new XMLHttpRequest();xmlhttp.open(\"GET\", \"/socket\", true); xmlhttp.send();}</script></head>";
    String HTTT= HTML_TITLE +HTML_TITLE + HTML_SCRIPT_ONE +  "<body > <div class=\"content\"><div class=\"header\"><p id=\"information\">IP:";

   String we ="<input class=\"button\" type=\"button\" value=\"关闭\" onclick=\"socket()\">";
   
     String ysl =HTTT+ipa+"</p> "+ we;
 
  server.send(200, "text/html", ysl);

}

void socket(){
   Serial.println("9999");
i++;
  server.close();
  WiFi.mode(WIFI_STA);
 
}
void wifi_Config()
{
   server.begin();
  WiFi.mode(WIFI_AP);//配置为AP模式
  boolean result = WiFi.softAP(AP_SSID, ""); //开启WIFI热点
  if (result)
  {
    IPAddress myIP = WiFi.softAPIP();
    //打印相关信息
    Serial.println("");
    Serial.print("Soft-AP IP address = ");
    Serial.println(myIP);
    Serial.println(String("MAC address = ")  + WiFi.softAPmacAddress().c_str());
    Serial.println("waiting ...");
  } else {  //开启热点失败
    Serial.println("WiFiAP Failed");
    delay(3000);
    ESP.restart();  //复位esp32
  }

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
        digitalWrite(ledr,0);
      digitalWrite(ledg,255);
        digitalWrite(ledb,255);
  }

  // 首页
  server.on("/", []() {
    server.send(200, "text/html", ROOT_HTML );

  });

  //连接
  server.on("/connect", []() {
    
  server.send(200, "text/html", "<html><body><font size=\"10\">successd!  wifi connecting...<br /></font></body></html>");
   

    WiFi.softAPdisconnect(true);
    //获取输入的WIFI账户和密码
    wifi_ssid = server.arg("ssid");
    wifi_pass = server.arg("pass");

    WiFi.softAPdisconnect();
    Serial.println("WiFi Connect SSID:" + wifi_ssid + "  PASS:" + wifi_pass);

    //设置为STA模式并连接WIFI
    WiFi.mode(WIFI_AP_STA);
    WiFi.begin(wifi_ssid.c_str(), wifi_pass.c_str());
    uint8_t Connect_time = 0; //用于连接计时，如果长时间连接不成功，复位设备
    while (WiFi.status() != WL_CONNECTED) {  //等待WIFI连接成功
      delay(500);
          digitalWrite(ledr,255);
      digitalWrite(ledg,255);
        digitalWrite(ledb,0);
      Serial.print(".");
      Connect_time ++;
      if (Connect_time > 80) {  //长时间连接不上，复位设备
        Serial.println("Connection timeout, check input is correct or try again later!");
        delay(3000);
        ESP.restart();
      }
    }
  });
}

//用于上电自动连接WiFi
bool AutoConfig()
{
  WiFi.begin();
  for (int i = 0; i < 20; i++)
  {    digitalWrite(ledr,255);
      digitalWrite(ledg,255);
        digitalWrite(ledb,0);
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
      Serial.println("WIFI SmartConfig Success");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf(", PSW:%s\r\n", WiFi.psk().c_str());
      Serial.print("LocalIP:");
      Serial.print(WiFi.localIP());
      Serial.print(" ,GateIP:");
      Serial.println(WiFi.gatewayIP());
      return true;
    }
    else
    {
      Serial.print("WIFI AutoConfig Waiting......");
      Serial.println(wstatus);
      delay(1000);
    }
  }
  Serial.println("WIFI AutoConfig Faild!" );
  return false;
}
#define AP_SSID1 "kongb"
// 设置热点的密码
#define AP_PSW1  "text123456"
IPAddress local_IP(192, 168, 9, 1);
// 设置网关
IPAddress gateway(192, 168, 9, 1);
// 设置子网掩码
IPAddress subnet(255, 255, 255, 0);
void setup() {
  Serial.begin(115200);
  pinMode(RESET_PIN, INPUT_PULLUP);
    pinMode(ledr,OUTPUT);
    pinMode(ledg,OUTPUT);
    pinMode(ledb,OUTPUT);
    digitalWrite(ledr,0);
      digitalWrite(ledg,255);
        digitalWrite(ledb,255);
  // 连接WiFi
  if (!AutoConfig())
  {
    wifi_Config();
  }

  while (1) {
    server.handleClient();
    if (WiFi.status() == WL_CONNECTED) {
            digitalWrite(ledr,255);
          digitalWrite(ledg,0);
            digitalWrite(ledb,255);
      Serial.print(WiFi.localIP());
      break;
    }
  }
  WiFi.mode(WIFI_AP_STA);
  // 设置IP地址网关和子网掩码
  WiFi.softAPConfig(local_IP, gateway, subnet);
  // 设置wifi的名字和密码
  WiFi.softAP(AP_SSID1, AP_PSW1);
  server1.begin();
}
const char* host1 = "192.168.8.111"; 
const char* host2 = "192.168.8.112"; 
const char* host3 = "192.168.8.113"; 
const char* host4 = "192.168.8.114"; 
const char* host5 = "192.168.8.115"; 
const int httpPort = 80;          
String http =  String("GET /") + " HTTP/1.1\r\n" +
               "Host: " + "\r\n" +
               "Connection: close\r\n\r\n" ;
String httpResponse = "HTTP/1.0 200 OK\r\n"
                      "Connection: close\r\n"
                      "Content-Type: application/json;\r\n"
                      "\r\n"
                      "{\"code\":1}";
String httpRes = "HTTP/1.0 200 OK\r\n"
                 "Connection: close\r\n"
                 "Content-Type: application/json;\r\n"
                 "\r\n"
                 "{\"code\":flase}";
void loop() {
    //用于删除已存WiFi
  if (digitalRead(RESET_PIN) == LOW) {
    delay(1000);
    esp_wifi_restore();
    delay(10);
      digitalWrite(ledr,255);
        digitalWrite(ledg,255);
          digitalWrite(ledb,0);
    delay(3000);
    ESP.restart();  //复位esp32
  }
  WiFiClient client;
  DynamicJsonDocument  doc(1024);
  WiFiClient incomingClient = server1.available();
  if (incomingClient) {
    // Serial.println(incomingClient);
    Serial.println("====Client  Connected===");
    // 通过串口监视器输出客户端请求信息
    if (incomingClient.find("\r\n\r\n")) {
      while (incomingClient.available()) {
        clientRequest = clientRequest + char(incomingClient.read());
      }
      Serial.print(clientRequest);
      delay(1);
      incomingClient.print(httpResponse);

      deserializeJson(doc, clientRequest);
      JsonObject obj = doc.as<JsonObject>();
      String nameStr = obj["sub"];
      String num = obj["num"];
      int j = obj["num"][i].as<int>();

      if (nameStr == "1") {
        String hh = http + clientRequest;
        if (client.connect(host1, httpPort)) {
          // 连接成功后串口输出“Success”信息
          client.print(hh);              // 向服务器发送请求
        }
      }
      if (nameStr == "2") {
        Serial.println("123");
        String hh = http + clientRequest;
        if (client.connect(host2, httpPort)) {
          // 连接成功后串口输出“Success”信息
          client.print(hh);              // 向服务器发送请求
        }
      }
            if (nameStr == "3") {
        String hh = http + clientRequest;
        if (client.connect(host3, httpPort)) {
          // 连接成功后串口输出“Success”信息
          client.print(hh);              // 向服务器发送请求
        }
      }
            if (nameStr == "4") {
        String hh = http + clientRequest;
        if (client.connect(host4, httpPort)) {
          // 连接成功后串口输出“Success”信息
          client.print(hh);              // 向服务器发送请求
        }
      }
            if (nameStr == "5") {
        String hh = http + clientRequest;
        if (client.connect(host5, httpPort)) {
          // 连接成功后串口输出“Success”信息
          client.print(hh);              // 向服务器发送请求
        }
      }
      clientRequest = "";
    }

  }
}