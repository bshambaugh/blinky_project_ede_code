
#include <WiFi.h>
#include <WebSocketsServer.h>
 
// Constants
const char* ssid = "NETGEAR09";
const char* password = "silentsky936";
const int led_pin = 15;
const char* first_name = "Marko";
 
// Globals
WebSocketsServer webSocket = WebSocketsServer(8080);
char msg_buf[10];
int led_state = 0;
 
// Called when receiving any WebSocket message
void onWebSocketEvent(uint8_t num,
                      WStype_t type,
                      uint8_t * payload,
                      size_t length) {


  // Figure out the type of WebSocket event
  switch(type) {
 
    // Client has disconnected
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
 
    // New client has connected
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connection from ", num);
        Serial.println(ip.toString());
      }
      break;
 
    // Echo text message back to client
 //   case WStype_TEXT:
  //    Serial.printf("[%u] Text: %s\n", num, payload);
  //    webSocket.sendTXT(num, payload);
  //    break;

    // Handle messages from client
    case WStype_TEXT:
 
      // Print out raw message
      Serial.printf("[%u] Received text: %s\n", num, payload);
 
      // Toggle LED
      if ( strcmp((char *)payload, "toggleLED") == 0 ) {
        led_state = led_state ? 0 : 1;
        Serial.printf("Toggling LED to %u\n", led_state);
        digitalWrite(led_pin, led_state);
 
      // Report the state of the LED
      } else if ( strcmp((char *)payload, "getLEDState") == 0 ) {
        sprintf(msg_buf, "%d", led_state);
        // the LED state is saved in the buffer...so there is no way of knowing whether the circuit is connected
        Serial.printf("Sending to [%u]: %s\n", num, msg_buf);
        webSocket.sendTXT(num, msg_buf);
        // webSocket.sendTXT(num, first_name)
 
      // Message not recognized
      } else {
        Serial.println("[%u] Message not recognized");
      }
      break;
 
    // For everything else: do nothing
    case WStype_BIN:
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    default:
      break;
  }
}
 
void setup() {
  // Init LED and turn off
  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
 
  // Start Serial port
  Serial.begin(115200);
 
  // Connect to access point
  // DO I need the code in this block??
  Serial.println("Connecting");
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }
 
  // Print our IP address
  Serial.println("Connected!");
  Serial.print("My IP address: ");
  Serial.println(WiFi.localIP());
 
  // Start WebSocket server and assign callback
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
}
 
void loop() {
 
  // Look for and handle WebSocket data
  webSocket.loop();
}
