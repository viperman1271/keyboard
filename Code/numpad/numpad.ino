#include <usb_keyboard.h>
#include <core_pins.h>
#include <Keypad.h>

#define LED_ON    (PORTD |= (1<<6))
#define LED_OFF   (PORTD &= ~(1<<6))

const int ledPin = LED_BUILTIN;

#define PIN_D0 0
#define PIN_D1 1
#define PIN_D2 2
#define PIN_D3 3
#define PIN_D4 4
#define PIN_D5 5
#define PIN_D6 6

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
char keyCodes[ROWS][COLS] = {
  {KEYPAD_1, KEYPAD_2, KEYPAD_3},
  {KEYPAD_4, KEYPAD_5, KEYPAD_6},
  {KEYPAD_7, KEYPAD_8, KEYPAD_9},
  {KEYPAD_ASTERIX, KEYPAD_0, KEYPAD_PLUS}
};
byte rowPins[ROWS] = {0, 1, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 5, 6}; //connect to the column pinouts of the keypad

uint bitMap[12];

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void clearKeys()
{
  int asdf = KEY_A;
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

  pinMode(PIN_D0, OUTPUT);
  pinMode(PIN_D1, OUTPUT);
  pinMode(PIN_D2, OUTPUT);
  pinMode(PIN_D3, OUTPUT);

  pinMode(PIN_D4, INPUT);
  pinMode(PIN_D5, INPUT);
  pinMode(PIN_D6, INPUT);

  pinMode(ledPin, OUTPUT);

  digitalWrite(ledPin, HIGH);
}

void blinkLight()
{
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
}

void useKeypadLib()
{
  char key = keypad.getKey();
  Keyboard.print(key);
}

void useManualMethod()
{
  clearKeys();
  char keyIndex = 0;
  
  // Re-intialize the row pins. Allows sharing these pins with other hardware.
  for (byte r = 0; r < ROWS; ++r) 
  {
    pinMode(rowPins[r],INPUT_PULLUP);
  }
  
  // bitMap stores ALL the keys that are being pressed.
  for (byte c = 0; c < COLS; ++c)
  {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], LOW);  // Begin column pulse output.
    for (byte r = 0; r < ROWS; ++r)
    {
      bitWrite(bitMap[r], c, !digitalRead(rowPins[r]));  // keypress is active low so invert to high.
      if(!digitalRead(rowPins[r]))
      {
        if(keyIndex < 6)
        {
          keyboard_keys[keyIndex] = keyCodes[r][c];
          ++keyIndex;
        }
        //Keyboard.print(keys[r][c]);
      }
      //Keyboard.print(keys[r][c]);
    }
    // Set pin to high impedance input. Effectively ends column pulse.
    digitalWrite(colPins[c], HIGH);
    pinMode(colPins[c], INPUT);
  }

  if(keyIndex < 6)
  {
    keyboard_keys[keyIndex] = KEY_NUM_LOCK;
  }
  usb_keyboard_send();
}

void loop()
{
  //int keyIndex = 0;
  //digitalWrite(PIN_D0, HIGH);

  /*if(!digitalRead(PIN_D4))
    {
    digitalWrite(ledPin, LOW);
    }

    if(!digitalRead(PIN_D5))
    {
    digitalWrite(ledPin, LOW);
    }

    if(!digitalRead(PIN_D6))
    {
    digitalWrite(ledPin, LOW);
    }*/

  /*if(!digitalRead(PIN_D4))
    {
    //keyboard_keys[keyIndex++] = KEY_A;
    //usb_keyboard_press(KEY_A, 0);
    }*/
  /*if(!digitalRead(PIN_D5))
    {
    keyboard_keys[keyIndex++] = KEY_B;
    //usb_keyboard_press(KEY_B, 0);
    }
    if(!digitalRead(PIN_D6))
    {
    keyboard_keys[keyIndex++] = KEY_C;
    //usb_keyboard_press(KEY_C, 0);
    }*/

  //digitalWrite(ledPin, HIGH);
  //uint8_t modifier = 0;
  //  usb_keyboard_press(KEY_A, modifier);
  //  usb_keyboard_send();
  //  _delay_ms(50);
  //delay(50);


  //blinkLight();

  //usb_keyboard_send();

  useManualMethod();
  delay(50);
}
