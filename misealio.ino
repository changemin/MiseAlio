#include<Adafruit_NeoPixel.h>
#include<ArduinoJson.h>
#include<WiFi.h>
#include<HTTPClient.h>

#define ssid "USER_SSID"
#define password "PASSWORD"

#define pixelNum 0
#define pixelPin 0

#define APIkey ""
#define CITY "강동구"
#define VERSION "1.3"

const char server[] = "openapi.airkorea.or.kr";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNum, pixelPin,NEO_GRB+NEO_KHZ800);

StaticJsonBuffer<11746> jsonBuffer;

WiFiClient client;

void setup()
{
    Serial.begin(115200);
    strip.begin();
    strip.show();
    strip_init();

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
        BlinkNotificator(255,255,0);
    }
    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("[Setup]WiFi connected!");
        BlinkNotificator(0,255,0);
    }
    else 
    {
        Serial.println("[Setup]WiFi not connected!");
        BlinkNotificator(255,0,0);
    }
}

void loop()
{

}

void strip_init()
{
    SetStripColor(30, 255, 255, 255);
    SetStripColor(30, 255, 0, 0);
    SetStripColor(30, 0, 255, 0);
    SetStripColor(30, 0, 0, 255);
}

void SetStripColor(int interval, int R, int G, int B)
{
    for(int i = 0; i < pixelNum; i++)
    {
        strip.setPixelColor(i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void ColorCircle(int interval, int R, int G, int B)
{
    for(int i = 0; i < pixelNum; i++)
    {
        strip.setPixelColor(pixelNum%i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void BlinkNotificator(int R, int G, int B)
{
    for(int i = 0 ; i < 2; i ++)
    {
        SetStripColor(0, R, G, B);
        delay(200);
        SetStripColor(0, 0, 0, 0);
        delay(200);
    }
}

void httpsRequest()
{
    Serial.println("[httpsRequest]Function Activated");

    client.stop(); //이전의 연결을 끊음

    if (client.connect(server, 80)) 
    {
        Serial.println("[httpsRequest]Connecting...");
        client.print(F("GET /openapi/services/rest/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?stationName="));
        client.print(CITY);
        client.print(F("&dataTerm=daily&pageNo=1&numOfRows=10&ServiceKey="));
        client.print(APIkey);
        client.print(F("&ver="));
        client.print(VERSION);
        client.print(F(" HTTP/1.1\r\n"));
        client.print(F("Host: openapi.airkorea.or.kr\r\n"));
        client.print(F("Connection: close\r\n"));
        client.print(F("\r\n\r\n"));
    }
    else {
        Serial.println("[HTTP Request]Connection failed");
    }
}

int pm10GradeParse() //html태그 기반
{
    while(client.available())
    {
        Serial.println("[pm10GradeParse]Function Activated");
        String line = client.readStringUntil('\n');
        int indexOfTarget_back = line.indexOf("</pm10Grade>"); 
        if(indexOfTarget_back>0){
            String target_front="<pm10Grade>";
            String pm10Grade = line.substring(line.indexOf(target_front)+target_font.length(),indexOfTarget_back);
            Serial.print("PM10 Grade: ");
            Serial.println(pm10Grade);
            client.stop();
            return (int)pm10Grade;
        }
    }
}