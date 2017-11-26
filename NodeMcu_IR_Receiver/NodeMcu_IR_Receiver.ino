#include "ESP8266WiFi.h"
#include <PubSubClient.h>
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>

WiFiClient espClient;
PubSubClient client(espClient);

// WiFi parameters
const char* ssid = "";
const char* password = "";
const char* mqtt_server = "";


const char* outTopicIRCode = "IR/Code";


uint16_t RECV_PIN = 14; // Pin D5

IRrecv irrecv(RECV_PIN);

decode_results results;  // Somewhere to store the results
irparams_t save;         // A place to copy the interrupt state while decoding.



void setup() {
  Serial.begin(115200, SERIAL_8N1, SERIAL_TX_ONLY);
  Serial.print("Serial Begin");

  Serial.println("Setup WiFi");
  setup_wifi();

  Serial.println("Setup Client");
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  irrecv.enableIRIn();  // Start the receiver
  
  Serial.println("Setup done");
}

void loop() 
{

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (irrecv.decode(&results, &save)) {
    String sIncommingCode = getSignalAsString(&results);
    client.publish(outTopicIRCode, (char*)sIncommingCode.c_str());
    delay(25);
    irrecv.resume(); // Receive the next value
  }
  
}


String getSignalAsString(decode_results *results)
{
  
  char buf[8 * sizeof(results->value) + 1];  // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];
  uint8_t base = 16;
  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    char c = results->value % base;
    results->value /= base;

    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while (results->value);

  
  Serial.println(str);
  String sIncommingCode(str);

  return (sIncommingCode);
}


void setup_wifi() {

  delay(50);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.hostname("NodeMCU_Keller_IR_Receiver");
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
    if (client.connect("NodeMCU_Keller_IR_Receiver")) {
      Serial.println("connected");
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
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

