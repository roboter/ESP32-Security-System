#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoOTA.h>
#include "setup.h"
#include "sert.h"

WiFiClientSecure client;

bool previous = false;
int incomingByte = 0;
void connectWifi();
void Blink();

String createHttpPostRequest(String t_host, String t_url, String t_accessToken, String t_messagebody)
{
  String request = String("POST ") + t_url + " HTTP/1.1\r\n" +
                   "Host: " + t_host + "\r\n" +
                   "Authorization: Bearer " + t_accessToken + "\r\n" +
                   "Content-Type: application/json\r\n" +
                   "Content-Length: " + t_messagebody.length() + "\r\n\r\n" +
                   t_messagebody;
  return request;
}
void sendPushbulletHttpRequest(String t_messagebody)
{
  String post_request = createHttpPostRequest(PUSHBULLET_HOST,
                                              PUSHBULLET_PUSH_URL,
                                              PUSHBULLET_ACCESS, t_messagebody);
  // Debug mode
  Serial.print("Pusbullet HTTP request:");
  Serial.print(post_request);

  client.print(post_request);

  Serial.print("Push message sent.");
}

void Push(String text)
{
  String title = "Alarm system";
  Serial.println("\nStarting connection to server...");
  if (client.connect(server, 443))
  {
#if defined(SERIALDEBUG)
    Serial.println("Connected to server!");
#endif
  }
  else
  {
#if defined(SERIALDEBUG)
    Serial.println("Failed to connect to pushbullet.com");
#endif
    return;
  }

  String messagebody = {"{\"body\":\"" + text + "\",\"title\":\"" + title + "\",\"type\":\"note\"}"};
  sendPushbulletHttpRequest(messagebody);
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (client.available())
  {
    char c = client.read();
    Serial.write(c);
  }

  client.stop();
}
void setup()
{
  // Initialise Serial connection
#if defined(SERIALDEBUG)
  Serial.begin(115200);
#endif
  pinMode(BRAKELINE, INPUT_PULLUP);
  pinMode(LED, OUTPUT);

  // Initialise wifi connection
  connectWifi();

  ArduinoOTA.setHostname("AlarmSystemLive");
  ArduinoOTA.setPassword("ESP8266AlarmSystem");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
#if defined(SERIALDEBUG)
    Serial.println("\nEnd");
#endif
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
#if defined(SERIALDEBUG)
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
#endif
  });
  ArduinoOTA.onError([](ota_error_t error) {
#if defined(SERIALDEBUG)
    Serial.printf("Error[%u]: ", error);

    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
#endif
  });
  ArduinoOTA.begin();
  previous = digitalRead(BRAKELINE);
  Push("Boot finished");
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    connectWifi();
    Push("reconnect");
  }
  ArduinoOTA.handle();
  bool r = digitalRead(BRAKELINE);
  if (r != previous)
  {
    previous = r;
#if defined(SERIALDEBUG)
    Serial.println(r);
#endif
    Blink();
    if (r)
    {
      Push("Door opened");
      delay(1000);
    }
  }
}

void Blink()
{
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);
}

// connect to wifi – returns true if successful or false if not
void connectWifi()
{
  boolean state = true;
  int i = 0;
  WiFi.begin(ssid, password);
#if defined(SERIALDEBUG)
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting");
#endif
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
#if defined(SERIALDEBUG)
    Serial.print(".");
#endif
    if (i > 10)
    {
      state = false;
      break;
    }
    i++;
  }
#if defined(SERIALDEBUG)
  if (state)
  {

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  else
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }
#endif
  delay(1000);
}
