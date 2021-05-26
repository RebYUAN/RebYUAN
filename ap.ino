#include <WiFi.h>
#include <Adafruit_BMP280.h>

//定义WiFi
//#define MAX_SRV_CLIENTS 1
const char* ssid = "ESP32";
const char* password = "12345678";
WiFiServer server(23);
WiFiClient client;

//定义传感器
#define BMP_SCK 19 //SCL
#define BMP_MOSI 18 //SDA
#define BMP_CS 17 //CSB
#define BMP_MISO 16  //SDO
Adafruit_BMP280 bmp280(BMP_CS, BMP_MOSI, BMP_MISO, BMP_SCK);


float pressure;
char c[9];

void setup() {
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());

  //启动UART传输和服务器
  server.begin();
  server.setNoDelay(false);
  
  Serial.print("Port:   ");
  Serial.print(WiFi.localIP());//获得服务器本地IP
  Serial.println(":23");

  //启动传感器
  if (!bmp280.begin()) {
    Serial.println(F("找不到对应的传感器"));
    while (1);
  }
  
}

void loop() {
  
  client = server.available();
  if(client){
      Serial.println("[Client connected]");
      String readBuff;
      while (client.connected()) //如果客户端处于连接状态
      {   
          pressure = bmp280.readPressure()+'\n';
          dtostrf(pressure,6,2,c);
          Serial.println(pressure);
//          delay(3);
          client.write(c);
          client.write('\n');
//          client.println();
          delay(1);
      }
      client.stop(); //结束当前连接:
      Serial.println("[Client disconnected]");
  }
////  Serial.println(pressure);
////  uint8_t i;
//  //检测服务器端是否有活动的客户端连接
//  if (server.hasClient()){
//   if (!serverClient || !serverClient.connected()){
//      if(serverClient) serverClient.stop();
//      serverClient = server.available();
//      Serial.print("New client"); 
////      continue; 
//    }
//    //若没有可用客户端，则停止连接
////    serverClient = server.available();
////    serverClient.stop();
//  }
//  //检查客户端的数据
//  if (serverClient && serverClient.connected()){
//    serverClient.println(pressure);
////    if(serverClient.available()){
////      //从Telnet客户端获取数据，并推送到URAT端口
////      while(serverClient.available()) Serial.write(serverClient.read());
////    }
//  }
////  serverClient.write(sbuf, len);
////  //检查UART端口数据
////  if(Serial.available()){
////    size_t len = Serial.available();
////    unsigned char sbuf[len];
////    Serial.readBytes(sbuf, len);
////    //将UART端口数据推送到所有已连接的telnet客户端，实现双向通信
////    if (serverClient && serverClient.connected()){
////      serverClients[i].write(sbuf, len);
////      delay(1);
////    }
////  }
}
