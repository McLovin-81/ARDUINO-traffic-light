const int redCar = 2;
const int yellowCar = 4;
const int greenCar = 7;

const int redWalkers = 8;
const int greenWalkers = 10;

const int flashLED = 3;

const int pushButton = 12;

const int dSensorTrig = 5;
const int dSensorEcho = 6;

enum TrafficLightStates {GREEN_PHASE, YELLOW_PHASE, RED_PHASE, RED_YELLOW_PHASE};

TrafficLightStates trafficLightState = GREEN_PHASE;

int buttonState = 0;

// Time stamp variables
unsigned long lastGreenPhaseTimeStamp = 0;
unsigned long milliCounter = 0;
unsigned long timeDifference;

void greenPhase()
{
  digitalWrite(redCar, LOW);
  digitalWrite(yellowCar, LOW);
  digitalWrite(greenCar, HIGH);

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);
}

void yellowPhase()
{
  digitalWrite(redCar, LOW);
  digitalWrite(yellowCar, HIGH);
  digitalWrite(greenCar, LOW);

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);

  delay(2000);
  trafficLightState = RED_PHASE;
}

void redPhase()
{
  digitalWrite(redCar, HIGH);
  digitalWrite(yellowCar, LOW);
  digitalWrite(greenCar, LOW);

  digitalWrite(redWalkers, LOW);
  digitalWrite(greenWalkers, HIGH);

  // Delay 12 sec. //
  milliCounter = millis();
  while (millis() - milliCounter <= 12000)
  {
    // Make blitzer
    digitalWrite(dSensorTrig, HIGH);
    delayMicroseconds(1000);
    digitalWrite(dSensorTrig, LOW);

    // If the distance is lowert than 100 cm, make blitzer
    if (pulseIn(dSensorEcho, HIGH) / 58 <= 100)
    {
      digitalWrite(flashLED, HIGH);
      delay(10);
      digitalWrite(flashLED, LOW);

      // Wait 1 seconds for the next car
      delay(1000);
    }
  }

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);

  
  // Delay 3 sec. //
  milliCounter = millis();
  while (millis() - milliCounter <= 3000)
  {
    // Make blitzer
    digitalWrite(dSensorTrig, HIGH);
    delayMicroseconds(1000);
    digitalWrite(dSensorTrig, LOW);

    // If the distance is lowert than 100 cm, make blitzer
    if (pulseIn(dSensorEcho, HIGH) / 58 <= 100)
    {
      digitalWrite(flashLED, HIGH);
      delay(10);
      digitalWrite(flashLED, LOW);

      // Wait 1 seconds for the next car
      delay(1000);
    }
  }

  trafficLightState = RED_YELLOW_PHASE;
}

void redYellowPhase()
{
  digitalWrite(redCar, HIGH);
  digitalWrite(yellowCar, HIGH);
  digitalWrite(greenCar, LOW);

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);

  delay(3000);
  trafficLightState = GREEN_PHASE;
  lastGreenPhaseTimeStamp = millis(); // NEW

}


void setup()
{
  Serial.begin(9600);
  pinMode(pushButton, INPUT_PULLUP);
  
  pinMode(redCar, OUTPUT);
  pinMode(yellowCar, OUTPUT);
  pinMode(greenCar, OUTPUT);

  pinMode(redWalkers, OUTPUT);
  pinMode(greenWalkers, OUTPUT);

  pinMode(flashLED, OUTPUT);

  pinMode(dSensorTrig, OUTPUT);
  pinMode(dSensorEcho, INPUT);
}


void loop()
{
  switch(trafficLightState)
  {
    case GREEN_PHASE:
      greenPhase();
      break;
    case YELLOW_PHASE:
      yellowPhase();
      break;
    case RED_PHASE:
      redPhase();
      break;
    case RED_YELLOW_PHASE:
      redYellowPhase();
      break;
  }


  buttonState = digitalRead(pushButton);
  if (buttonState == HIGH)
  {
    unsigned long greenTimePhaseDuration = millis() - lastGreenPhaseTimeStamp;

    if (greenTimePhaseDuration >= 30000)
    {
      delay(10000);
      trafficLightState = YELLOW_PHASE;
    }
    else
    {
      delay(30000 - greenTimePhaseDuration);
      trafficLightState = YELLOW_PHASE;
    }
  }
}


/*
TEST switch to green

  FIRST CYCLE

    1. Press button after 40 seconds      PASS!!
      YELLOW_PHASE starts after 10 seconds -> start by second 50
    
    2. Press button after 10 seconds      PASS!!
      YELLOW_PHASE starts after 20 seconds -> start by second 30

  SECOND CYCLE

    1. Press button after 40 seconds      PASS!!
      YELLOW_PHASE starts after 10 seconds
    
    2. Press button after 10 seconds      PASS!!
      YELLOW_PHASE starts after 20 seconds
*/