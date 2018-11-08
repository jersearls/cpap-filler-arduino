#define PUMP 3
LEDSystemTheme theme; // Enable custom theme

int milliseconds;
String seconds;
bool pumpStatus = false;
bool pump;

void setup()
{
  theme.setColor(LED_SIGNAL_CLOUD_CONNECTED, 0x00000000); // Set LED_SIGNAL_NETWORK_ON to no color
  theme.apply(); // Apply theme settings
  pinMode(PUMP, OUTPUT);
  Particle.function("Pump", Pump);
  Particle.function("ManualPump", ManualPump);
}

void loop() {
  //run if pump is true
  if (pump) {
    Particle.publish("Running Pump for " + seconds + " seconds");
    digitalWrite(PUMP, HIGH);
    delay(milliseconds);
    digitalWrite(PUMP, LOW);
    Particle.publish("Stopping Pump");
    pump = false;
  }
}

//cloud functions

// Run pump with seconds argument
int Pump(String message) {
  seconds = message;
  milliseconds = seconds.toInt() * 1000;
  pump = true;
}

// Toggle pump on/off
int ManualPump(String message) {
  if (!pumpStatus) {
    Particle.publish("Running Pump");
    pumpStatus = true;
    digitalWrite(PUMP, HIGH);
  }
  else if (pumpStatus) {
    digitalWrite(PUMP, LOW);
    pumpStatus = false;
    Particle.publish("Stopping Pump");
  }
}
