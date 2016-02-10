#include <MIDI.h>
//This sketch probably does not work yet. Is was written on a train. Consider it pseudocode.

MIDI_CREATE_DEFAULT_INSTANCE();

const int TOTAL_BANKS = 5; //Total number of banks on rotary switch
const int TOTAL_PADS = 4; //Total number of drum pads
const int MIDI_CHANNEL = 1; //MIDI Channel to send notes on
const int VELOCITY = 127; //Velocity to use for each MIDI note
const int SENSITIVITY_CONTROL = A4;
int bankSwitch[TOTAL_BANKS] = {5, 6, 7, 8, 9}; //These are the Digital pins rotary switch attaches to
int knockSensors[TOTAL_PADS] = {A0, A1, A2, A3}; //These are the Analog pins drum pads attach to.

//This executes at startup
void setup() {
  //Setup all bank options as input pins
  for(int i = 0; i < TOTAL_BANKS; i++){
    pinMode(bankSwitch[i], INPUT);
  }
}


//Do this forever
void loop() {

  int bank = getChosenBank();
  int threshold = getThreshold();

  for(int i = 0; i < TOTAL_PADS; i++){
    checkAndTrigger(knockSensors[i], bank, threshold);
  }
  delay(50);
}


//Determine which bank is currently selected
int getChosenBank(){

  //Loops through all bankSwitch pins
  for(int i = 0; i < TOTAL_BANKS; i++){
    //If is in HIGH state, return value
    if(digitalRead(bankSwitch[i]) == HIGH){
      return i;
    };
  }
}

/*
get the current trigger threshold value
*/
int getThreshold(){
  int threshold = analogRead(SENSITIVITY_CONTROL);
  //TODO - Find realistic min / max bounds for sensitivity
  threshold = map(threshold, 0, 1024, 0, 200);
  return threshold;
}

/*
Checks a pads state and if required fires related MIDI Note On
@params {int} targetPad Pad to be checked
@params {int} bank Active bank number
@param {int} threshold Sensitivity threshold value
*/
void checkAndTrigger(int targetPad, int bank, int threshold){
  int note = 0;
  int sensorReading = analogRead(targetPad);
  if (sensorReading >= threshold) {
    //Serial.println(sensorReading);
    note = padToNote(targetPad, bank);
    MIDI.sendNoteOn(note, VELOCITY, MIDI_CHANNEL);
  }
}
/**
Convert a pad and bank combination to MIDI Note
@params {int} Triggered pad
@params {int} Currently selected bank
*/
int padToNote(int pad, int bank){
  int baseNote = 60;
  int note = (pad * (bank + 1)) + baseNote;
  return note;
}
