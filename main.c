#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <UrlEncode.h>


int temp = 0;
int vib_pin = D1;
int flag = 0;
int buzzer = D2;
int counter;
SoftwareSerial GPS_SoftSerial(D4, D3);
TinyGPSPlus gps;
const char *ssid = "Wifi enduku ra";
const char *password = "check123";
String phoneNumber = "+919700662454";
String apiKey = "5371501";
void sendMessage(String message)
{
    String url = "http://api.callmebot.com/whatsapp.php?phone=" + phoneNumber + "&apikey=" + apiKey + "&text=" +
                 urlEncode(message);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpResponseCode = http.POST(url);

    if (httpResponseCode == 200)
    {
        Serial.print("Message sent successfully");
    }
    else
    {
        Serial.println("Error sending the message");
        Serial.print("HTTP response code: ");
        Serial.println(httpResponseCode);
    }
    http.end();
}
void setup()
{
    pinMode(vib_pin, INPUT);
    pinMode(buzzer, OUTPUT);
    Serial.begin(9600);
    Serial.println("start");
    GPS_SoftSerial.begin(9600);
    Serial.println("end");
    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wifi");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");

    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());
}
void loop()
{
    int val;
    val = digitalRead(vib_pin);
    if (digitalRead(vib_pin) == 1)
    {
        Serial.println("Collision Detected !! Fetching location...");
        digitalWrite(buzzer, HIGH);
        while (flag == 0)
        {
            unsigned long start = millis();
            do
            {
                while (GPS_SoftSerial.available())
                    gps.encode(GPS_SoftSerial.read());
            } while (millis() - start < 1000);
            double lat_val, lng_val, alt_m_val;
            uint8_t hr_val, min_val, sec_val;
            bool loc_valid, alt_valid, time_valid;
            lat_val = gps.location.lat();
            loc_valid = gps.location.isValid();
            lng_val = gps.location.lng();
            if (!loc_valid)
            {

                Serial.println("Location not identified yet...");
                delay(1000);
            }
            else
            {
                flag = 1;
                Serial.println("Accident detected !!");
                Serial.print("Latitude in Decimal Degrees : ");
                Serial.println(lat_val, 6);
                Serial.print("Longitude in Decimal Degrees : ");
                Serial.println(lng_val, 6);
Serial.println("Press Reset button in under 10 seconds to cancel sending 
message...");
counter = 10;
digitalWrite(buzzer,HIGH);
while (counter > 0)
{
                    Serial.println(counter);
                    delay(1000);
                    counter = counter - 1;
}
digitalWrite(buzzer,LOW);
Serial.print("Sending message....");
sendMessage("Accident Detected !! Location: 
https://maps.google.com/?q="+String(lat_val,6)+"," + String(lng_val,6));
            }
        }
        flag = 0;
    }
