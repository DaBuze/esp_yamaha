#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi parameters
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";


const char* inTopicIRCode = "IR/Code";

String sLastCommand = "";


IRsend irsend(4); // An IR LED is controlled by GPIO pin 4 (D2)



void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.print("Serial Begin");

  Serial.println("Setup WiFi");
  setup_wifi();

  Serial.println("Setup Client");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  irsend.begin();
  
  Serial.println("Setup done");
}

void loop() 
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}


void setup_wifi() {

  delay(50);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname("NodeMCU_Keller_IR_Sender");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  delay(500);

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("NodeMCU_Keller_IR_Sender")) {
      Serial.println("connected");
      client.subscribe(inTopicIRCode);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 1,5 seconds");
      // Wait 1,5 seconds before retrying
      delay(1500);
    }
  }
}


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String sIncommingCode = "";
  
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
    sIncommingCode += (char)payload[i];
  }

  Serial.println();

  uint64_t sendCode;

  if (sIncommingCode == "FFFFFFFFFFFFFFFF")
  {
    Serial.print("Received FFFF code - repate last command: ");
    Serial.println(sLastCommand);
    sIncommingCode = sLastCommand;
  }
  else
  {
    sLastCommand = sIncommingCode;
  }
  

  if (sIncommingCode == "BE4112ED")
  {
    // power
    sendCode = 0xBE4112ED;
  }
  else if (sIncommingCode == "BE4120DF")
  {
    // DTS
    sendCode = 0xBE4120DF;
  }
  else if (sIncommingCode == "BE41A05F")
  {
    // THTR 1
    sendCode = 0xBE41A05F;
  }
  else if (sIncommingCode == "BE41609F")
  {
    // THTR 2
    sendCode = 0xBE41609F;
  }
  else if (sIncommingCode == "BE41E01F")
  {
    // Mono
    sendCode = 0xBE41E01F;
  }
  else if (sIncommingCode == "BE4110EF")
  {
    // TV Sports
    sendCode = 0xBE4110EF;
  }
  else if (sIncommingCode == "BE41906F")
  {
    // Disco
    sendCode = 0xBE41906F;
  }
  else if (sIncommingCode == "BE4150AF")
  {
    // Rock
    sendCode = 0xBE4150AF;
  }
  else if (sIncommingCode == "BE41D02F")
  {
    // Hall
    sendCode = 0xBE41D02F;
  }
  else if (sIncommingCode == "BE4158A7")
  {
    // TEST
    sendCode = 0xBE4158A7;
  }
  else if (sIncommingCode == "BE412AD5")
  {
    // 'Hoch'
    sendCode = 0xBE412AD5;
  }
  else if (sIncommingCode == "BE41B847")
  {
    // Effect
    sendCode = 0xBE41B847;
  }
  else if (sIncommingCode == "BE41F807")
  {
    // 'Links'
    sendCode = 0xBE41F807;
  }
  else if (sIncommingCode == "BE417887")
  {
    // 'Rechts'
    sendCode = 0xBE417887;
  }
  else if (sIncommingCode == "BE41D827")
  {
    // Time / Level
    sendCode = 0xBE41D827;
  }
  else if (sIncommingCode == "BE4128D7")
  {
    // 'Runter'
    sendCode = 0xBE4128D7;
  }
  else if (sIncommingCode == "BE418A75")
  {
    // Set Menu
    sendCode = 0xBE418A75;
  }
  else if (sIncommingCode == "BE410AF5")
  {
    // 6 CH Inpurt
    sendCode = 0xBE410AF5;
  }
  else if (sIncommingCode == "BE41A25D")
  {
    // CD
    sendCode = 0xBE41A25D;
  }
  else if (sIncommingCode == "BE41629D")
  {
    // Tuner
    sendCode = 0xBE41629D;
  }
  else if (sIncommingCode == "BE41E21D")
  {
    // TAPE
    sendCode = 0xBE41E21D;
  }
  else if (sIncommingCode == "BE4102FD")
  {
    // DVD
    sendCode = 0xBE4102FD;
  }
  else if (sIncommingCode == "BE41827D")
  {
    // D-TV
    sendCode = 0xBE41827D;
  }
  else if (sIncommingCode == "BE4142BD")
  {
    // VCR
    sendCode = 0xBE4142BD;
  }
  else if (sIncommingCode == "BE4152AD")
  {
    // CBL SAT
    sendCode = 0xBE4152AD;
  }
  else if (sIncommingCode == "BE41EA15")
  {
    // SLEEP
    sendCode = 0xBE41EA15;
  }
  else if (sIncommingCode == "BE4138C7")
  {
    // Muting
    sendCode = 0xBE4138C7;
  }
  else if (sIncommingCode == "BE41FA05")
  {
    // Vol Down - 
    sendCode = 0xBE41FA05;
  }
  else if (sIncommingCode == "BE417A85")
  {
    // Vol Up +
    sendCode = 0xBE417A85;
  }
  else 
  {
    Serial.println("Unkonwn Command: ");
    serialPrintUint64(sendCode, 16);
  }



  Serial.println("Sending code: ");
  serialPrintUint64(sendCode, 16);
  
  irsend.sendNEC(sendCode, 32);

  
  Serial.println();
}

