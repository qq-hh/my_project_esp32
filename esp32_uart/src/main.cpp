// #include <Arduino.h>
// #include <WiFi.h>

// const char* ssid ="Bean";
// const char* password ="Bean@#Tech";
// WiFiServer server(80);
// String message ="";
// void setup() {
//  Serial.begin(115200);
//   WiFi.begin(ssid,password);
//  while(WiFi.status()!=WL_CONNECTED){
//   delay(500);
//   Serial.print(".");
//  }
//  Serial.println(WiFi.localIP());
//  server.begin();
// }
// String httpResponse = "HTTP/1.0 200 OK\r\n"
//                             "Connection: close\r\n"
//                             "Content-Type: application/json;\r\n"
//                             "\r\n"
//                             "{\"code\":1}";
// void loop() {
//   WiFiClient incomingClient =server.available();
//   if(incomingClient){
//     if(incomingClient.find("\r\n\r\n")){
//       while(incomingClient.available()){
//         message=message+char(incomingClient.read());
//       }
//     }
//     Serial.println(message);
//     delay(10);
//     message="";
//     incomingClient.print(httpResponse); 
//   }
// }


#include <WiFi.h>
 
void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  delay(2000);
  // 等待配网
  WiFi.beginSmartConfig();
 while (1)
  {
    Serial.print(".");
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void setup()
{
  Serial.begin(115200);
  smartConfig();
}
 
void loop()
{
  delay(100);

}

