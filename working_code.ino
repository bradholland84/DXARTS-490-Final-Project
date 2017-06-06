
#include "pitches.h"

// number of analog samples to take per reading
#define NUM_SAMPLES 10

int sum = 0;                    // sum of samples taken
unsigned char sample_count = 0; // current sample number
float voltage = 0.0;            // calculated voltage

// notes in the melody:
int melody[] = {
  NOTE_GS4, 0, 0, 0, NOTE_FS4, 0, 0, 0, NOTE_DS5, NOTE_B4, 0, 0, NOTE_FS4, 0, 0, 0
};

const int songLength = 16;

int noteDurations[4][songLength] = {
  {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4},
  {6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6},
  {8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8},
  {12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12}  
 };

int speakerPin = 9;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    // take a number of analog samples and add them up
    while (sample_count < NUM_SAMPLES) {
        sum += analogRead(A2);
        sample_count++;
        delay(10);
    }

    voltage = ((float)sum / (float)NUM_SAMPLES * 5.015) / 1023.0; //1023 for analog
    Serial.print(voltage);
    Serial.println (" V");
    sample_count = 0;
    sum = 0;

    //only play music if some voltage is present, but not max
    if (voltage > 0.45) {
      if (voltage < 1.15) {
         playMelody(0);
      }
      else if (voltage < 1.45) {
        playMelody(1);
      }
      else if (voltage < 3.00) {
        playMelody(2);
      }
      else if (voltage < 4.50) {
        playMelody(3);
      }
    }
}

void playMelody(int index) 
{
  Serial.print("Melody at index");
  Serial.println(index);
  for (int thisNote = 0; thisNote < songLength; thisNote++) {

    int noteDuration = 1000 / noteDurations[index][thisNote];
    tone(speakerPin, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(speakerPin);     // stop the tone playing

  }
}

