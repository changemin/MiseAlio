#include<Adafruit_NeoPixel.h>
#include<ArduinoJson.h>
#include<WiFi.h>
#include<HTTPClient.h>

#define ssid "USER_SSID"
#define password "PASSWORD"

#define pixelNum 0
#define pixelPin 0
#define requestInterval 10

#define APIkey ""
#define CITY "강동구"
#define VERSION "1.3"

const char server[] = "openapi.airkorea.or.kr";

Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelNum, pixelPin,NEO_GRB+NEO_KHZ800);

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
        blinkNotificator(255,255,0);
    }
    if(WiFi.status() == WL_CONNECTED)
    {
        Serial.println("[Setup]WiFi connected!");
        blinkNotificator(0,255,0);
    }
    else 
    {
        Serial.println("[Setup]WiFi not connected!");
        blinkNotificator(255,0,0);
    }
    if (!root.success()) 
    {
        Serial.println("parseObject() failed");
        return;
    }
}

void loop()
{
    //httpsRequest();
    //StaticJsonBuffer<11746> jsonBuffer; 
    //JsonObject& root = jsonBuffer.parseObject(client); //JSON 문자열을 root라는 JsonObject로 디코딩 / 파싱한다.
    //char* pm10Grade = root["pm10Grade"];
    //Serial.println(pm10Grade);
    //delay(requestInterval);
}

void strip_init()
{
    setStripColor(30, 255, 255, 255);
    setStripColor(30, 255, 0, 0);
    setStripColor(30, 0, 255, 0);
    setStripColor(30, 0, 0, 255);
}

void setStripColor(int interval, int R, int G, int B)
{
    for(int i = 0; i < pixelNum; i++)
    {
        strip.setPixelColor(i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void colorCircle(int interval, int R, int G, int B)
{
    for(int i = 0; i < pixelNum; i++)
    {
        strip.setPixelColor(pixelNum%i, strip.Color(R,G,B));
        delay(interval);
        strip.show();
    }
}

void blinkNotificator(int R, int G, int B)
{
    for(int i = 0 ; i < 2; i ++)
    {
        setStripColor(0, R, G, B);
        delay(200);
        setStripColor(0, 0, 0, 0);
        delay(200);
    }
}

void httpsRequest()
{
    //http://openapi.airkorea.or.kr/openapi/services/rest/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?stationName=%EC%A2%85%EB%A1%9C%EA%B5%AC&dataTerm=month&pageNo=1&numOfRows=10&ServiceKey=&ver=1.3
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
        client.print(F("&_returnType=json")); //JSON형식으로 받아온다.
        client.print(F(" HTTP/1.1\r\n"));
        client.print(F("Host: openapi.airkorea.or.kr\r\n"));
        client.print(F("Connection: close\r\n"));
        client.print(F("\r\n\r\n"));
    }
    else {
        Serial.println("[HTTP Request]Connection failed");
    }
}

int pm10GradeParser() //html태그 기반
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