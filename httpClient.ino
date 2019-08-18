#include <Bridge.h>
#include <HttpClient.h>

#define url ""http://www.arduino.cc/asciilogo.txt""

void setup() {
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Bridge.begin();
  digitalWrite(13, HIGH);

  Serial.begin(9600);
}

void loop() {
  HttpClient client;
  client.get(url);

  while (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  Serial.flush();

  delay(5000);
}