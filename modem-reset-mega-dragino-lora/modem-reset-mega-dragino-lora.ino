#include "ArduinoLog.h"
#include <RH_RF95.h>
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define RFM95_INT 2
#define RFM95_RST 9
#define RFM95_CS 10
#define LED 13

#define RESET 7

/// start radio def /////

#define RF95_FREQ 868.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);

#define DIGITAL_RESET 31

uint8_t radioMsg[12];
uint8_t len = sizeof(radioMsg);

//Mapping mapping;

void setup()
{

  
  delay(1000);

  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);



  while (!Serial);
  // eratics problem with lora radio when changing baud rate
  Serial.begin(115200);
  Serial2.begin(115200);

  Serial.println("start reset modem");

  Log.begin   (LOG_LEVEL, &Serial);
  //midi debug

  delay(100);

  pinMode(DIGITAL_RESET, OUTPUT); 

  /*digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
  while (!rf95.init()) {
    Log.error("LoRa radio init failed");
    while (1);
  }
  Log.notice("LoRa radio init OK!\n");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Log.error("setFrequency failed");
    while (1);
  }
  //printString(debug, "Set Freq to: "); printDoubleLn(debug, RF95_FREQ);
  rf95.setTxPower(23, false);*/
}

void radioRead() {
  if (rf95.available())
  {
    uint8_t len = sizeof(radioMsg);
    if (rf95.recv(radioMsg, &len))
    {
      digitalWrite(LED, HIGH);
      Log.notice("radio received %s\n", radioMsg);
      digitalWrite(LED, LOW);

    }
    else
    {
      Log.error("Receive failed");
    }
  }
}




void loop()
{

  //delay(5000);

  //digitalWrite(DIGITAL_RESET, HIGH);
  
  /*radioRead();



  if (strcmp((char*)radioMsg, "")) {
    Serial2.print((char*)radioMsg);


  memset(radioMsg, 0, sizeof(radioMsg));
*/


  
}
