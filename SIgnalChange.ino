#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>


const char *SSID = "Realme ";
const char *PWD = "3+2+1+2002";

int Greenlights[]={12,15};
int Redlights[]={13,14};
int k=0;
int density=0;
int changeroad=0;



void allred()
{
    for(int i=0;i<2;i++)
    {
    digitalWrite(Greenlights[i],LOW);
    digitalWrite(Redlights[i],HIGH);
    Serial.println("All Red");
    }

}
void Green(int Rno,int waittime)
{
    allred();
    digitalWrite(Redlights[Rno],LOW);
    Serial.print("Red turned off in Road ");
    Serial.println(Rno+1);
    digitalWrite(Greenlights[Rno],HIGH);
    Serial.print("Green turned on in Road ");
    Serial.println(Rno+1);
    delay(waittime);
}


// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient); 
char *mqttServer = "broker.hivemq.com";
int mqttPort = 1883;
void setupMQTT() {
  mqttClient.setServer(mqttServer, mqttPort);
  // set the callback function
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  char str[10];
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    str[i]=(char)payload[i];
  }
  Serial.println("\n");
  density=int(str);
}

void connectToWiFi() {
  Serial.print("Connecting to ");
 
  WiFi.begin(SSID, PWD);
  Serial.println(SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("Connected.");
  
}

void reconnect() {
  Serial.println("Connecting to MQTT Broker...");
  while (!mqttClient.connected()) {
      Serial.println("Reconnecting to MQTT Broker..");
      String clientId = "ESP32Client-";
      clientId += String(random(0xffff), HEX);
      
      if (mqttClient.connect(clientId.c_str())) {
        Serial.println("Connected.");
        // subscribe to topic
        mqttClient.subscribe("encyclopedia/temperature");
      }
      
  }
}




void setup() {
  Serial.begin(9600);
  // put your setup code here, to run once:
 for(int i=0;i<2;i++)
    {
    pinMode(Greenlights[i],OUTPUT);
    pinMode(Redlights[i],OUTPUT);
    }

    connectToWiFi();
    setupMQTT();
}

void loop() {
  // put your main code here, to run repeatedly:
 
 if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
    { if(density>6)
      { Serial.println("road changed!!");
        Green(1,6000);}
      Green(k,3000);
      k=k+1;
      if(k>1)
      {k=0;}     
  
    }


}
