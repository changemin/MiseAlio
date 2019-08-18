#include<Adafruit_NeoPixel.h>
#include<ArduinoJson.h>
#include<WiFi.h>
#include<HTTPClient.h>

#define pixelNum 0
#define pixelPin 0
#define APIkey ""
#define ssid "USER_SSID"
#define password "PASSWORD"

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNum, pixelPin,NEO_GRB+NEO_KHZ800);
StaticJsonBuffer<11746> jsonBuffer;
WiFiClient client;

void setup(){
    Serial.begin(115200);
    strip.begin();
    strip.show();
    strip_init();

    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
        BlinkNotificator(255,255,0);
    }
    if(WiFi.status() == WL_CONNECTED){
        Serial.println("[Setup]WiFi connected!");
        BlinkNotificator(0,255,0);
    }
    else {
        Serial.println("[Setup]WiFi not connected!");
        BlinkNotificator(255,0,0);
    }
}

void loop(){

}

void strip_init(){
    SetStripColor(30, 255, 255, 255);
    SetStripColor(30, 255, 0, 0);
    SetStripColor(30, 0, 255, 0);
    SetStripColor(30, 0, 0, 255);
}

void SetStripColor(int interval, int R, int G, int B){
    for(int i = 0; i < pixelNum; i++){
        strip.setPixelColor(i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void ColorCircle(int interval, int R, int G, int B){
    for(int i = 0; i < pixelNum; i++){
        strip.setPixelColor(pixelNum%i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void BlinkNotificator(int R, int G, int B){
    for(int i = 0 ; i < 2; i ++){
        SetStripColor(0, R, G, B);
        delay(200);
        SetStripColor(0, 0, 0, 0);
        delay(200);
    }
}

int httpsRequest(){
    Serial.println("[httpsRequest]Function Activated");
}