/* A IoT hub to interact with an Android device (through serial USB) and control some home appliances:
   - a TV through infrared remote
   - a lamp or something to switch on and off through a 110V/220V AC output relay
   - a (hacked) Sennheiser Office 1 Headset ( http://en-us.sennheiser.com/wireless-phone-headset-single-sided-dect-sd-office ) through two optocouplers

   Some parts of the code were based on the following references:
   - http://www.arduino.cc/en/Tutorial/LiquidCrystal : for the LCD (LCM1602B) control
   - http://forum.arduino.cc/index.php/topic,4104.0.html and http://playground.arduino.cc/uploads/Learning/relays.pdf : for the relay control
   - https://learn.sparkfun.com/tutorials/ir-communication and http://www.righto.com/2009/08/multi-protocol-infrared-remote-library.html : for the infrared communication control using the awesome IRremote library and examples by Ken Shirriff
   - http://www.martyncurrey.com/?p=34 and http://tronixstuff.com/2010/09/21/education-introduction-to-the-optocoupler/ : for the optocouplers part

   For Android communication with Arduino through serial USB take a look at the following references: 
   - http://android-er.blogspot.com.br/2014/02/android-usb-host-mode-step-by-step.html
   - http://android-er.blogspot.com.br/2014/02/send-hello-to-arduino-from-android-in.html
   - http://android-er.blogspot.com.br/2014/09/send-data-from-android-to-arduino-uno.html
   - https://github.com/mik3y/usb-serial-for-android (Android Java library)
   - https://github.com/isis/USB-Serial-Module (Titanium library for Android)
 */

#include <IRremote.h>
#include <LiquidCrystal.h>

// Hardware pins definitions:
// general intput pins:
int BTN1 = 19;
int BTN2 = 18;
int BTN3 = 17;
int BTN4 = 16;

//general output pins:
int LED_PIN = 15;
int RELAY_PIN = 14;
int STATUS_PIN = 13;
int OPTO2_PIN = 12;
int OPTO1_PIN = 11;

// infrared control pins:
int IR_PIN = 3; //internally defined inside the IRremote library
int IR_RECV_PIN = 2;
int IR_STATUS_PIN = 4;

// infrared library initialization
IRrecv irrecv(IR_RECV_PIN);
IRsend irsend;
decode_results results;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(10, 9, 8, 7, 6, 5);

// Control variables:
int buttonState, lastButtonState = LOW;
int recButtonState, lastRecButtonState = LOW;
unsigned long timer, recTimer, recTime, seconds;
int recording = 0;
int lcdCursor = 0;
boolean btn1, btn2, btn3, btn4 = false;
boolean communicating = false;

