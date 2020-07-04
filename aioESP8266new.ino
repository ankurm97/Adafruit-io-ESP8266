#include<ESP8266WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>


#define Relay0 1

#define WLAN_SSID "AndroidAP"
#define WLAN_PASS "password"

#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883
#define AIO_USERNAME "ankurm97"
#define AIO_key "f3860dd90837416ca7a41eaba888c691"

WiFiClient client; 
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_key);
Adafruit_MQTT_Subscribe Light0 = Adafruit_MQTT_Subscribe(&mqtt,AIO_USERNAME"/feeds/Relay0");

void MQTT_Connect();

void setup() {
  Serial.begin(115200); 
  pinMode(Relay0, OUTPUT);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());
  mqtt.subscribe(&Light0);
}
void loop() {
  MQTT_Connect();
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
     if (subscription == &Light0) {
      Serial.print(F("Got: "));
      Serial.println((char *)Light0.lastread);
      int Light0_State = atoi((char *)Light0.lastread);
      digitalWrite(Relay0, Light0_State);
    }
  }  
}
void MQTT_Connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000); 
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
