#include "WiFi.h"
#include "DHT.h"
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define DHTPIN 4       // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11  // DHT 11
#define TURBIDITY_PIN 34

// for ph values

const int potPin = 35;
float ph;
float Value = 0;

// relay ports
int LIGHT_PORT = 26;  // ln1 is light
int FAN_PORT = 33;    // ln3 is fan
int PUMP_PORT = 25;   // ln2x is pump

// for ultrasonic
/* Define the names for ESP32 pin for HC-SR04*/
#define trigger_pin 5
#define Echo_pin 18
#define LED 2
/* two variables to store duraion and distance value */
long duration;
int distance;

// values fir ph
#define TdsSensorPin 27
#define VREF 3.3  // analog reference voltage(Volt) of the ADC
#define SCOUNT 30

int analogBuffer[SCOUNT];  // store the analog value in the array, read from ADC
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float _temperature = 25;

// median filtering algorithm
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}



WebServer server(5000);  // connecting webserver on port 80

const char *ssid = "tatendaZw";
const char *pass = "12345677";
IPAddress ip;

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

// sever values or server endpoints
String host = "http://192.168.198.127/api/";
String light_url = "light_value";
String URL = host + light_url;

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
  ip = WiFi.localIP();
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
  connect_to_hotspot();
  pinMode(trigger_pin, OUTPUT);  // configure the trigger_pin(D9) as an Output
  // pinMode(LED, OUTPUT);          // Set the LED (D13) pin as a digital output
  pinMode(Echo_pin, INPUT);  // configure the Echo_pin(D11) as an Input
  pinMode(LIGHT_PORT, OUTPUT);
  pinMode(FAN_PORT, OUTPUT);
  pinMode(PUMP_PORT, OUTPUT);

  // setup ph
  pinMode(potPin, INPUT);

  // setuop realy ports
  digitalWrite(LIGHT_PORT, HIGH);  // set  relay to off on start
  digitalWrite(FAN_PORT, HIGH);    // set  relay to off on start
  digitalWrite(PUMP_PORT, HIGH);   // set  relay to off on start

  // setup form light relay
  server.on("/relay_on", handle_relay_on);
  server.on("/relay_off", handle_relay_off);

  //setup for fan
  server.on("/fan_on", handle_fan_on);
  server.on("/fan_off", handle_fan_off);

  // get ip address
  server.on("/send_ip", handle_send_ip);

    // setup for pump
    server.on("/pump_on", handle_pump_on);
  server.on("/pump_off", handle_pump_off);

  pinMode(TdsSensorPin, INPUT);
  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);
  server.begin();
  delay(1000);
}

void handle_send_ip() {
  server.send(200, "text/plain", "ip");
  delay(1000);
}

void handle_relay_on() {
  digitalWrite(LIGHT_PORT, LOW);
  server.send(200, "text/plain", "Relay turned on");
  delay(1000);
}

void handle_relay_off() {
  digitalWrite(LIGHT_PORT, HIGH);
  server.send(200, "text/plain", "Relay turned off");
  delay(1000);
}


// torn fan on and off
void handle_fan_on() {
  digitalWrite(FAN_PORT, LOW);
  server.send(200, "text/plain", "fan turned on");
  delay(1000);
}
void handle_fan_off() {
  digitalWrite(FAN_PORT, HIGH);
  server.send(200, "text/plain", "fan turned off");
  delay(1000);
}

// torn pump on and off
void handle_pump_on() {
  digitalWrite(PUMP_PORT, LOW);
  server.send(200, "text/plain", "Pump turned on");
  delay(1000);
}
void handle_pump_off() {
  digitalWrite(PUMP_PORT, HIGH);
  server.send(200, "text/plain", "Pump turned off");
  delay(1000);
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>ESP32 Hello World</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\n";
  ptr += "p {font-size: 24px;color: #444444;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<div id=\"webpage\">\n";
  ptr += "<h1>Hello World !!</h1>\n";
  ptr += "</div>\n";
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}

void show_ph() {
  Value = analogRead(potPin);
  float voltage = Value * (3.3 / 4095.0);
  ph = (3.3 * voltage);
  Serial.println(ph);
  delay(500);
}

void handle_OnConnect() {
  server.send(200, "text/html", SendHTML());
}

void loop() {
  // Wait a few seconds between measurements.
  delay(1000);
  // setup items for sending to sercver -- diff variables for diff items
  server.handleClient();

  show_ph();
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


  int sensorValue = analogRead(TURBIDITY_PIN);   // read the input on analog pin 0:
  float voltage = sensorValue * (5.0 / 1024.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):

  HTTPClient http;
  http.begin("https://hydroponics-server.onrender.com/values");  //Specify destination for HTTP request
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
  root["ph_value"] = ph;
  root["humidity_val"] = h;
  root["distance_val"] = distance;
  root["quality_val"] = voltage;
  root["ip_address"] = ip;
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