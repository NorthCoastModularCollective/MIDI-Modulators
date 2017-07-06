#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioSynthWaveformSine   sine1;          //xy=194,156
AudioSynthWaveformSine   sine2;          //xy=194,156
AudioSynthWaveformSine   sine3;          //xy=194,156
AudioAnalyzePeak         peak1;          //xy=350,123
AudioAnalyzePeak         peak2;          //xy=350,123
AudioAnalyzePeak         peak3;          //xy=350,123
AudioOutputI2S           i2s2;           //xy=475,155
AudioConnection          patchCord1(sine1, peak1);
AudioConnection          patchCord2(sine2, peak2);
AudioConnection          patchCord3(sine3, peak3);
// GUItool: end automatically generated code

AudioControlSGTL5000 codec;

//  Set up all the pin number variables
//trigger inputs
int Trigger1Pin = 5; 
int Trigger2Pin = 8; 
int Trigger3Pin = 28; 
//LEDs
int LEDA = 29;
int LEDB = 30;
int LEDC = 31;
int LEDD = 32;
int LEDE = 33;
int Trig1LED = 27;
int Trig2LED = 26;
int Trig3LED = 25;
//Switches
int SwitchA1 = 0;
int SwitchB1 = 1;
int SwitchC1 = 2;
int SwitchD1 = 3;
int SwitchE1 = 4;
//Potentiometers
int pA2 = A13;
int pB2 = A0;
int pC2 = A12;
int pD2 = A7;
//int pE2 = A6;
//int pA3 = A3; 
int pB3 = A11;
int pC3 = A10;
int pD3 = A1;
int pE3 = A2;

// the MIDI channel number to send messages
const int ccChannel = 1;

// set up 5 MIDI CC numbers
const int controllerA0 = 10; // 10 = pan position
const int controllerA1 = 11; // 11
const int controllerA2 = 12; // 12
const int controllerA3 = 13; // 13
const int controllerA4 = 14; // 14

// set up 5 MIDI CC values
float ccValue1;
float ccValue2;
float ccValue3;
float ccValue4;
float ccValue5;

void setup() {
  //Set the amplitude, from 0 to 1.0.
  sine1.amplitude(1.0);
  sine2.amplitude(1.0);
  sine3.amplitude(1.0);
  //Set the frequency, from 0 to 22000
  sine1.frequency(0.1);
  sine2.frequency(0.1);
  sine3.frequency(0.1);

  ccValue1 = 0.0;  
  ccValue2 = 0.0;  
  ccValue3 = 0.0;  
  ccValue4 = 0.0;  
  ccValue5 = 0.0;  

  // Get the board ready
    AudioMemory(50);
    codec.enable();
    
  //Now set up the oscillators  
    AudioNoInterrupts();

  //Set up Digital Pins
  //triggers
    pinMode(Trigger1Pin, INPUT); //D1 is a digital input
    pinMode(Trigger2Pin, INPUT); //D2 is a digital input
    pinMode(Trigger3Pin, INPUT); //D3 is a digital input
  //LEDs  
    pinMode(LEDA, OUTPUT); 
    pinMode(LEDB, OUTPUT); 
    pinMode(LEDC, OUTPUT); 
    pinMode(LEDD, OUTPUT);
    pinMode(LEDE, OUTPUT); 
    pinMode(Trig1LED, OUTPUT);
    pinMode(Trig2LED, OUTPUT);         
    pinMode(Trig3LED, OUTPUT); 
  //switches 
  
    pinMode(SwitchA1, INPUT_PULLUP); //Switch input with internal pullup resistor  
    pinMode(SwitchB1, INPUT_PULLUP); //Switch input with internal pullup resistor
    pinMode(SwitchC1, INPUT_PULLUP); //Switch input with internal pullup resistor
    pinMode(SwitchD1, INPUT_PULLUP); //Switch input with internal pullup resistor       
    pinMode(SwitchE1, INPUT_PULLUP); //Switch input with internal pullup resistor 
  
    AudioInterrupts();
  
  //Board ready, flash LEDs to indicate ready state on boot up
  digitalWrite(LEDA, HIGH); //animate cycling the LEDs on from left to right
  delay(100);              
  digitalWrite(LEDA, LOW);
  digitalWrite(LEDB, HIGH);
  delay(100);              
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDC, HIGH);
  delay(100);              
  digitalWrite(LEDC, LOW);
  digitalWrite(LEDD, HIGH);
  delay(100);              
  digitalWrite(LEDD, LOW);
  digitalWrite(LEDE, HIGH);
  delay(100);              
  digitalWrite(LEDE, LOW);
  digitalWrite(LEDD, HIGH);  //now animate cycling the LEDs on from right to left
  delay(100);              
  digitalWrite(LEDD, LOW);
  digitalWrite(LEDC, HIGH);
  delay(100);              
  digitalWrite(LEDC, LOW);
  digitalWrite(LEDB, HIGH);
  delay(100);              
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDA, HIGH);
  delay(100); 
  digitalWrite(LEDA, LOW);
  delay(100);             
  digitalWrite(LEDA, HIGH); //flash them all on
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDC, HIGH);
  digitalWrite(LEDD, HIGH);
  digitalWrite(LEDE, HIGH);
  delay(200);              
  digitalWrite(LEDA, LOW); //flash them all off
  digitalWrite(LEDB, LOW); 
  digitalWrite(LEDC, LOW); 
  digitalWrite(LEDD, LOW); 
  digitalWrite(LEDE, LOW); 
}

