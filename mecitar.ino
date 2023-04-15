#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char* ssid = "your Hotspot name";
const char* password = "your password";

ESP8266WebServer server(80);

const int relay1 = 12;
const int relay2 = 13;
const int relay3 = 4;
const int relay4 = 5;

// Set your Static IP address
IPAddress staticIP(192, 168, 0, 150);
// Set your Gateway IP address
IPAddress gateway(192, 168, 0, 1);

IPAddress subnet(255, 255, 0, 0);

// Timer for Relay 3
unsigned long timerRelay3 = 0;
const int relay3Interval = 5000000; // 1 Jam
void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  
  Serial.begin(115200);
  WiFi.config(staticIP, gateway, subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", [](){
    String html = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=0' >"
                  
                  "<style>"
                    ".switch {"
                      "position: relative;"
                      "display: inline-block;"
                      "width: 60px;"
                      "height: 34px;"
                    "}"
                    ".switch input {" 
                      "opacity: 0;"
                      "width: 0;"
                      "height: 0;"
                    "}"
                    ".slider {"
                      "position: absolute;"
                      "cursor: pointer;"
                      "top: 0;"
                      "left: 0;"
                      "right: 0;"
                      "bottom: 0;"
                      "background-color: #ccc;"
                      "transition: .4s;"
                    "}"
                    ".slider:before {"
                      "position: absolute;"
                      "content: '';"
                      "height: 26px;"
                      "width: 26px;"
                      "left: 4px;"
                      "bottom: 4px;"
                      "background-color: white;"
                      "transition: .4s;"
                    "}"
                    "input:checked + .slider {"
                      "background-color: #2196F3;"
                    "}"
                    "input:focus + .slider {"
                      "box-shadow: 0 0 1px #2196F3;"
                    "}"
                    "input:checked + .slider:before {"
                      "transform: translateX(26px);"
                    "}"
                    ".slider.round {"
                      "border-radius: 34px;"
                    "}"
                    ".slider.round:before {"
                      "border-radius: 50%;"
                    "}"
                  "</style>"
                  "</head><body>"
                  "<div style='text-align:center' class='container'>"
                  "<h2 style='text-align:center'>ARABELLA'S HOME AUTOMATION</h2>"
                  "<div>"
                  "<img src='https://drive.google.com/uc?export=view&id=1C9nAF2VWQ09gZG_Ft663gLScSaNE_kml' width='200'>"
                  "</div>"
                  "<br>"
                  "<div class='row'>"
                  "<div class='col-md-3'>"
                  "<h3>LAMPU RUANG TAMU</h3>"
                  "<label class='switch'>"
                  "<input type='checkbox' id='r1' onclick='checkRelay(1)'>"
                  "<span class='slider round'></span>"
                  "</label>"
                  "</div>"
                  "<div class='col-md-3'>"
                  "<h3>KIPAS RUANG TAMU</h3>"
                  "<label class='switch'>"
                  "<input type='checkbox' id='r2' onclick='checkRelay(2)'>"
                  "<span class='slider round'></span>"
                  "</label>"
                  "</div>"
                  "<div class='col-md-3'>"
                  "<h3>POMPA AIR</h3>"
                  "<label class='switch'>"
                  "<input type='checkbox' id='r3' onclick='checkRelay(3)'>"
                  "<span class='slider round'></span>"
                  "</label>"
                  "</div>"
                  "<div class='col-md-3'>"
                  "<h3>LAMPU RUANG TENGAH</h3>"
                  "<label class='switch'>"
                  "<input type='checkbox' id='r4' onclick='checkRelay(4)'>"
                  "<span class='slider round'></span>"
                  "</label>"
                  "</div>"
                  "</div>"
                  "<br>"
                  
                  "</div>"
                  "<script>"
                  "function checkRelay(relay) {"
                    "var xhttp = new XMLHttpRequest();"
                    "if(document.getElementById('r' + relay).checked) {"
                      "xhttp.open('GET', '/relay' + relay + '/1', true);"
                    "} else {"
                      "xhttp.open('GET', '/relay' + relay + '/0', true);"
                    "}"
                    "xhttp.send();"
                  "}"
                  "</script>"
                  "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/relay1/1", [](){
    digitalWrite(relay1, LOW);
    server.send(200, "text/plain", "Relay 1 is ON");
  });

  server.on("/relay1/0", [](){
    digitalWrite(relay1, HIGH);
    server.send(200, "text/plain", "Relay 1 is OFF");
  });

  server.on("/relay2/1", [](){
    digitalWrite(relay2, LOW);
    server.send(200, "text/plain", "Relay 2 is ON");
  });

  server.on("/relay2/0", [](){
    digitalWrite(relay2, HIGH);
    server.send(200, "text/plain", "Relay 2 is OFF");
  });

  server.on("/relay3/1", [](){
    digitalWrite(relay3, LOW);
    timerRelay3 = millis();
    server.send(200, "text/plain", "Relay 3 is ON");
  });

  server.on("/relay3/0", [](){
    digitalWrite(relay3, HIGH);
    server.send(200, "text/plain", "Relay 3 is OFF");
  });

  server.on("/relay4/1", [](){
    digitalWrite(relay4, LOW);
    server.send(200, "text/plain", "Relay 4 is ON");
  });

  server.on("/relay4/0", [](){
    digitalWrite(relay4, HIGH);
    server.send(200, "text/plain", "Relay 4 is OFF");
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
  // Timer for Relay 3
if (timerRelay3 != 0 && millis() - timerRelay3 >= relay3Interval) {
digitalWrite(relay3, HIGH);
timerRelay3 = 0;
}
}
