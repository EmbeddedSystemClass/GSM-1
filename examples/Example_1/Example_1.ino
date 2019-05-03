#include <GSM_Client.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);
GSM_Client client(mySerial);
const char host[] = "m16.cloudmqtt.com";
uint16_t port = 12529;
void setup() {
  client.connect( host, port );
}

void loop() {
  // put your main code here, to run repeatedly:

}