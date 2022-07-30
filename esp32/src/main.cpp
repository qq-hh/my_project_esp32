#include <Adafruit_NeoPixel.h>
#include <ArduinoJson.h>
#include <WiFi.h>
//#include <Arduino.h>
#define PIN 22
#define NUMPIXELS 120 //灯个数
int i = 0;
#define ledr 16
#define ledg 17
#define ledb 21
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void Task1(void *pvParameters);
void Task2(void *pvParameters);
const char *ssid = "Bean";
const char *password = "Bean@#Tech";
String ss = "";
String ww = "";
String yy = "";
uint32_t red = strip.Color(255, 0, 0);
uint32_t green = strip.Color(0, 255, 0);
uint32_t off_close = strip.Color(0, 0, 0);
uint32_t yellow = strip.Color(255, 255, 0);
uint32_t orange = strip.Color(255, 97, 0);
uint32_t blue = strip.Color(0, 0, 255);
uint32_t white = strip.Color(255, 192, 203);
String red_3s = "001";
String green_3s = "002";
String yellow_1 = "003";
String orange_1 = "004";
String blue_1 = "005";
String white_1 = "006";
String x_orange = "007";
String x_blue = "008";
String X_white = "009";
WiFiServer server(80);
//静态地址、网关、子网掩码
IPAddress local_IP(192, 168, 1, 119);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
String httpResponse = "HTTP/1.0 200 OK\r\n"
                      "Connection: close\r\n"
                      "Content-Type: application/json;\r\n"
                      "\r\n"
                      "{\"code\":1}";

void Light_up(uint32_t mess, JsonObject obj) //只亮灯
{
  String num = obj["num"];
  for (int i = 0; i < obj["num"].size(); i++)
  {
    int j = obj["num"][i].as<int>();
    strip.setPixelColor(j, mess);
    strip.show();
  }
}

void On_off3s(uint32_t mess, JsonObject obj) //亮灯3s灭
{
  Light_up(mess, obj);
  vTaskDelay(3000);
  Light_up(off_close, obj);
}
void flash_light(uint32_t mess, JsonObject obj) // 3s内闪灯
{
  String num = obj["num"];
  for (int t = 0; t < 3; t++)
  {
    Light_up(off_close, obj);
    vTaskDelay(500);
    Light_up(mess, obj);
    vTaskDelay(500);
  }
}
void showw(String message)
{

  DynamicJsonDocument doc(1024);
  deserializeJson(doc, message);
  JsonObject obj = doc.as<JsonObject>();
  String nameStr = obj["action"];
  String num = obj["num"];
  Serial.println(nameStr);
  int R = 0, G = 0, B = 0;
  if (nameStr == red_3s || nameStr == green_3s)
  { //亮灯3S灭
    if (nameStr == red_3s)
    { // red
      On_off3s(red, obj);
    }
    else
    { // green
      On_off3s(green, obj);
    }
  }
  else if (nameStr == yellow_1 || nameStr == orange_1 || nameStr == blue_1 || nameStr == white_1)
  { //只亮灯部分
    if (nameStr == yellow_1)
    { // yellow
      Light_up(yellow, obj);
    }
    else if (nameStr == orange_1)
    { //橙色
      Light_up(orange, obj);
    }
    else if (nameStr == blue_1)
    { //蓝色
      Light_up(blue, obj);
    }
    else
    { //白色
      Light_up(white, obj);
    }
  }
  else if (nameStr == x_orange || nameStr == x_blue || nameStr == X_white)
  { //闪灯部分
    if (nameStr == x_orange)
    {
      flash_light(orange, obj);
    }
    else if (nameStr == x_blue)
    {
      flash_light(blue, obj);
    }
    else
    { //白色闪
      flash_light(white, obj);
    }
  }
  else
  {
    return;
  }
}

void taskOne(void *parameter)
{
  while (1)
  {
    vTaskDelay(10);
    WiFiClient incomingClient = server.available();
    if (incomingClient)
    {
      Serial.println(xPortGetCoreID()); //查看使用哪个cpu大小核
      if (incomingClient.find("\r\n\r\n"))
      {
        while (incomingClient.available())
        {
          ss = ss + char(incomingClient.read());
        }
      }
      Serial.println(ss);
      incomingClient.print(httpResponse);
      showw(ss);
      ss = "";
    }
  }
}

void taskTwo(void *parameter)
{
  while (1)
  {
    vTaskDelay(10);

    WiFiClient incomingClient = server.available();
    if (incomingClient)
    {
      Serial.println(xPortGetCoreID());
      if (incomingClient.find("\r\n\r\n"))
      {
        while (incomingClient.available())
        {
          ww = ww + char(incomingClient.read());
        }
      }
      Serial.println(ww);
      incomingClient.print(httpResponse);
      showw(ww);
      ww = "";
    }
  }
}

void taskThree(void *parameter)
{
  while (1)
  {
    vTaskDelay(100);
    WiFiClient incomingClient = server.available();
    if (incomingClient)
    {
      Serial.println(xPortGetCoreID());
      if (incomingClient.find("\r\n\r\n"))
      {
        while (incomingClient.available())
        {
          yy = yy + char(incomingClient.read());
        }
      }
      Serial.println(yy);
      incomingClient.print(httpResponse);
      showw(yy);
      yy = "";
    }
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledr, OUTPUT);
  pinMode(ledg, OUTPUT);
  pinMode(ledb, OUTPUT);
  digitalWrite(ledr, 0);
  digitalWrite(ledg, 255);
  digitalWrite(ledb, 255);
  delay(3000);
  WiFi.config(local_IP, gateway, subnet); //设置静态IP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //路由器的WiFi名称和密码
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  digitalWrite(ledr, 255);
  digitalWrite(ledg, 0);
  digitalWrite(ledb, 255);
  server.begin();
  strip.begin();
  strip.setBrightness(50); //设置亮度 (0~255)
  delay(100);
  xTaskCreate(taskOne, "TaskOne", 10000, NULL, 1, NULL);
  xTaskCreate(taskTwo, "TaskTwo", 10000, NULL, 1, NULL);
  xTaskCreate(taskThree, "taskThree", 10000, NULL, 1, NULL);
}

void loop()
{
  delay(1000);
}
