#define REDPOTPIN 5
#define GREENPOTPIN 4
#define BLUEPOTPIN 3
#define WHITEPOTPIN 2
#define BRIGHTNESSPOTPIN 1
#define POWERSWITCHPIN 2


#define REDOUTPIN 3
#define GREENOUTPIN 5
#define BLUEOUTPIN 6
#define WHITEOUTPIN 9
#define LEDPIN 13

int redPotValue = 0;
int greenPotValue = 0;
int bluePotValue = 0;
int whitePotValue = 0;
int brightnessPotValue = 0;

int redOutValue = 0;
int greenOutValue = 0;
int blueOutValue = 0;
int whiteOutValue = 0;

int currentPowerSwitchState = HIGH;      // the current state of the output pin
int currentPowerSwitchReading;           // the current reading from the input pin
int previousPowerSwitchReading = LOW;    // the previous reading from the input pin

unsigned long lastSwitchPushMillis = 0; // the last time the output pin was toggled
unsigned long debounceMillis = 100;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(REDOUTPIN, OUTPUT);
  pinMode(GREENOUTPIN, OUTPUT);
  pinMode(BLUEOUTPIN, OUTPUT);
  pinMode(WHITEOUTPIN, OUTPUT);

  pinMode(POWERSWITCHPIN, INPUT);
  digitalWrite(LEDPIN, currentPowerSwitchState);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long start = millis();
  readPowerSwitchState();
  unsigned long afterPowerSwitchRead = millis();
 
  redPotValue = analogRead(REDPOTPIN);
  greenPotValue = analogRead(GREENPOTPIN);
  bluePotValue = analogRead(BLUEPOTPIN);
  whitePotValue = analogRead(WHITEPOTPIN);
  brightnessPotValue = analogRead(BRIGHTNESSPOTPIN);
  unsigned long afterRead = millis(); 

      
  unsigned int scaledRedPotValue = scaleToPwmOutput(redPotValue, brightnessPotValue);
  unsigned int scaledGreenPotValue = scaleToPwmOutput(greenPotValue, brightnessPotValue);
  unsigned int scaledBluePotValue = scaleToPwmOutput(bluePotValue, brightnessPotValue);
  unsigned int scaledWhitePotValue = scaleToPwmOutput(whitePotValue, brightnessPotValue);

  unsigned long afterScaling = millis();

  if (currentPowerSwitchState == LOW) {
    analogWrite(REDOUTPIN, 0);
    analogWrite(GREENOUTPIN, 0);
    analogWrite(BLUEOUTPIN, 0);
    analogWrite(WHITEOUTPIN, 0);
  } else {
    analogWrite(REDOUTPIN, scaledRedPotValue);
    analogWrite(GREENOUTPIN, scaledGreenPotValue);
    analogWrite(BLUEOUTPIN, scaledBluePotValue);
    analogWrite(WHITEOUTPIN, scaledWhitePotValue);
  }
  unsigned long afterWriting = millis();

//  Serial.print("TIMING \n    start:               " + String(start) + 
//                      "\n    afterPowerSwitchRead " + String(afterPowerSwitchRead) +
//                      "\n    afterRead            " + String(afterRead) + 
//                      "\n    afterScaling         " + String(afterScaling) + 
//                      "\n    afterWriting         " + String(afterWriting) + "\n\n");
//  delay(1000);

//  Serial.print(" -- SCALED RGBW value: " + String(scaledRedPotValue) + ","
//      + String(scaledGreenPotValue) + ","
//      + String(scaledBluePotValue) + ","
//      + String(scaledWhitePotValue)
//      + ";\n");
}

void readPowerSwitchState() {
  currentPowerSwitchReading = digitalRead(POWERSWITCHPIN);

  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
//  unsigned long now = millis();
  if (currentPowerSwitchReading == HIGH && previousPowerSwitchReading == LOW && millis() - lastSwitchPushMillis > debounceMillis) {
    if (currentPowerSwitchState == HIGH) {
      currentPowerSwitchState = LOW;
    } else {
      currentPowerSwitchState = HIGH;
    }

    lastSwitchPushMillis = millis();
  }

  digitalWrite(LEDPIN, currentPowerSwitchState);

  previousPowerSwitchReading = currentPowerSwitchReading;
}

unsigned int scaleToPwmOutput(int colorPotValue, int brightnessPotValue) {
  return (unsigned int)((unsigned long) colorPotValue * (unsigned long) brightnessPotValue / (4096L));
}
