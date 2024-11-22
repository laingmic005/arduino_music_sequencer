/*
 * Author:      Micah Laing
 * Date:        22 November 2024
 * Description: This code is designed for an Arduino Nano. It controls a multiplexer using pins D4, D5, D6, and D7,
 *              and a passive buzzer on pin D3. Pins D8, D9, D10, and D11 are used as inputs to read binary values,
 *              which determine the note to be played on the buzzer.
 */


// define the pins for controlling the multiplexer
const int s0 = 7;
const int s1 = 6;
const int s2 = 5;
const int s3 = 4;

const int buzzer = 3; // PWM pin
const int tempo = 500; // set the rate that the multiplexer cycles through channels

void setup() {
  // set pinmode for the pins controlling the multiplexer
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  // set input pins with internal pull-up resistors
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);

  // set PWM buzzer pin out
  pinMode(buzzer, OUTPUT);

  // initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // set the control pins to select the current channel (0-16)
  for (int i = 0; i < 16; i++) {
    digitalWrite(s0, i & 1);
    digitalWrite(s1, (i >> 1) & 1);
    digitalWrite(s2, (i >> 2) & 1);
    digitalWrite(s3, (i >> 3) & 1);
    
    int stateS0 = digitalRead(s0);
    int stateS1 = digitalRead(s1);
    int stateS2 = digitalRead(s2);
    int stateS3 = digitalRead(s3);

    // read the states of the input pins
    int state8 = digitalRead(8);
    int state9 = digitalRead(9);
    int state10 = digitalRead(10);
    int state11 = digitalRead(11);

    // print channel number and input values for debugging
    Serial.println("Channel: ");
    Serial.println(i);
  
    Serial.print(" | Pin 8: ");
    Serial.print(state8);
    Serial.print(" | Pin 9: ");
    Serial.print(state9);
    Serial.print(" | Pin 10: ");
    Serial.print(state10);
    Serial.print(" | Pin 11: ");
    Serial.println(state11);

// play the corresponding note on the buzzer based on pin states
  // Read the states of the input pins as a binary number
  int binaryValue = (digitalRead(8) << 3) |
                    (digitalRead(9) << 2) |
                    (digitalRead(10) << 1) |
                    digitalRead(11);

  // Array of frequencies for notes from C4 to G#5 (including half-steps)
  float notes[] = {
    261.63, // C4,  0001
    277.18, // C#4, 0010
    293.66, // D4,  0011
    311.13, // D#4, 0100
    329.63, // E4,  0101
    349.23, // F4,  0110
    369.99, // F#4, 0111
    392.00, // G4,  1000
    415.30, // G#4, 1001
    440.00, // A4,  1010
    466.16, // A#4, 1011
    493.88, // B4,  1100
    523.25, // C5,  1101
    554.37, // C#5, 1110
    587.33, // D5,  1111
    622.25, // D#5
    659.25, // E5
    698.46, // F5
    739.99, // F#5
    783.99, // G5
    830.61  // G#5
  };

  // Calculate the index by inverting the binary value
  int index = 15 - binaryValue;

  // Ensure the binary value is within range
  if (binaryValue != 15) {
    float frequency = notes[index];
    tone(buzzer, frequency);
    delay(tempo); // play the tone for 1 second
    noTone(buzzer);
    delay(5);
  } else {
    delay(tempo+5); // if binaryValue is 1111, play no tone
  }
}
}

