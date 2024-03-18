/*
 * TrafficLightController
 *
 * The program simulates a basic traffic light system with green, yellow, red, and red-yellow phases
 * for cars, along with corresponding pedestrian signals.
 *
 * Developed by: Cristian Felipe Castillo Barrero
 * Date: 2024-03-19
 *
 * Pin 10: Red LED for Cars
 * Pin 9: Yellow LED for Cars
 * Pin 8: Green LED for Cars
 * Pin 3: Red LED for Pedestrians
 * Pin 2: Green LED for Pedestrians
 * Pin 7: LED for Flashing
 * Pin 5: Push Button
 * Pin 12: Trigger Pin for Distance Sensor
 * Pin 13: Echo Pin for Distance Sensor
 * Pin GND (Ground): ground rail (-) on the breadboard
 * Pin 5V: (+) rail on the breadboard
 * 
*/


const int redCar = 10;       // Pin for the red light of cars
const int yellowCar = 9;    // Pin for the yellow light of cars
const int greenCar = 8;     // Pin for the green light of cars

const int redWalkers = 3;   // Pin for the red light of pedestrians
const int greenWalkers = 2;// Pin for the green light of pedestrians

const int flashLED = 7;     // Pin for the flash LED

const int pushButton = 5;  // Pin for the push button

const int dSensorTrig = 12;  // Pin for the trigger of distance sensor
const int dSensorEcho = 13;  // Pin for the echo of distance sensor


enum TrafficLightStates {GREEN_PHASE, YELLOW_PHASE, RED_PHASE, RED_YELLOW_PHASE};

// Initial state: Green phase for cars
TrafficLightStates trafficLightState = GREEN_PHASE;

int buttonState;

// Time stamp variables
unsigned long lastGreenPhaseTimeStamp = 0;
unsigned long milliCounter; // Required for delay
unsigned long greenPhaseDuration;


/**
 * @brief Generate a trigger signal for the distance sensor and check if the distance is lower than 10cm to activate a blitzer.
 *
 * @param None.
 * 
 * @details This function sets the trigger pin of the distance sensor to HIGH to initiate a measurement signal,
 * delays for a short duration to ensure the trigger signal is stable, and then sets the trigger pin back to LOW
 * to end the measurement signal. It then checks if the distance is lower than 10cm and activates a blitzer accordingly.
 * 
 * @note Ensure that the appropriate pins are configured for the distance sensor, blitzer LED, and that the appropriate delay is applied.
 * 
 * @return None.
 */
void triggerAndCheckDistance()
{
  // Set the trigger pin of the distance sensor to HIGH to initiate a measurement signal.
  digitalWrite(dSensorTrig, HIGH);
  // Delay for a short duration to ensure the trigger signal is stable.
  delayMicroseconds(1000);
  // Set the trigger pin of the distance sensor back to LOW to end the measurement signal.
  digitalWrite(dSensorTrig, LOW);

  // If the distance is lower than 10 cm, activate blitzer
  if (pulseIn(dSensorEcho, HIGH) / 58 <= 10)
  {
    digitalWrite(flashLED, HIGH);
    delay(10);
    digitalWrite(flashLED, LOW);

    // Wait 1 second for the next action
    delay(1000);
  }
}


/**
 * @brief Set the traffic lights to display the green phase.
 *
 * @param None.
 * 
 * @details This function turns on the green light for cars and the red light for pedestrians, indicating the green phase.
 * 
 * @note Ensure that the appropriate pins are configured for the traffic lights.
 * 
 * @return None.
 */
void greenPhase()
{
  digitalWrite(redCar, LOW);
  digitalWrite(yellowCar, LOW);
  digitalWrite(greenCar, HIGH);

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);
}


/**
 * @brief Set the traffic lights to display the yellow phase.
 *
 * @param None.
 * 
 * @details This function turns on the yellow light for cars and the red light for pedestrians, indicating the yellow phase.
 * It then delays for 2 seconds before transitioning to the red phase.
 * 
 * @note Ensure that the appropriate pins are configured for the traffic lights.
 * 
 * @return None.
 */
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


