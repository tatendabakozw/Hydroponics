#include "WiFi.h"
#include "DHT.h"
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define DHTPIN 4       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
#define TURBIDITY_PIN A3

// for ultrasonic
/* Define the names for ESP32 pin for HC-SR04*/
#define trigger_pin 5 // trigger pin
#define Echo_pin 18 // echo pin

/* two variables to store duraion and distance value */
long duration;
int distance;

// configutring web client module
WiFiClient client;
WebServer server(80);  // connecting webserver on port 80

// configuring hotspot values
const char *ssid = "**";
const char *pass = "**";

// VARIABLES FOR TURBIIUTY SENSOR
int adcValue;
float voltValue;

int sensorVal;
#define LIGHT_SENSOR_PIN 36  // or A0
const int RESOLUTION = 12;   // Could be 9-12

// defining code variables
float temperature;
float humidity;
float light_value;
int pH_value;
int distance_clearance;

// Initializing the DHT11 sensor.
DHT dht(DHTPIN, DHTTYPE);


// function to connect eps32 to hotspot
void connect_to_hotspot() {
  WiFi.begin(ssid, pass);  //Connect to WiFi

  // run loop below while trying to connect to hotspot
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// funsiton to get disatance cleatrajce from ultrasonic
int get_Distance() {
  digitalWrite(trigger_pin, LOW);  //set trigger signal low for 2us
  delayMicroseconds(2);

  /*send 10 microsecond pulse to trigger pin of HC-SR04 */
  digitalWrite(trigger_pin, HIGH);  // make trigger pin active high
  delayMicroseconds(10);            // wait for 10 microseconds
  digitalWrite(trigger_pin, LOW);   // make trigger pin active low

  /*Measure the Echo output signal duration or pulss width */
  duration = pulseIn(Echo_pin, HIGH);  // save time duration value in "duration variable
  distance = duration * 0.034 / 2;     //Convert pulse duration into distance
  delay(1000);
  return distance;
}


void setup() {
  Serial.begin(115200);
  pinMode(trigger_pin, OUTPUT);  // configure the trigger_pin(D9) as an Output
  pinMode(LED, OUTPUT);          // Set the LED (D13) pin as a digital output
  pinMode(Echo_pin, INPUT);      // configure the Echo_pin(D11) as an Input
  delay(1000);
  connect_to_hotspot();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  // setup items for sending to sercver -- diff variables for diff items

  get_Distance();

  // read values from dh11
  // Reading temperature or humidity takes about 250 milliseconds!
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  // int sensorValue = analogRead(TURBIDITY_PIN);// read the input on analog pin 0:
  // float voltage = sensorValue * (5.0 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

  HTTPClient http;
  http.begin("http://<server-ip-address>/values");  //Specify destination for HTTP request
  // set headers
  http.addHeader("Content-Type", "application/json");

  // prepare json variable
  DynamicJsonDocument doc(512);
  JsonObject root = doc.to<JsonObject>();

  //Read and print the sensor pin value
  sensorVal = analogRead(LIGHT_SENSOR_PIN);

  // prepare json object before sending
  root["light_val"] = sensorVal;
  root["temp_val"] = t;
  root["humidity_val"] = h;
  root["distance_val"] = distance;
  root["quality_val"] = voltValue;
  String json;
  serializeJsonPretty(root, json);


  //Send the actual POST request
  int httpResponseCode = http.POST(json);
  // We'll have a few threshholds, qualitatively determined

  // check if api request has an error or not
  if (httpResponseCode > 0) {
    String response = http.getString();  //Get the response to the request

    Serial.println(httpResponseCode);  //Print return code
    Serial.println(response);          //Print request answer

  } else {

    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();  //Free resources
  // sleep for some time before next read
  delay(2000);
}