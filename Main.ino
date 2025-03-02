#define BLYNK_TEMPLATE_ID "-------------"
#define BLYNK_TEMPLATE_NAME "----------"
#define BLYNK_AUTH_TOKEN "------------"

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <BlynkSimpleEsp32.h>

const char* ssid = "Wifi Name";
const char* password = "Wifi Password";

const char* mqtt_server = "Server Link";
const int mqtt_port = 8883;
const char* mqtt_user = "Mqtt user name";
const char* mqtt_password = "Mqtt password";

WiFiClientSecure espClient;  
PubSubClient client(espClient);

#define DHTPIN 19
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

BlynkTimer timer;

#define LED_PIN 18
#define LM35_PIN 34        
#define RED_PIN 25         
#define GREEN_PIN 26       
#define BLUE_PIN 27       


const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n" 
"------------------------------------------------------------------"
"-----END CERTIFICATE-----\n";

void setRGBColor(bool red, bool green, bool blue) {
  digitalWrite(RED_PIN, red ? HIGH : LOW);
  digitalWrite(GREEN_PIN, green ? HIGH : LOW);
  digitalWrite(BLUE_PIN, blue ? HIGH : LOW);
}

void updateRGBLed(float temperature) {
  if (temperature < 20.0) {
    setRGBColor(false, true, false);  // Green
  } else if (temperature >= 20.0 && temperature <= 30.0) {
    setRGBColor(false, false, true);  // blue
  } else {
    setRGBColor(true, false, false);  // Red
  }
}

float readLM35Temperature() {
  int lm35_value = analogRead(LM35_PIN);             
  float lm35_temp = (lm35_value * 5 / 4095.0) * 100.0; 
  return lm35_temp;
}

void sendSensorData() {
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
 
  sensors_event_t event;
  accel.getEvent(&event);
  float vibrationX = event.acceleration.x;
  float vibrationY = event.acceleration.y;
  float vibrationZ = event.acceleration.z;

  float lm35_temp = readLM35Temperature();  
  updateRGBLed(lm35_temp);                 

  char tempStr[8], humStr[8], vibXStr[8], vibYStr[8], vibZStr[8], lm35TempStr[8];
  dtostrf(temp, 1, 2, tempStr);
  dtostrf(hum, 1, 2, humStr);
  dtostrf(vibrationX, 1, 2, vibXStr);
  dtostrf(vibrationY, 1, 2, vibYStr);
  dtostrf(vibrationZ, 1, 2, vibZStr);
  dtostrf(lm35_temp, 1, 2, lm35TempStr);

  client.publish("iot/temp", tempStr);
  client.publish("iot/hum", humStr);
  client.publish("iot/vibX", vibXStr);
  client.publish("iot/vibY", vibYStr);
  client.publish("iot/vibZ", vibZStr);
  client.publish("iot/lm35_temp", lm35TempStr);  

  Blynk.virtualWrite(V0, temp);
  Blynk.virtualWrite(V1, hum);
  Blynk.virtualWrite(V2, lm35_temp);  
  Blynk.virtualWrite(V3, vibrationX);
  Blynk.virtualWrite(V4, vibrationY);
  Blynk.virtualWrite(V5, vibrationZ);

  Serial.print("Temperature: "); Serial.println(temp);
  Serial.print("Humidity: "); Serial.println(hum);
  Serial.print("LM35 Temperature: "); Serial.println(lm35_temp);
}

BLYNK_WRITE(V6) {  
  int switchState = param.asInt();
  if (switchState == 1) {
    digitalWrite(LED_PIN, HIGH);
    Serial.print("LED ON");
    client.publish("iot/led_control", "ON");  
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.print("LED OFF");
    client.publish("iot/led_control", "OFF");    
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);  
  digitalWrite(LED_PIN, LOW);

  pinMode(RED_PIN, OUTPUT);   
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  espClient.setCACert(ca_cert);
  client.setServer(mqtt_server, mqtt_port);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  dht.begin();
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }

  reconnect();

  timer.setInterval(2000L, sendSensorData);
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("ESP32Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  Blynk.run();
  timer.run();
}