/**
 * @brief Set the traffic lights to display the red phase.
 *
 * @param None.
 * 
 * @details This function turns on the red light for cars and the green light for pedestrians, indicating the red phase.
 * It activates a blitzer for cars for 15 seconds.
 * After that, it transitions to the red-yellow phase.
 * 
 * @note Ensure that the appropriate pins are configured for the traffic lights and sensors.
 * 
 * @return None.
 */
void redPhase()
{
  digitalWrite(redCar, HIGH);
  digitalWrite(yellowCar, LOW);
  digitalWrite(greenCar, LOW);

  digitalWrite(redWalkers, LOW);
  digitalWrite(greenWalkers, HIGH);

  // Speed camera // 
  milliCounter = millis();
  while (millis() - milliCounter <= 12000)
  {
    triggerAndCheckDistance();
  }

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);

  // Speed camera //
  milliCounter = millis();
  while (millis() - milliCounter <= 3000)
  {
    triggerAndCheckDistance();
  }

  trafficLightState = RED_YELLOW_PHASE;
}


/**
 * @brief Set the traffic lights to display the red-yellow phase.
 *
 * @param None.
 * 
 * @details This function turns on both the red and yellow lights for cars and the red light for pedestrians, indicating the red-yellow phase.
 * It then delays for 3 seconds before transitioning back to the green phase and updating the timestamp.
 * 
 * @note Ensure that the appropriate pins are configured for the traffic lights.
 * 
 * @return None.
 */
void redYellowPhase()
{
  digitalWrite(redCar, HIGH);
  digitalWrite(yellowCar, HIGH);
  digitalWrite(greenCar, LOW);

  digitalWrite(redWalkers, HIGH);
  digitalWrite(greenWalkers, LOW);

  // Speed camera //
  milliCounter = millis();
  while (millis() - milliCounter <= 3000)
  {
    triggerAndCheckDistance();
  }

  trafficLightState = GREEN_PHASE;
  lastGreenPhaseTimeStamp = millis();
}


/**
 * @brief Set up the pin modes for the components used in the traffic light system.
 *
 * @param None.
 * 
 * @details This function initializes the pin modes for the push button, car traffic lights (red, yellow, green),
 * pedestrian traffic lights (red, green), flash LED, and distance sensor (trigger and echo pins).
 * It configures the push button pin as INPUT_PULLUP to enable internal pull-up resistor.
 * Other pins are set as OUTPUT for controlling various components.
 * 
 * @note Make sure to connect the components to the correct pins specified in the global variables.
 * 
 * @return None.
 */
void setup()
{
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


/**
 * @brief Main loop function to control the traffic light system.
 *
 * @param None.
 * 
 * @details This function continuously loops through different traffic light phases based on the current state.
 * It calls specific functions for each phase (green, yellow, red, red-yellow).
 * It also checks for button press to manually trigger the transition to the yellow phase.
 * If the green phase duration exceeds or reaches 30 seconds, it waits for 10 seconds before transitioning to yellow.
 * If the button is pressed during the green phase (less than 30 seconds), the time difference is calculated and used to determine the delay before switching to yellow.
 * 
 * @note Ensure that the button pin is correctly connected and configured.
 * 
 * @return None.
 */
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

  // Check button state to manually trigger transition to yellow phase
  buttonState = digitalRead(pushButton);

  // Transition to yellow phase based on button press and green phase duration
  if (buttonState == HIGH)
  {
    // Calculate the duration of the current green phase
    unsigned long greenPhaseDuration = millis() - lastGreenPhaseTimeStamp;

    // If green phase duration exceeds or reaches 30 seconds, wait 10 seconds before transitioning to yellow
    if (greenPhaseDuration >= 30000)
    {
      delay(10000);
      trafficLightState = YELLOW_PHASE;
    }
    // If button is pressed before green phase duration reaches 30 seconds, calculate remaining time and wait before transitioning to yellow
    else
    {
      delay(30000 - greenPhaseDuration);
      trafficLightState = YELLOW_PHASE;
    }
  }
}
