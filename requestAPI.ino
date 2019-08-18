#include <WiFi.h>

#define ssid ""   
#define password ""      

#define APIKEY    ""
#define CITY    "강동구"
#define VERSION    "1.3"

char server[] = "openapi.airkorea.or.kr";

unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10000; // delay between updates, in milliseconds

String rcvbuf;
int indexOftmp=0;
String DataTime;
String pm10Grade;
String tmp_str;

int red = 0;
int green = 0;
int blue = 0;
float brightness = 1.0;

WiFiClient client;

void httpRequest() {
    Serial.println();
    client.stop();

    if (client.connect(server, 80)) {
        Serial.println("[HTTP Request]Connecting...");

        client.print(F("GET /openapi/services/rest/ArpltnInforInqireSvc/getMsrstnAcctoRltmMesureDnsty?stationName="));
        client.print(CITY);
        client.print(F("&dataTerm=daily&pageNo=1&numOfRows=10&ServiceKey="));
        client.print(APIKEY);
        client.print(F("&ver="));
        client.print(VERSION);
        client.print(F(" HTTP/1.1\r\n"));
        client.print(F("Host: openapi.airkorea.or.kr\r\n"));
        client.print(F("Connection: close\r\n"));
        client.print(F("\r\n\r\n"));
    
        lastConnectionTime = millis();
    }
    else {
        Serial.println("[HTTP Request]Connection failed");
    }
}

void setup(){
    Serial.begin(115200);
    Serial.println(F("\r\n[Setup]Serial Init"));
    pinMode(LED_BUILTIN, OUTPUT);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.println("[Setup]Connecting to WiFi..");
    }
    Serial.println("[Setup]Connected to the WiFi network");
    for(int i = 0 ; i < 10 ; i ++){
        digitalWrite(LED_BUILTIN, HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN, LOW);
        delay(50);
    }
    
}

void loop() {
    if (millis() - lastConnectionTime > postingInterval) {
        httpRequest();
    }
    while (client.available()) {
        String line = client.readStringUntil('\n');
        indexOftmp = line.indexOf("</dataTime>");
        //Serial.println(line);

        if(indexOftmp>0){
            tmp_str="<dataTime>";
            DataTime = line.substring(line.indexOf(tmp_str)+tmp_str.length(),indexOftmp);
            Serial.print("DataTime : ");
            Serial.println(DataTime); 
        
        }
        
        indexOftmp = line.indexOf("</pm10Grade>");

        if(indexOftmp>0){
            tmp_str="<pm10Grade>";
            pm10Grade = line.substring(line.indexOf(tmp_str)+tmp_str.length(),indexOftmp);
            Serial.print("PM10 Grade: ");
            Serial.println(pm10Grade);
            client.stop();
            break;
             
        }
        
    
    }
}

