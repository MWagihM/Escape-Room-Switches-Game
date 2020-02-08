#include <Wire.h>
#include <FastLED.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"


Adafruit_7segment matrix = Adafruit_7segment();
CRGB leds[NUM_LEDS];

bool clockRunning = false;
bool solved = false;
//=============you can edit the below numbers only depending on your hardware configuration==============\\
const int pinSwitch1 = 7; //Pin number for switch number 1
const int pinSwitch2 = 8; //Pin number for switch number 2
const int pinSwitch3 = 9; //Pin number for switch number 3
const int pinSwitch4 = 12;//Pin number for switch number 4 

const int LED_PIN = 5; // LED strip Din wire must be connected with PWM pin that have '~' sign beside it
const int NUM_LEDS = 14; //<= write how much led in your LED strip here

const int lockCode = 201; //<= Game completion code here that will be displayed

//=============you can edit the above numbers only depending on your hardware configuration==============\\

int switch1 = 0;
int switch2 = 0;
int switch3 = 0;
int switch4 = 0;



uint16_t counter = 3000;
int countdownSpeed = 10;

void lED_Strip(char color)
{
  switch (color)
  {
    case 'R': for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Red; break;
    case 'G': for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Green; break;
    case 'W': for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::White; break;
    case 'B': for (int i = 0; i < NUM_LEDS; i++) leds[i] = CRGB::Black; break;
  }
  FastLED.show();
} 

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment Backpack Test");
#endif

  pinMode(pinSwitch1, INPUT);
  pinMode(pinSwitch2, INPUT);
  pinMode(pinSwitch3, INPUT);
  pinMode(pinSwitch4, INPUT);

  matrix.begin(0x70);

  matrix.print(0x3333, HEX);
  matrix.writeDisplay();
  delay(100);

  matrix.print(0x2222, HEX);
  matrix.writeDisplay();
  delay(100);

  matrix.print(0x1111, HEX);
  matrix.writeDisplay();
  delay(100);


  matrix.clear();
  matrix.writeDisplay();
  FastLED.addLeds<WS2812, LED_PIN, RGB>(leds, NUM_LEDS);
  delay(500);
  Serial.println("READY!");
}

void loop() {
  if (solved == false) {
    switch1 = digitalRead(pinSwitch1);
    switch2 = digitalRead(pinSwitch2);
    switch3 = digitalRead(pinSwitch3);
    switch4 = digitalRead(pinSwitch4);

    if (switch1 == HIGH && clockRunning == false)
    {
      Serial.println("BEGIN!");
      lED_Strip('G');
      clockRunning = true;
    }

    if (clockRunning == true)
    {
      matrix.println(counter);
      matrix.writeDigitRaw(2, 0x02);
      matrix.writeDisplay();
      delay(countdownSpeed);

      if (counter == 0)
      {
        matrix.writeDigitRaw(0, B01110001);
        matrix.writeDigitRaw(1, B01110111);
        matrix.writeDigitRaw(3, B00110000);
        matrix.writeDigitRaw(4, B00111000);
        matrix.writeDisplay();
        Serial.println("BOOM!");
        lED_Strip('R');
        counter = 3000;
        clockRunning = false;
        delay(30000);
        matrix.clear();
        matrix.writeDisplay();
        lED_Strip('B');
        return;
      }
      else
        counter--;

    }

    if (switch1 == HIGH && switch2 == HIGH && switch3 == HIGH && switch4 == HIGH && clockRunning == true) {
      solved = true;
      Serial.println("solved");
      clockRunning = false;
      lED_Strip('W');
      matrix.println(lockCode);
      matrix.writeDisplay();
      
    }
  }

}
