// Robust Rotary encoder reading
//
// Copyright John Main - best-microcontroller-projects.com
//
#define PIN_CLK 2
#define PIN_DATA 3
#define PIN_BUTTON 4

void setup() {
  pinMode(PIN_CLK, INPUT);
  pinMode(PIN_CLK, INPUT_PULLUP);
  pinMode(PIN_DATA, INPUT);
  pinMode(PIN_DATA, INPUT_PULLUP);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.begin (9600);
  Serial.println("R");
}

static uint8_t prevNextCode = 0;
static uint16_t store=0;

int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

/*
bool buttonPinState = LOW;
bool prevButtonPinState = buttonPinState;

unsigned long lastButtonPinDebounceTime = 0;  // the last time the output pin was toggled
unsigned long buttonPinDebounceDelay = 50;    // the debounce time; increase if the output flickers
*/
void loop() {
  static uint8_t c, val;
  if (val = read_rotary()) {
    c += val;
    if (prevNextCode == 0x0b) {
       // Serial.print(c);
       Serial.println("-");
       // Serial.println(store,HEX);
    } else if (prevNextCode == 0x07) {
       // Serial.print(c);
       Serial.println("+");
       // Serial.println(store,HEX);
    } else if (store == 0xd7d4) {
       // Serial.print(c);
       Serial.println("-");
       // Serial.println(store,HEX);
    } else if (store == 0xebe8) {
       // Serial.print(c);
       Serial.println("+");
       // Serial.println(store,HEX);
    }

  }

  int reading = digitalRead(PIN_BUTTON);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        Serial.println("x");
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

  /*
  buttonPinState = digitalRead(PIN_BUTTON);
  if (buttonPinState != prevButtonPinState){
    lastButtonPinDebounceTime = millis();
  }
  if ((millis() - lastButtonPinDebounceTime) > buttonPinDebounceDelay) {
    Serial.println(buttonPinState);
     if (buttonPinState != prevButtonPinState){
      prevButtonPinState = buttonPinState;
      // Serial.println(buttonPinState);
    }
    
   
    
  }
  prevButtonPinState = buttonPinState;
  */
}

// A vald CW or  CCW move returns 1, invalid returns 0.
int8_t read_rotary() {
  static int8_t rot_enc_table[] = {0,1,1,0,1,0,0,1,1,0,0,1,0,1,1,0};

  prevNextCode <<= 2;
  if (digitalRead(PIN_DATA)) prevNextCode |= 0x02;
  if (digitalRead(PIN_CLK)) prevNextCode |= 0x01;
  prevNextCode &= 0x0f;

  // If valid then store as 16 bit data.
  if  (rot_enc_table[prevNextCode] ) {
    store <<= 4;
    store |= prevNextCode;
    // if (store==0xd42b) return 1;
    // if (store==0xe817) return -1;
    // Serial.print("  ");
    // Serial.println(store, HEX);
    // full detents
    if ((store&0xff) == 0x2b) return -1;
    if ((store&0xff) == 0x17) return 1;
    // half detents
    if (store == 0xd7d4) return -1;
    if (store == 0xebe8) return 1;
  }
  return 0;
}
