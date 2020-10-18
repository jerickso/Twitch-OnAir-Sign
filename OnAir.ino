#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#include "arduino_secrets.h"        // Where personalized items are configured
 
const char* ssid     = SECRET_SSID; // From arduino_secrets.h
const char* password = SECRET_PASS; // From arduino_secrets.h


// String received back from authentication login that we need to parse for
const char *accesstoken = "\"access_token\":\"";

#define HOST "api.twitch.tv"      // The host to query if live
#define PATH "/helix/streams"     // Path on the host
#define REFRESH 60                // How many seconds to wait between checks to see if stream is live?
#define LED 13                    // Pin LEDs power is connected to, can add digitalWrite(LED, LOW)
#define PIN 12                    // Arduino Pin number that communicates with the LEDs


// # of pixels in Strip
// Arduino Communication Pin #
// GRB and 800KHz bitstream
Adafruit_NeoPixel strip = Adafruit_NeoPixel(15, PIN, NEO_GRB + NEO_KHZ800);

WiFiClientSecure client;  //
char bearer[32];  // Authentication code, it is really only 30 char long but allows NULL termination + 1 extra


boolean isStreaming = false;      // Is the user detected as streaming?


// Set all LEDs to the same color and brightness
// c - The color
// brightness - The brightness value between 0-255
void colorSet(uint32_t c,  uint32_t brightness=255) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.setBrightness(brightness);
  strip.show();
}


// Get our Authentication code from the Twitch API
// Note that token is only valid for upto 60 days
void get_bearer() {
  Serial.println("Attempting to get authentication token");
  if (!client.connect("id.twitch.tv", 443)) {
    Serial.println("Connection failed to get bearer authentication token");
    // Zero out the bearer variable
    for(int i=0; i < sizeof(bearer); i++) {
      bearer[i] = 0;
    }
    return;
  }
  client.print(String("POST /oauth2/token?client_id=") + CLIENT_ID + "&client_secret=" + SECRET + "&grant_type=client_credentials HTTP/1.1\r\n" +
           "Host: id.twitch.tv\r\n" + 
           "Connection: close\r\n\r\n");
  String resp = client.readString();
  Serial.print("Response: ");
  Serial.println(resp);

  // Grab the token in the response
  int offset = resp.indexOf(accesstoken) + strlen(accesstoken);
  int offset2 = resp.indexOf("\"", offset);
  resp.substring(offset, offset2).toCharArray(bearer, offset2-offset+1);

  Serial.print("Bearer code: ");
  Serial.println(bearer);

  client.stop();
}

void setup() {
  client.setInsecure();   // We do not need to validate the SSL certificate thumbprints
  strip.begin();
  strip.show();           // Initialize all pixels to off
  
  Serial.begin(115200);
  delay(100);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    colorSet(strip.Color(0, 0, 10));     // We are Blue while we are connecting to the wireless network
    while (WiFi.status() != WL_CONNECTED) {
      Serial.println();
      Serial.println();
      Serial.println("Not connected");
      Serial.print("Attempting to connect to wireless network: ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);
      
      int j = 0;
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        j++;
        if (j > 10) break;
      }
      if (j <= 10) break;
    }
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    colorSet(strip.Color(0, 10, 10));   // Green/Blue - We are connected to the wireless network
    get_bearer();
  }
  else if (bearer[0] == NULL) {
    get_bearer();
  }
  else {
    Serial.print("Connecting to: ");
    Serial.println(HOST);
  
    if (!client.connect(HOST, 443)) {
      Serial.println("Connection error");
      delay(REFRESH * 1000);
      return;
    }

    client.print(String("GET ") + PATH + "?user_login=" + USERNAME + " HTTP/1.1\r\n" +
                 "Client-Id: " + CLIENT_ID + "\r\n" +
                 "Host: " + HOST + "\r\n" +
                 "Authorization: Bearer " + bearer + "\r\n" +
                 "Connection: close\r\n\r\n");

    int32_t timeout = millis() + 1000;
    while (client.available() == 0) { 
      if (timeout - millis() < 0) { 
        Serial.println(">>> Client Timeout !"); 
        client.stop(); 
        return;
      }
    }
    String resp = client.readString();
    Serial.print("Response: ");
    Serial.println(resp);


    // If authentication failed, reauthenticate
    if (resp.indexOf("\"error\":") != -1) {
      if (resp.indexOf("\"Unauthorized\"") != -1) {
        if(resp.indexOf("401") != -1) {
          Serial.println("Refreshing login token");
          get_bearer();
          return;
        }
      }
    }

    isStreaming = false;

    // If the string "type": is in the stream, then assume stream is live
    if (resp.indexOf("\"type\":") != -1) {
      isStreaming = true;
    }

    Serial.print("Streaming status: "); Serial.println(isStreaming);
    if (isStreaming) {
      colorSet(strip.Color(255, 0, 0));     // Red
    } else {
      colorSet(strip.Color(10, 10, 10), 32);    // White
    }
    Serial.println("Disconnecting from server.");
    client.stop();
  }

  delay(REFRESH * 1000);
}
