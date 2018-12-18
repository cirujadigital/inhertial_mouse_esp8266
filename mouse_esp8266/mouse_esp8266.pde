import java.util.*;
import java.net.InetAddress; //Para mDNS y obtener la IP del ESP8266

import hypermedia.net.*; //Para el UDP
 
import java.awt.Robot;
import java.awt.event.InputEvent;
Robot rbt;


int timeOut = 5000;
boolean connected;
InetAddress inet;
int posmouseX;
int posmouseY;

UDP udp;  

int puerto_destino =81;
int puerto_local= 80;
int x;
int y;
boolean click;

String IP;
void setup()
{
  
try
{
 
 inet = InetAddress.getByName("esp8266.local");
 //println(inet);
 connected = inet.isReachable(timeOut);
 if(connected == true)
 {
 println(inet.getHostName()); 
println(inet.getHostAddress());
IP=inet.getHostAddress();

 }
 else
 {
  println("no puedo conectar");

 }
}
catch (Exception e)
{
 e.printStackTrace();
}
  //IP="192.168.1.145"; //prueba
 udp = new UDP( this, 80 );
  udp.listen( true );
  udp.send( "a", IP, puerto_destino );
 
 try {
    rbt = new Robot();
  } catch(Exception e) {
    e.printStackTrace();
  }
  
  posmouseX= mouseX
  posmouseY= mouseY
}
 
void draw()
{
  rbt.mouseMove(x, y);
 // rbt.mousePress(InputEvent.BUTTON1_MASK);
}


//void receive( byte[] data ) {       
void receive( byte[] data) { 
  
  
  data = subset(data, 0, data.length);
  String message = new String( data );
  String[] valor = split(message, ',');
  
  x= parseInt(valor[0]);
  y= parseInt(valor[1]);
  posmouseX= posmouseX+x;
  posmouseY= posmouseY+y;
  println(x+","+y);
}

