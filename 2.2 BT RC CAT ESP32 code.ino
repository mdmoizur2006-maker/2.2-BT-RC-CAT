#include <BluetoothSerial.h>
BluetoothSerial SerialBT;

const int pins[] = {2, 4, 5, 18};  // Controlled IO pins
bool timerEnabled = true;
unsigned long lastCommandTime = 0;
const unsigned long timerDuration = 200; // 0.5 seconds
bool pinsOn = false;
char lastCommand = '\0';

void setup() {
  Serial.begin(115200);
  SerialBT.begin("MOIZ_RC_CAR");
  for (int pin : pins) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  Serial.println("Bluetooth device is ready to pair");
}

void loop() {
  if (SerialBT.available()) {
    char cmd = SerialBT.read();
    Serial.print("Received Command: ");
    Serial.println(cmd);

    if (cmd == 'T') {
      timerEnabled = !timerEnabled;
      Serial.println(timerEnabled ? "Timer Enabled (0.5s)" : "Timer Disabled");

      // If timer is now ON, immediately turn off any previously active pins
      if (timerEnabled) {
        turnOffAllPins();
        pinsOn = false;
        Serial.println("Timer ON → Previous pins turned OFF");
      }
      return;
    }

    // Always turn off pins before executing a new command
    turnOffAllPins();
    executeCommand(cmd);
    lastCommand = cmd;

    if (timerEnabled) {
      lastCommandTime = millis();
      pinsOn = true;
    }
  }

  // Auto-turn-off if timer is enabled and 0.5s has passed
  if (timerEnabled && pinsOn && millis() - lastCommandTime >= timerDuration) {
    turnOffAllPins();
    Serial.print("Timer Expired: Pins turned OFF for command ");
    Serial.println(lastCommand);
    pinsOn = false;
  }
}

void executeCommand(char cmd) {
  switch (cmd) {
    case 'F': digitalWrite(2, HIGH); break;
    case 'B': digitalWrite(4, HIGH); break;
    case 'R': digitalWrite(5, HIGH); break;
    case 'L': digitalWrite(18, HIGH); break;
    case '1': digitalWrite(2, HIGH); digitalWrite(18, HIGH); break;
    case '0': digitalWrite(2, HIGH); digitalWrite(5, HIGH); break;
    case '2': digitalWrite(4, HIGH); digitalWrite(18, HIGH); break;
    case '3': digitalWrite(4, HIGH); digitalWrite(5, HIGH); break;
    default:
      Serial.println("Unknown Command");
      break;
  }
}

void turnOffAllPins() {
  for (int pin : pins) {
    digitalWrite(pin, LOW);
  }
}
