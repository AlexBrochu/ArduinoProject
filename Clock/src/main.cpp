#include <Arduino.h>
#include "Time.h"
// Piezo melody comes from
// https://github.com/42Bots/ArduinoTunes
#include "pitches.h"

void playTune(int notes[], int durations[], int BPM);
// Array with the notes in the melody (see pitches.h for reference)
int melody[] = {NOTE_A4, NOTE_A4, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4, NOTE_E5, NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_C5, NOTE_A4};

// Array with the note durations: a quarter note has a duration of 4, half note 2 etc.
int durations[]  = {4, 4, 4, 5, 16, 4, 5, 16, 2, 4, 4, 4, 5, 16, 4, 5, 16, 2};

int tempo = 120; // tempo for the melody expressed in beats per minute (BPM)

void digitalClockDisplay();
void printDigits(int);
void soundAlarm();
void stopAlarm();
void timeOUT();
void blinkLED(int pin);
void displayRemainingTime(long ElapsedTime, bool reset);
void alternateLED();

// Button panel
const int startBtnPin = 13;
const int startLEDPin = 12;

const int resetBtnPin = 11;
const int resetLEDPin = 10;

// LED interface
const int percent75LEDPin = 7;
const int percent50LEDPin = 6;
const int percent25LEDPin = 4;

// Piezo alarm
const int alarmPin = 2;

bool startedTimer = false;

long StartTime = millis();
long CurrentTime = millis();
const long timeLimit = (long)60*60*1000;

int val;

void setup() {
    Serial.begin(9600);

    Serial.print(F("TimeLimit : "));
    Serial.print(timeLimit/1000);
    Serial.println(F(" seconds."));

    pinMode(startBtnPin, INPUT);
    pinMode(startLEDPin, OUTPUT);
    pinMode(resetBtnPin, INPUT);
    pinMode(resetLEDPin, OUTPUT);

    // LED Panel
    pinMode(percent75LEDPin, OUTPUT);
    pinMode(percent50LEDPin, OUTPUT);
    pinMode(percent25LEDPin, OUTPUT);

    // put your setup code here, to run once:
}

void loop() {
    // put your main code here, to run repeatedly:
    val = digitalRead(startBtnPin);
    if(val == HIGH){
        digitalWrite(startLEDPin, HIGH);
        startedTimer = true;
        Serial.println(F("============  Timer Started ============"));
        StartTime = millis();
        displayRemainingTime(0, false);
    }

    val = digitalRead(resetBtnPin);
    if(val == HIGH){
        digitalWrite(resetLEDPin, HIGH);
        digitalWrite(startLEDPin, LOW);
        startedTimer = false;
        stopAlarm();
        displayRemainingTime(0, true);
    }
    else{
        digitalWrite(resetLEDPin, LOW);
    }


    if(startedTimer){
        alternateLED();
        CurrentTime = millis();
        // digitalClockDisplay();
        delay(1000);
        long ElapsedTime = CurrentTime - StartTime;
        Serial.print(F("ElapsedTime : "));
        Serial.println(ElapsedTime/1000);
        if(ElapsedTime >= timeLimit){
            timeOUT();
        }
        else{
            displayRemainingTime(ElapsedTime, false);
        }
    }
}

void alternateLED(){
    int startedState = digitalRead(startLEDPin);
    if(startedState == HIGH){
        digitalWrite(startLEDPin, LOW);
        digitalWrite(resetLEDPin, HIGH);
    }else{
        digitalWrite(startLEDPin, HIGH);
        digitalWrite(resetLEDPin, LOW);
    }
}

void displayRemainingTime(long ElapsedTime, bool reset = false){
    float percentage = 1.0 - (float)ElapsedTime/timeLimit;
    if(reset){
        percentage = 0;
    }
    // Serial.print("Percentage : ");
    // Serial.println(percentage);
    if(percentage >= 0.75){
        digitalWrite(percent75LEDPin, HIGH);
        digitalWrite(percent50LEDPin, HIGH);
        digitalWrite(percent25LEDPin, HIGH);
    }else if(percentage >= 0.50){
        digitalWrite(percent75LEDPin, LOW);
        digitalWrite(percent50LEDPin, HIGH);
        digitalWrite(percent25LEDPin, HIGH);
    }else if(percentage >= 0.25){
        digitalWrite(percent75LEDPin, LOW);
        digitalWrite(percent50LEDPin, LOW);
        digitalWrite(percent25LEDPin, HIGH);
    } else {
        digitalWrite(percent75LEDPin, LOW);
        digitalWrite(percent50LEDPin, LOW);
        digitalWrite(percent25LEDPin, LOW);
    }
}

void timeOUT(){
    soundAlarm();
    blinkLED(percent75LEDPin);
    blinkLED(percent50LEDPin);
    blinkLED(percent25LEDPin);
}

void blinkLED(int pin){
    digitalWrite(pin, HIGH);
    delay(250);
    digitalWrite(pin, LOW);
}

void soundAlarm(){
    playTune(melody, durations, tempo);
}

void stopAlarm(){
    noTone(alarmPin);
}

void digitalClockDisplay() {
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(" ");
    Serial.print(day());
    Serial.print(" ");
    Serial.print(month());
    Serial.print(" ");
    Serial.print(year());
    Serial.println();
}

void printDigits(int digits) {
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(":");
    if (digits < 10)
    Serial.print('0');
    Serial.print(digits);
}


void playTune(int notes[], int durations[], int BPM)
{
  int tuneSize = sizeof(melody) / sizeof(int);

  // iterate over the notes of the tune:
  for (int thisNote = 0; thisNote < tuneSize; thisNote++) {

    // For details on calculating the note duration using the tempo and the note type,
    // see http://bradthemad.org/guitar/tempo_explanation.php.
    // A quarter note at 60 BPM lasts exactly one second and at 120 BPM - half a second.

    int noteDuration = (int)((1000 * (60 * 4 / BPM)) / durations[thisNote] + 0.);
    tone(alarmPin, notes[thisNote],noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 20% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.20;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(alarmPin);
  }
}
