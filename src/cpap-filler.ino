#define PUMP 3
LEDSystemTheme theme; // Enable custom theme

int seconds;
bool pumpStatus = false;

void setup()
{
  theme.setColor(LED_SIGNAL_CLOUD_CONNECTED, 0x00000000); // Set LED_SIGNAL_NETWORK_ON to no color
  theme.apply(); // Apply theme settings
  pinMode(PUMP, OUTPUT);
  Particle.function("Pump", Pump);
  Particle.function("ManualPump", ManualPump);
}

// Run pump with seconds argument
int Pump(String message) {
  seconds = message.toInt() * 1000;

  if (seconds == 0) {
    digitalWrite(PUMP, LOW);
  }
  else if (seconds >= 1) {
    Particle.publish("Running Pump for " + message + " seconds");
    digitalWrite(PUMP, HIGH);
    delay(seconds);
    digitalWrite(PUMP, LOW);
    Particle.publish("Stopping Pump");
  }
}

// TOGGLE PUMP ON/OFF
int ManualPump(String message) {
  if (pumpStatus == false) {
    Particle.publish("Running Pump");
    pumpStatus = true;
    digitalWrite(PUMP, HIGH);
  }
  else if (pumpStatus == true) {
    digitalWrite(PUMP, LOW);
    Particle.publish("Stopping Pump");
    pumpStatus = false;
  }
}
