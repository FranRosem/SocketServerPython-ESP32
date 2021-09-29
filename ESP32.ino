#include <WiFi.h>

const char* WIFI_SSID = "YOUR WIFI";
const char* WIFI_PASS = "YOUR PASSWORD";

const char* SERVER_ADDRESS = "YOUR IP V4";
const int SERVER_PORT = 8080;

const int TRIGGER_PIN = 26;
const int ECHO_PIN = 27;
const int LED_GREEN = 13;
const int LED_YELLOW = 12;
const int LED_RED = 14;

String line;
int inches, cm;


long readUltrasonicDistance(int triggerPin, int echoPin) {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  return 0.01723 * pulseIn(echoPin, HIGH);
}
void turnOnLed(int ledPin) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
}
void turnOffLed(int ledPin) {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}
void distanceOfLeds(int cm) {
  if (cm < 10) {
        turnOnLed(LED_GREEN);
        turnOffLed(LED_YELLOW);
        turnOffLed(LED_RED);
      } else if (cm < 40) {
        turnOffLed(LED_GREEN);
        turnOnLed(LED_YELLOW);
        turnOffLed(LED_RED);
      } else {
        turnOffLed(LED_GREEN);
        turnOffLed(LED_YELLOW);
        turnOnLed(LED_RED);
      }
}

void setup() {
  Serial.begin(115200);

  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while ( WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.print("\nLocal IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;
  Serial.print("Connecting to: ");
  Serial.println(SERVER_ADDRESS);
  cm = readUltrasonicDistance(TRIGGER_PIN, ECHO_PIN);
  
  if (!client.connect(SERVER_ADDRESS, SERVER_PORT)) {
    Serial.println("Connection failed");
    //delay(5000);
    return;
  }
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 10000) {
      Serial.println("Server didn't answer");
      client.stop();
      return;
    }
    delay(10);
  }

  if (client.available() > 0) {
    line = client.readStringUntil('\n');
  }
  if (line == "D") {
    Serial.println(line);
    distanceOfLeds(cm);
    client.printf("Distance: %i cm.", cm);
  }
  else {
    client.println("Incorrect line!");
  }
  
  Serial.println("Closing Connection");
  client.stop();
}