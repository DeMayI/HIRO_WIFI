#include <HIRO.h>
#include <HIRO_Motion.h>
#include <HIRO.h>
#include <WiFi.h>


HIRO hiro; // Declare an intance of HIRO.

//set to true to have HIRO create its own wifi network, 
//if false HIRO will instead try to connect to the network listed in 
const bool ap = false;

//AP Settings
const char* ap_ssid = "Hiro";
const char* ap_password = "password123";
//Connnect command(using netcat) MUST BE CONNECTED TO WIFI IN ORDER TO WORK
//nc 192.168.4.1 23

//Home Wifi Settings:
const char* home_ssid = "bluehouse";
const char* home_password = "1234432112344321";
WiFiServer wifiSerialServer(23);
WiFiClient remoteClient;


void setup() {
  hiro.begin();
  //hiro.genZeroBegin(); Uncomment this line and comment the line above if you are using H.I.R.O. Gen Zero Edition.
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  neutralStance(hiro);
  rgbRandom();
  delay(100);

  //Access Point Creation
  if(ap){
    Serial.print("Creating access point...");
    WiFi.softAP(ap_ssid, ap_password);
    wifiSerialServer.begin();
    Serial.println("Wifi Initialized");
    Serial.print("Network Name: ");
    Serial.println(ap_ssid);
    Serial.print("IP Address to connect to: ");
    Serial.println(WiFi.softAPIP()); //defaults to 192.168.4.1
  } else {
    //Home network logic
    Serial.print("Connecting to network: ");
    Serial.println(home_ssid);

    WiFi.begin(home_ssid, home_password);

    while(WiFi.status() != WL_CONNECTED){
      delay(500);
      Serial.print("Connecting...\n");

    }
    wifiSerialServer.begin();
    Serial.println("\nWiFi Connected Successfully!");
    Serial.print("IP Address to connect to: ");
    Serial.println(WiFi.localIP());
  }
  
  
  
  Serial.println("Hello! My name is H.I.R.O!");
}

void loop() {
  if(!remoteClient || !remoteClient.connected()) {
    remoteClient = wifiSerialServer.accept();
    if(remoteClient){
      remoteClient.println("--- Connected to H.I.R.O ---");
      remoteClient.println("Enter command(1-10):");
    }
  }
  if(remoteClient && remoteClient.available()){
    int status = int(remoteClient.parseInt());

    remoteClient.print("Executing action: ");
    remoteClient.println(status);
    switch (status) {
      case 1:
       // rgbRandom();
        easyForward(hiro,4, 2);
        break;
      case 2:
        rgbRandom();
        easyBackward(hiro, 4, 2);
        
        break;
      case 3:
        wave(hiro);
        rgbRandom();
        break;
      case 4:
        rgbRandom();
        easyCounterClockwise(hiro,2, 2);
        break;
      case 5:
      rgbRandom();
        easyClockwise(hiro,2, 2);
        break;
      case 6:
        rgbRandom();
        sideToSideWorm(hiro, 2);
        break;
      case 7:
        rgbRandom();
        sideToSideStretch(hiro, 1);
        break;
      case 8:
        rgbRandom();
        pushUps(hiro, 2, 5);
        break;
      case 9:
        rgbRandom();
        cabbagePatch(hiro, 5,10);
        break;
      case 10:
        rgbRandom();
        cabbagePatch2(hiro, 4, 10);
        break;
      default:
        neutralStance(hiro);
        break;
    }
  }else{
    neutralStance(hiro);
  }
}

void rgbRandom(){
  analogWrite(hiro.red, random(0,10));
  analogWrite(hiro.green, random(0,10));
  analogWrite(hiro.blue, random(0,10));
}









