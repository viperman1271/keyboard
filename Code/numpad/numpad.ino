#include <usb_keyboard.h>
#include <core_pins.h>

const int ledPin = LED_BUILTIN;

const int ROWS = 4; //four rows
const int COLS = 3; //three columns

int keyCodes[ROWS][COLS] = {
  {KEYPAD_1, KEYPAD_2, KEYPAD_3},
  {KEYPAD_4, KEYPAD_5, KEYPAD_6},
  {KEYPAD_7, KEYPAD_8, KEYPAD_9},
  {KEYPAD_ASTERIX, KEYPAD_0, KEYPAD_PLUS}
};

int rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
int colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

bool useLed = false;

void clearKeys()
{
  //Clear all of the keys, just to be safe
  keyboard_modifier_keys = 0;
  for (int i = 0; i < 6; ++i)
  {
    keyboard_keys[i] = 0;
  }
}

void setup()
{
  clearKeys();
  
  if(useLed)
  {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, HIGH);
  }
}

void useManualMethod()
{
  clearKeys();
  int keyIndex = 0;
  
  // Re-intialize the row pins. Allows sharing these pins with other hardware.
  for (int r = 0; r < ROWS; ++r) 
  {
    pinMode(rowPins[r],INPUT_PULLUP);
  }
  
  // bitMap stores ALL the keys that are being pressed.
  for (int c = 0; c < COLS; ++c)
  {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);  // Begin column pulse output.
    for (int r = 0; r < ROWS; ++r)
    {
      if(!digitalRead(rowPins[r]))
      {
        if(keyIndex < 6)
        {
          keyboard_keys[keyIndex] = keyCodes[r][c];
          ++keyIndex;
        }
      }
    }
    // Set pin to high impedance input. Effectively ends column pulse.
    digitalWrite(colPins[c], HIGH);
    pinMode(colPins[c], INPUT);
  }

  if(keyIndex < 6)
  {
    keyboard_keys[keyIndex] = (unsigned int)KEY_NUM_LOCK;
  }
  usb_keyboard_send();
}

void loop()
{
  useManualMethod();
  delay(20);
}
