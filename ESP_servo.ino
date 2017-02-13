#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <Servo.h>

const char* ssid = "BLACKHAWK";
const char* password = "slbiscay";

ESP8266WebServer server(80);

const int led = 13;

Servo myServo;

void setup(void){
  myServo.attach(2);
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
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

  if (MDNS.begin("buttonpusher")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", [](){
    if (server.hasArg("pos")) { // Process arg
      int pos = server.arg("pos").toInt();
      if (pos < 10 || pos > 160) {
        server.send(400, "text/plain", "POST requests to /; ?set=[10-160]");
      } else {
        myServo.write(pos);
        server.send(204);
      }
    } else { // Print form page
      server.send(200, "text/html", "<!DOCTYPE html><html><head><title>Set Servo Angle</title></head><body><form method='POST'>Angle: <input type='number' name='pos' min='10' max='160'><button type='submit'>Set</button></form></body></html>");

    }
  });

  server.onNotFound([](){
    server.send(404, "text/plain", "POST requests to /; ?set=[10-160]");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
