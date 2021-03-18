#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SerialFlash.h>


/*From PJRC Audio System Design Tool */
// GUItool: begin automatically generated code
AudioSynthWaveformModulated waveformMod1;   //xy=192,327
AudioAnalyzePeak         peak1;          //xy=344,327
AudioOutputI2S           i2s1;           //xy=345,285
AudioConnection          patchCord1(waveformMod1, peak1);
AudioConnection          patchCord2(waveformMod1, 0, i2s1, 0);
// GUItool: end automatically generated code

//Do we need this in order to generate waveforms? We aren't actually generating audio output...
AudioControlSGTL5000 codec;


// MIDI SETUP
// the MIDI channel number to send messages

const int ccChannel = 1;

// set up 5 MIDI CC numbers
const int controllerNumber = 10; // 10 = pan position

// Put this here for now
float sampleValue = 0.0;
int previousCCValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // WAVEFORM GENERATOR
  //Set the amplitude, from 0 to 1.0.
  waveformMod1.begin(0.5, 0.25, WAVEFORM_SINE);
  waveformMod1.offset(0.5);

  /* OLD WAY OF SETTING AMPLITUDE & FREQUENCY
  waveformMod1.amplitude(1.0);
  //Set the frequency, from 0 to 22000
  waveformMod1.frequency(0.1);
  */

  // Get the board ready
  AudioMemory(50);
  codec.enable();
}

void loop() {
  // put your main code here, to run repeatedly:
  // set up initial MIDI CC value
  if(peak1.available()) {
    sampleValue = peak1.read();
  }

  
  int ccValue = floatToMIDICCValue(sampleValue);

  // only output a CC value if it has changed from the previous value to not continually send duplicate CC values
  if(ccValue != previousCCValue) {
    previousCCValue = ccValue;
    usbMIDI.sendControlChange(controllerNumber, ccValue, ccChannel);
  }

  // Output the waveform value as a MIDI CC
  Serial.println("Sample Value was: ");
  Serial.println(sampleValue);
  Serial.println("CC Value was: ");
  Serial.println(ccValue);
}

int floatToMIDICCValue(float val) {
  return val * 127;
}
