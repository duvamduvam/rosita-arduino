//local arduino_midi_library-master.zip
#include <MIDI.h>
#include <SPI.h>
//local RadioHead-master.zip
//used pin 9 2 maybe used 13 12 11 10 8 7 6
#include <RH_RF95.h>
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#include "Move.cpp"
#include "Lights.cpp"
#include "Head.cpp"
#include "Mapping.cpp"

Move move;
long lastMove = 0;
bool stopped = true;
#define moveTime 400

/// start radio def /////
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 868.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);
#define LED 13

uint8_t radioMsg[10];
uint8_t len = sizeof(radioMsg);

Lights lights;
bool lightOn = false;

MIDI_CREATE_DEFAULT_INSTANCE();

Mapping mapping;

String input;



void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  MIDI.begin();
  while (!Serial);
  // eratics problem with lora radio when changing baud rate
  Serial.begin(9600);
  Log.begin   (LOG_LEVEL, &Serial);

  delay(100);

  //// init radio
  //printStringLn(debug, "Arduino LoRa RX Test!");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
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
  rf95.setTxPower(23, false);
}

void radioRead() {
  if (rf95.available())
  {
    if (rf95.recv(radioMsg, &len))
    {
      digitalWrite(LED, HIGH);
      //Log.notice(F("radio received %s")CR, radioMsg);
      digitalWrite(LED, LOW);
    }
    else
    {
      Log.error("Receive failed");
    }
  }
}

void sendNote(int note) 
{
  Log.notice("send midi %d, %d", note);
  MIDI.sendNoteOn(note, 127, 3);    // Send a Note (pitch 42, velo 127 on channel 1)
  delay(10);                // Wait for a second
  MIDI.sendNoteOff(note, 0, 3);
}

void loop()
{

  radioRead();

  char* midiNote = mapping.getValue((char*)radioMsg);

  // test monitor
  if (Serial.available()) 
  {
    input = Serial.readStringUntil('\n');
    Log.notice("You typed: %s \n", radioMsg );
    input.toCharArray(radioMsg, 10);
  }
  // end test monitor


  if (strcmp((char*)radioMsg, "") != 0) 
  {
    Log.notice("input msg %s translate %s midi \n", radioMsg, midiNote);
    move.process(radioMsg);
    lastMove = millis();
    stopped = false;

    if (strstr((char*)radioMsg, "A26")) 
    {
      Log.notice("light");
      lightOn = !lightOn;
    }

    if (midiNote[0] == 'N') {
      int note;
      char mod;
      sscanf(midiNote, "%s %d", mod, &note);
      sendNote(note);
    }
  }

  if ((millis() - lastMove > moveTime) && !stopped) {
    move.stop();
    Log.notice("stop");
    stopped = true;
  }

  if (lightOn) {
    lights.process();
  }

  //delay(30);

  memset(radioMsg, 0, sizeof(radioMsg));
}
