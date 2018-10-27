Servo catArm;  // create servo object to control a servo

#define PUMP 3

void setup()
{
  pinMode(PUMP, OUTPUT);
  Particle.function("Pump", Pump) ;
}

// TOGGLE PUMP
int Pump(String message) {
  if (message == "0") {
    digitalWrite(PUMP, LOW);
  }
  else if (message == "1") {
    digitalWrite(PUMP, HIGH);
  }
}