void loop() {
  int lfo1Freq = map(analogRead(pA2), 0, 1023, 0.001, 10); 
  int lfo2Freq = map(analogRead(pB2), 0, 1023, 0.001, 10); 
  int lfo3Freq = map(analogRead(pC2), 0, 1023, 0.001, 10); 
  sine1.frequency(lfo1Freq);
  sine2.frequency(lfo2Freq);
  sine3.frequency(lfo3Freq);
  
  ccValue1 = peak1.read();
  ccValue2 = peak2.read();
  ccValue3 = peak3.read();
  displaySerial();

  // Check current values of switches to see which LFOs are on/off
  bool SwitchA1state = digitalRead(SwitchA1);
  bool SwitchB1state = digitalRead(SwitchB1);
  bool SwitchC1state = digitalRead(SwitchC1);
  bool SwitchD1state = digitalRead(SwitchD1);
  bool SwitchE1state = digitalRead(SwitchE1);

  // LFO A
  if (SwitchA1state == HIGH) {
    // If the LFO is turned on, send out the peak snapshot value of the LFO as a CC
    usbMIDI.sendControlChange(controllerA0, floatToMIDICCValue(ccValue1), ccChannel);
    digitalWrite(LEDA, HIGH); 
  } else {
    // Otherwise, send the knob position as the CC value

    // NEED TO UPDATE THIS TO ONLY SEND CC DATA IF KNOB WAS CHANGED FROM 
    // LAST VALUE - OTHERWISE, MESSES UP ABLETON LIVE'S MAPPING ASSIGNMENTS
    // AND ERRONEOUSLY ENABLES 'RE-ENABLE AUTOMATION BUTTON IN LIVE
    usbMIDI.sendControlChange(controllerA0, map(analogRead(pA2), 0, 1023, 0, 127), ccChannel);
    digitalWrite(LEDA, LOW);     
  }

  // LFO B
  if (SwitchB1state == HIGH) {
    usbMIDI.sendControlChange(controllerA1, floatToMIDICCValue(ccValue2), ccChannel);
    digitalWrite(LEDB, HIGH); 
  } else {
    usbMIDI.sendControlChange(controllerA1, map(analogRead(pB2), 0, 1023, 0, 127), ccChannel);
    digitalWrite(LEDB, LOW);     
  }

  // LFO C
  if (SwitchC1state == HIGH) {
    usbMIDI.sendControlChange(controllerA2, floatToMIDICCValue(ccValue3), ccChannel);
    digitalWrite(LEDC, HIGH); 
  } else {
    usbMIDI.sendControlChange(controllerA2, map(analogRead(pC2), 0, 1023, 0, 127), ccChannel);
    digitalWrite(LEDC, LOW);     
  }
  if (SwitchD1state == HIGH) {
    usbMIDI.sendControlChange(controllerA3, floatToMIDICCValue(ccValue1), ccChannel);
    digitalWrite(LEDD, HIGH); 
 } else {
    digitalWrite(LEDD, LOW);     
  }
  if (SwitchE1state == HIGH) {
    usbMIDI.sendControlChange(controllerA4, floatToMIDICCValue(ccValue1), ccChannel);
    digitalWrite(LEDE, HIGH); 
  } else {
    digitalWrite(LEDE, LOW);     
  }
  delay(10);
}

int floatToMIDICCValue(float val) {
  return val * 128;
}

void displaySerial() {
  Serial.println("ccValue1 is ");
  Serial.println(ccValue1);
  Serial.println("MIDI CC value is ");
  Serial.println(floatToMIDICCValue(ccValue1));
}
