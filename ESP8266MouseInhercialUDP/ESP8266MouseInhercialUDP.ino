#include <ESP8266WiFi.h>
#include <WiFiUDP.h>


#include <Wire.h>
#include <FaBo9Axis_MPU9250.h>

#include <WiFiClient.h>
//#include <ESP8266LLMNR.h>

#include <ESP8266mDNS.h>

FaBo9Axis fabo_9axis;
float ax,ay,az;
  float gx,gy,gz;
  float mx,my,mz;
  float Axyz[3];
  float temp;
int mapX;
int mapY;
String valor;

WiFiUDP Client;

byte serdata=0;
byte fromserver=0;

#include <WiFiManager.h>  
#include <DNSServer.h>
//#include <ESP8266WebServer.h>

 //ESP8266WebServer web_server(80);
WiFiServer server(80);
void setup() 
{
  
  Serial.begin(9600);
WiFiManager wifiManager;
 wifiManager.autoConnect("MouseInhercial");
 
   if (fabo_9axis.begin()) {
    Serial.println("RESET");
  delay(300);
    } else {
    Serial.println("device error");
  //  while(1);
  }

  
  while(WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
        delay(500);
  }
     Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());   
      //LLMNR.begin("esp8266");
     MDNS.begin("esp8266");
        //web_server.begin();
    Client.begin(81);
}

void loop() 
{getAccel_Data();
convertir();
udpsend();
//web_server.handleClient();
delay(100);
}

void udpsend()
  {
    //const char ip[]="192.168.1.78";
  valor = String(-mapX)+","+String(-mapY);
  Client.beginPacket(Client.remoteIP(),80);
  Client.print(valor);
  Client.endPacket();
  yield();
  }


void convertir()
{
    float pitchrad = atan(Axyz[0] / sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2])); // radians
  float rollrad = atan(Axyz[1] / sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2])); // radians
  float yawrad = atan(Axyz[2] / sqrt(Axyz[0] * Axyz[0] + Axyz[1] * Axyz[1])); 
  
  float rolldeg = 180 * (atan(Axyz[1] / sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2]))) / PI; // degrees
  float pitchdeg = 180 * (atan(Axyz[0] / sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2]))) / PI; // degrees
  float yawdeg = 180 * (atan(Axyz[2] / sqrt(Axyz[1] * Axyz[1] + Axyz[0] * Axyz[0]))) / PI; // degrees

  float Minx = -5;
  float Min = -15;//-30, -45, -15
  float Max = 15;// 30, 45, 15
  
   pitchdeg = constrain(pitchdeg, Min, Max);
  mapX = map(pitchdeg, Min, Max,-10,10);

  
  rolldeg = constrain(rolldeg, Minx, Max);
  mapY = map(rolldeg, Minx, Max,-10,10);
 
 
 // int mapZ = map(yawdeg, Min, Max, -2, 2);
//Serial.println(String(rolldeg)+", "+String( pitchdeg));
Serial.println(String(-mapX)+","+String(-mapY));
delay(30);

  
  }

  void getAccel_Data(void)
{
  
  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  fabo_9axis.readGyroXYZ(&gx,&gy,&gz);
  fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
  fabo_9axis.readTemperature(&temp);
  
  Axyz[0] = ( ax );
  Axyz[1] = (ay );
  Axyz[2] =  az;

}
