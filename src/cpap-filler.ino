//define pins
#define PUMP 3
int FLOAT_SWITCH = D6;

//define variables
LEDSystemTheme theme; // Enable custom theme
volatile int count ;
bool deviceError = false;
bool pumpStatus = false;
bool pump;

void setup()
{
  theme.setColor(LED_SIGNAL_CLOUD_CONNECTED, 0x00000000); // Set LED_SIGNAL_NETWORK_ON to no color
  theme.apply(); // Apply theme settings
  pinMode(PUMP, OUTPUT);
  pinMode(FLOAT_SWITCH, INPUT_PULLUP);
  Particle.function("Pump", Pump);
  Particle.function("ManualPump", ManualPump);
  Particle.variable("Device Error Mode", deviceError) ;
}

void loop() {
  int floatState = digitalRead( FLOAT_SWITCH );
  if (count > 3 ) {
    stopPump();
    deviceError = true;
    Particle.publish("Filling Error: Chk H2O & Power Cycle");
  } else if (floatState == HIGH && count > 0){
    stopPump();
  } else if (!deviceError && pump && floatState == LOW) {
    digitalWrite(PUMP, HIGH);
    count += 1;
  }
  delay(1000);
}

//local functions
void stopPump() {
  digitalWrite(PUMP, LOW);
  Particle.publish("Stopping Pump");
  pump = false;
  count = 0;
}

//cloud functions
// Run pump
int Pump(String message) {
  Particle.publish("Running Pump");
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
