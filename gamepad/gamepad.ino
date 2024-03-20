#include <Dictionary.h>

//Digital Pins
const int pinStart = 0;
const int pinSelect = 1;
const int pinA = 2;
const int pinB = 3;
const int pinX = 4;
const int pinY = 5;

const int pinUp = 6;
const int pinDown = 7;
const int pinLeft = 8;
const int pinRight = 9;

const int digitalPins[] = { pinStart, pinSelect, pinA, pinB, pinX, pinY };
const int digitalPinCount = sizeof(digitalPins) / sizeof(digitalPins[0]);

const int hatPins[] = { pinUp, pinDown, pinLeft, pinRight };
const int hatPinCount = 4;

//Analog Pins
const int analogLeftX = A0;
const int analogLeftY = A1;
const int analogRightX = A2;
const int analogRightY = A3;

byte allButtons[digitalPinCount];
byte prevButtons[digitalPinCount];

byte hatButtons[hatPinCount];
byte prevHatButtons[hatPinCount];
Dictionary<String, int> hatDictionary = Dictionary<String, int>();

void setup() {
  Serial.begin(9600);
  Serial.println("Initialization Start:");

  Joystick.useManualSend(true);
  for (int i = 0; i < digitalPinCount; i++) {
    pinMode(digitalPins[i], INPUT_PULLUP);
    Serial.print("Digital Pin ");
    Serial.print(digitalPins[i]);
    Serial.println(" set to INPUT_PULLUP...");
  }

  for (int i = 0; i < hatPinCount; i++) {
    pinMode(hatPins[i], INPUT_PULLUP);
    Serial.print("Hat Pin ");
    Serial.print(hatPins[i]);
    Serial.println(" set to INPUT_PULLUP...");
  }

  initializeHatDictionary();

  Serial.println("Initialization Complete!");
}

void loop() {
  //Update Left Joystick
  //int mappedAnalogLeftX = map(analogRead(analogLeftX), 220, 1024, -512, 512);
  //Serial.println(mappedAnalogLeftX);
  Joystick.X(analogRead(analogLeftX));
  //int mappedAnalogLeftY = map(analogRead(analogLeftY), 220, 1024, -512, 512);
  Joystick.Y(analogRead(analogLeftY));

  //Update Right Joystick
  //int mappedAnalogRightX = map(analogRead(analogRightX), 220, 1024, 0, 1023);
  Joystick.Z(analogRead(analogRightX));
  //int mappedAnalogRightY = map(analogRead(analogRightY), 220, 1024, 0, 1023);
  Joystick.Zrotate(analogRead(analogRightY));

  //Update Digital Button State
  for (int i = 0; i < digitalPinCount; i++) {
    if (digitalRead(digitalPins[i])) {
      allButtons[i] = 0;
    } else {
      allButtons[i] = 1;
    }
    Joystick.button(i + 1, allButtons[i]);
  }

  //Update Hat Button State
  for (int i = 0; i < hatPinCount; i++) {
    if (digitalRead(hatPins[i])) {
      hatButtons[i] = 0;
    } else {
      hatButtons[i] = 1;
    }
  }

  //Update Hat Angle
  Joystick.hat(evaluateCurrentHatAngle());

  //Send Joystick Data
  Joystick.send_now();

  ///////////////////
  //   MONITORING  //
  ///////////////////
  boolean anyChange = false;
  for (int i = 0; i < digitalPinCount; i++) {
    if (allButtons[i] != prevButtons[i]) {
      anyChange = true;
    }
    prevButtons[i] = allButtons[i];
  }

  for (int i = 0; i < hatPinCount; i++) {
    if (hatButtons[i] != prevHatButtons[i]) {
      anyChange = true;
    }
    prevHatButtons[i] = hatButtons[i];
  }

  if (anyChange) {
    Serial.print("Digital Buttons: ");
    for (int i = 0; i < digitalPinCount; i++) {
      Serial.print(allButtons[i], DEC);
    }
    Serial.println();
    Serial.print("Hat Buttons: ");
    for (int i = 0; i < hatPinCount; i++) {
      Serial.print(hatButtons[i], DEC);
    }
    Serial.println();
  }


  delay(5);
}

int evaluateCurrentHatAngle() {
  String byteString;
  for (int i = 0; i < hatPinCount; i++) {
    byteString += hatButtons[i];
  }

  return byteString != NULL ? hatDictionary.get(byteString) : -1;
}

void initializeHatDictionary() {
  Serial.println("Initializing Hat Dictionary:");

  hatDictionary.set("0000", -1);  //unset
  Serial.print("[UNSET] - ");
  Serial.print("0000 - ");
  Serial.println(hatDictionary.get("0000"));

  hatDictionary.set("1000", 0);  //up
  Serial.print("[UP] - ");
  Serial.print("1000 - ");
  Serial.println(hatDictionary.get("1000"));

  hatDictionary.set("1001", 45);  //up-right
  Serial.print("[UP-RIGHT] - ");
  Serial.print("1001 - ");
  Serial.println(hatDictionary.get("1001"));

  hatDictionary.set("0001", 90);  //right
  Serial.print("[RIGHT] - ");
  Serial.print("0001 - ");
  Serial.println(hatDictionary.get("0001"));

  hatDictionary.set("0101", 135);  //down-right
  Serial.print("[DOWN-RIGHT] - ");
  Serial.print("0101 - ");
  Serial.println(hatDictionary.get("0101"));

  hatDictionary.set("0100", 180);  //down
  Serial.print("[DOWN] - ");
  Serial.print("0100 - ");
  Serial.println(hatDictionary.get("0100"));

  hatDictionary.set("0110", 225);  //down-left
  Serial.print("[DOWN-LEFT] - ");
  Serial.print("0110 - ");
  Serial.println(hatDictionary.get("0110"));

  hatDictionary.set("0010", 270);  //left
  Serial.print("[LEFT] - ");
  Serial.print("0010 - ");
  Serial.println(hatDictionary.get("0010"));

  hatDictionary.set("1010", 315);  //up-left
  Serial.print("[UP-LEFT] - ");
  Serial.print("1010 - ");
  Serial.println(hatDictionary.get("1010"));
}
