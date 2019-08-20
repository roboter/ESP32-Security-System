#include <Arduino.h>
const char *server = "api.pushbullet.com"; // Server URL
const String PUSHBULLET_HOST = "api.pushbullet.com";
const String PUSHBULLET_PUSH_URL = "/v2/pushes";
const String PUSHBULLET_ACCESS = "YOUR PUSHBUTTON TOKEN";

#define SERIALDEBUG
#define BRAKELINE 22
#define LED 2

#define PORT 443

// wifi connection variables
const char *ssid = "YOUR WIFI NAME";
const char *password = "PASS TO WIFI";
