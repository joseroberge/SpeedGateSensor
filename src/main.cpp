#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "KinUQAR";
const char* password = "12345678";
const String server = "http://192.168.4.1/gate-crossed";
const String route = "gate-crossed";
const int LIGHT_THRESHOLD = 2000;

bool sent = false;


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

    // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// the loop function runs over and over again forever
void loop() {
  /*
   * Note: ADC2 pins cannot be used when Wi-Fi is used.
   * So, if you’re using Wi-Fi and you’re having trouble
   * getting the value from an ADC2 GPIO, you may consider
   * using an ADC1 GPIO instead, that should solve your problem.
   * https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/
   */
  int lightIntensity = analogRead(32);
  //Serial.println(lightIntensity);
  if(lightIntensity < LIGHT_THRESHOLD && !sent) {
    int t = millis();
    HTTPClient http;
    String url = server + "?time=" + t;
    http.begin (url);
    int response = http.GET();
    Serial.println(response);

    http.end();

    sent = true;
  } else if (lightIntensity >= LIGHT_THRESHOLD) {
    sent = false;  
  }
}