void setup()
{
  Serial.begin(115200);
  irrecv.enableIRIn(); // start the receiver
  lcd.begin(16, 2); // start  the LCD with 16 columns and 2 rows

  // Setting the pin modes:
  pinMode(BTN1,INPUT_PULLUP);
  pinMode(BTN2,INPUT_PULLUP);
  pinMode(BTN3,INPUT_PULLUP);
  pinMode(BTN4,INPUT_PULLUP);
  pinMode(OPTO1_PIN,OUTPUT);
  pinMode(OPTO2_PIN,OUTPUT);
  pinMode(STATUS_PIN,OUTPUT);
  pinMode(RELAY_PIN,OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  pinMode(IR_STATUS_PIN,OUTPUT);
  
  // Initial values:
  lcd.print("Hello, world!"); // prints a welcome message to the LCD
  digitalWrite(OPTO1_PIN,LOW);
  digitalWrite(OPTO2_PIN,LOW);
  digitalWrite(STATUS_PIN,LOW);
  digitalWrite(IR_STATUS_PIN,LOW);
  digitalWrite(RELAY_PIN,LOW);
  digitalWrite(LED_PIN,HIGH);
  buttonState = LOW;
  recButtonState = LOW;
  
  //Blink twice just to show it has just turned on ;-)
  digitalWrite(LED_PIN,LOW);
  digitalWrite(LED_PIN,HIGH);
  delay(500);
  digitalWrite(LED_PIN,LOW);
  delay(200);
  digitalWrite(LED_PIN,HIGH);
  delay(500);
  digitalWrite(LED_PIN,LOW);  
}

// Storage for the recorded code
int codeType = -1; // The type of code
unsigned long codeValue; // The code value if not raw
unsigned int rawCodes[RAWBUF]; // The durations if raw
int codeLen; // The length of the code
int toggle = 0; // The RC5/6 toggle state

// Stores the code for later playback
// Most of this code is just logging
void storeCode(decode_results *results) {
  codeType = results->decode_type;
  int count = results->rawlen;
  if (codeType == UNKNOWN) {
    Serial.println("Received unknown code, saving as raw");
    codeLen = results->rawlen - 1;
    // To store raw codes:
    // Drop first value (gap)
    // Convert from ticks to microseconds
    // Tweak marks shorter, and spaces longer to cancel out IR receiver distortion
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else {
        // Space
        rawCodes[i - 1] = results->rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(rawCodes[i - 1], DEC);
    }
    Serial.println("");
  }
  else {
    if (codeType == NEC) {
      Serial.print("Received NEC: ");
      if (results->value == REPEAT) {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    } 
    else if (codeType == SONY) {
      Serial.print("Received SONY: ");
    } 
    else if (codeType == RC5) {
      Serial.print("Received RC5: ");
    } 
    else if (codeType == RC6) {
      Serial.print("Received RC6: ");
    } 
    else {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results->value, HEX);
    codeValue = results->value;
    codeLen = results->bits;
  }
}

void sendCode(int repeat) {
  if (codeType == NEC) {
    if (repeat) {
      irsend.sendNEC(REPEAT, codeLen);
      Serial.println("Sent NEC repeat");
    } 
    else {
      irsend.sendNEC(codeValue, codeLen);
      Serial.print("Sent NEC ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == SONY) {
    irsend.sendSony(codeValue, codeLen);
    Serial.print("Sent Sony ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == RC5 || codeType == RC6) {
    if (!repeat) {
      // Flip the toggle bit for a new button press
      toggle = 1 - toggle;
    }
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (codeLen - 1));
    codeValue = codeValue | (toggle << (codeLen - 1));
    if (codeType == RC5) {
      Serial.print("Sent RC5 ");
      Serial.println(codeValue, HEX);
      irsend.sendRC5(codeValue, codeLen);
    } 
    else {
      irsend.sendRC6(codeValue, codeLen);
      Serial.print("Sent RC6 ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == UNKNOWN /* i.e. raw */) {
    // Assume 38 KHz
    irsend.sendRaw(rawCodes, codeLen, 38);
    Serial.println("Sent raw");
  }
}

void loop() {
  timer = millis();
  // Read buttons:
  // negation because buttons are configured with PULLUP (HIGH is the normal state)
  btn1 = !digitalRead(BTN1);
  btn2 = !digitalRead(BTN2);
  btn3 = !digitalRead(BTN3);
  btn4 = !digitalRead(BTN4);
      
  // Writing to the LCD:
  // Prints the seconds since last reset while waiting first data reception:
  if (!communicating) {
    // set the cursor to column 0, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(0, 1);
    // print the number of seconds since reset:
    seconds = millis()/1000;  
    lcd.print(seconds);
    lcdCursor = 0;
    while (seconds/10 > 0) {
      lcdCursor++;
      seconds = seconds / 10;
    }
    lcdCursor++;
  } 
  // Prints the data received from Serial port:
  if(Serial.available()){
    communicating = true;
    lcd.setCursor(0, 1);
    lcdCursor = 0;
    lcd.print("                ");
    delay(100);
    lcd.setCursor(0, 1);
    while(Serial.available() > 0){
      lcd.write(Serial.read());
      lcdCursor++;
    }
  }
  lcd.setCursor(lcdCursor+1, 1);
  if (btn1) printToAll("(btn1)");
  else if (btn2) printToAll("(btn2)");
  else if (btn3) printToAll("(btn3)");
  else if (btn4) printToAll("(btn4)");
  else lcd.print("              "); //to clear out the LCD display
  
  // If button pressed, send the code:
  buttonState = btn1; 
  if (lastButtonState == HIGH && buttonState == LOW) {
    Serial.println("Released");
    //irrecv.enableIRIn(); // Re-enable receiver
  }

  recButtonState = btn2;
  if (lastRecButtonState == LOW && recButtonState == HIGH) {
    Serial.println("Rec button pressed: release to record IR code");
  }
  if (lastRecButtonState == HIGH && recButtonState == LOW) {
    Serial.println("Recording: press button in 3 seconds...");
    recTime = timer;
    recording = 3;
    irrecv.enableIRIn(); // Re-enable receiver
  }
  if (recording) {
    if (timer - recTime > 3000) {
      recording = 0;
      Serial.println("Recorded!");
      storeCode(&results);
      irrecv.resume();
    }
    else if ((timer - recTime > 1000)&&(recording > 2)) {
      Serial.println("2");
      recording--;
    }
    else if ((timer - recTime > 2000)&&(recording > 1)) {
      Serial.println("1");
      recording--;
    }
  }

  if (buttonState) {
    Serial.println("Pressed, sending");
    digitalWrite(IR_STATUS_PIN, HIGH);
    sendCode(lastButtonState == buttonState);
    digitalWrite(IR_STATUS_PIN, LOW);
    delay(50); // Wait a bit between retransmissions
  }
  else if (recording)
    if (irrecv.decode(&results)) {
      digitalWrite(IR_STATUS_PIN, HIGH);
      storeCode(&results);
      irrecv.resume(); // resume receiver
      digitalWrite(IR_STATUS_PIN, LOW);
    }

  if (btn1||btn2) digitalWrite(STATUS_PIN,HIGH);
  else digitalWrite(STATUS_PIN,LOW);
  if (btn3||btn4) digitalWrite(LED_PIN,HIGH);
  else digitalWrite(LED_PIN,LOW);

  if (!digitalRead(BTN2)) digitalWrite(RELAY_PIN,HIGH);
  else digitalWrite(RELAY_PIN,LOW);  
  if (!digitalRead(BTN3)) digitalWrite(OPTO1_PIN,HIGH);
  else digitalWrite(OPTO1_PIN,LOW);
  if (!digitalRead(BTN4)) digitalWrite(OPTO2_PIN,HIGH);
  else digitalWrite(OPTO2_PIN,LOW);
  
  lastButtonState = buttonState;
  lastRecButtonState = recButtonState;
}

void printToAll(String text) {
  Serial.println(text);
  //lcd.setCursor(0, 1);
  lcd.print(text);
  delay(100);
}
