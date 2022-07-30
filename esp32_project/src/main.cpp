#include <Arduino.h>
#include <WiFi.h>
const char *ssid = "Bean";
const char *password = "Bean@#Tech";
IPAddress local_IP(192, 168, 1, 115);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
String mess = "";
String httpResponse = "HTTP/1.0 200 OK\r\n"
                      "Connection: close\r\n"
                      "Content-Type: application/json;\r\n"
                      "\r\n"
                      "{\"code\":1}";
WiFiServer server(80);
void setup()
{
  Serial.begin(115200);
  WiFi.config(local_IP, gateway, subnet); //设置静态IP
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //路由器的WiFi名称和密码
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
  server.begin();
  delay(100);
}

void loop()
{
  WiFiClient incomingClient = server.available();
  if (incomingClient)
  {
    Serial.println(xPortGetCoreID()); //查看使用哪个cpu大小核
    if (incomingClient.find("\r\n\r\n"))
    {
      while (incomingClient.available())
      {
        mess = mess + char(incomingClient.read());
      }
    }
    Serial.println(mess);
    incomingClient.print(httpResponse);
    mess = "";
  }
}