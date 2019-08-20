

# Hardware
ESP32 NodeMCU Module
Button1 BUILD IN connected to GPIO2
LED_BUILTIN is also a BUZZER 
KEY ON ESP Board = KEY_BUILTIN = 0

In Arduino IDE Board Node32S
In platformio board = esp32dev

BrakeLine - PIN 22 - reed switch
# Software
rely on [pushbutton.com](http://pushbutton.com) service, 
need to create an account and get API key
# Configuration
```
// wifi connection variables
const char *ssid = "YOUR WIFI NAME";
const char *password = "PASS TO WIFI";

const String PUSHBULLET_ACCESS = "YOUR PUSHBUTTON TOKEN";
```