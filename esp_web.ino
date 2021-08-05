#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WebSocketsServer.h>
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<script>
var connection = new WebSocket('ws://'+location.hostname+':81/');
var button_1_status = 0;
var button_2_status = 0;
var button_3_status = 0;
var button_4_status = 0;
var button_5_status = 0;

function erase()
{
  button_1_status = 0;
  button_2_status = 0;
  button_3_status = 0;
  button_4_status = 0;
  button_5_status = 0;
}
function button_1_on()
{
  erase();
  button_1_status = 1; 
  console.log("Button 1 is ON");
  send_data();
}

function button_2_on()
{
  erase();
  button_2_status = 1;
  console.log("Button 2 is ON");
  send_data();
}

function button_3_on()
{
  erase();
  button_3_status = 1;
  console.log("Button 3 is ON");
  send_data();
}

function button_4_on()
{
  erase();
  button_4_status = 1;
  console.log("Button 4 is ON");
  send_data();
}

function button_5_on()
{
  erase();
  button_5_status = 1; 
  console.log("Button 5 is ON");
  send_data();
}

function send_data()
{
  var full_data = '{"Forward" :'+button_1_status+',"Right":'+button_2_status+',"Stop" :'+button_3_status+',"Left" :'+button_4_status+',"Backward" :'+button_5_status+'}';
  connection.send(full_data);
}
</script>
<body>
<center>
<h1>Control Movement</h1>
<h3> Forward </h3>
<button onclick= "button_1_on()" >Forward</button>
<h3> Right </h3>
<button onclick="button_2_on()">Right</button>
<h3> Stop </h3>
<button onclick="button_3_on()">Stop</button>
<h3> Left </h3>
<button onclick="button_4_on()">Left</button>
<h3> Backward </h3>
<button onclick="button_5_on()">Backward</button>
</center>
</body>
</html>

)=====";

//ipaddress/button/on
//ipaddress/button/off



AsyncWebServer server(80); // server port 80
WebSocketsServer websockets(81);

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Page Not found");
}


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

  switch (type) 
  {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = websockets.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        websockets.sendTXT(num, "Connected from server");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);
      String message = String((char*)( payload));
      Serial.println(message);

      
     DynamicJsonDocument doc(200);
    // deserialize the data
    DeserializationError error = deserializeJson(doc, message);
    // parse the parameters we expect to receive (TO-DO: error handling)
      // Test if parsing succeeds.
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  int Forward_status = doc["Forward"];
  int Right_status = doc["Right"];
  int Stop_status = doc["Stop"];
  int Left_status = doc["Left"];
  int Backward_status = doc["Backward"];

  if (Forward_status == 1)
  {Serial.print("");
  Serial.print("Move Forward");}

  if (Right_status == 1)
 { Serial.print("");
  Serial.print("Move Right");}

  if (Left_status == 1)
  {Serial.print("");
  Serial.print("Move Left");}

  if (Backward_status == 1)
 { Serial.print("");
  Serial.print("Move Backward");}
  
  if (Stop_status == 1)
  {Serial.print("");
  Serial.print("Stop");}
  }
}

void setup(void)
{
  
  Serial.begin(115200);
  
  WiFi.softAP("ESP Smart Method Yaser", "");
  Serial.println("softap");
  Serial.println("");
  Serial.println(WiFi.softAPIP());


  if (MDNS.begin("ESP")) { //esp.local/
    Serial.println("MDNS responder started");
  }



  server.on("/", [](AsyncWebServerRequest * request)
  { 
   
  request->send_P(200, "text/html", webpage);
  });

 

  server.onNotFound(notFound);

  server.begin();  // it will start webserver
  websockets.begin();
  websockets.onEvent(webSocketEvent);

}


void loop(void)
{
 websockets.loop();
}
