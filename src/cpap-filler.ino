//define pins
#define PUMP 3
int FLOAT_SWITCH = D6;

//define variables
LEDSystemTheme theme; // Enable custom theme
volatile int count = 0;
int floatState;
int pumpDurationInt;
bool pumpScheduled = true;
bool pumpStatus = false;
bool pump = false;
String pumpDuration = "60";
String currentTime;
String setTime = "10:30";

void setup()
{
  Time.zone(-5);
  theme.setColor(LED_SIGNAL_CLOUD_CONNECTED, 0x00000000); // Set LED_SIGNAL_NETWORK_ON to no color
  theme.apply();                                          // Apply theme settings
  pinMode(PUMP, OUTPUT);
  pinMode(FLOAT_SWITCH, INPUT_PULLUP);
  Particle.function("Pump", Pump);
  Particle.function("ManualPump", ManualPump);
  Particle.function("SetTime", SetTime);
  Particle.function("SetPumpDuration", SetPumpDuration);
  Particle.function("SchedulePump", SchedulePump);
  Particle.variable("CurrentTime", currentTime);
  Particle.variable("FloatState", floatState);
  Particle.variable("AutoPumpTime", setTime);
  Particle.variable("PumpDuration", pumpDuration);
  Particle.variable("PumpScheduled", pumpScheduled);
}

void loop()
{
  floatState = digitalRead(FLOAT_SWITCH);
  currentTime = Time.format(Time.now(), "%H:%M");
  pumpDurationInt = pumpDuration.toInt();

  if (currentTime == setTime && !pump && floatState == LOW)
  {
    Pump("String");
  }
  else if (count > pumpDurationInt)
  {
    stopPump();
    pumpScheduled = false;
    Particle.publish("Filling Error", "Check Water!");
  }
  else if (pump && floatState == HIGH)
  {
    stopPump();
  }
  else if (pumpScheduled && pump && floatState == LOW)
  {
    digitalWrite(PUMP, HIGH);
    count += 1;
  }
  delay(1000);
}

//local functions
void stopPump()
{
  digitalWrite(PUMP, LOW);
  Particle.publish("Stopping Pump");
  pump = false;
  count = 0;
}

//cloud functions
// Run pump
int Pump(String message)
{
  Particle.publish("Running Pump");
  pump = true;
}

int SetTime(String message)
{
  setTime = message;
}

int SetPumpDuration(String message)
{
  pumpDuration = message;
}

int SchedulePump(String message)
{
  if (!pumpScheduled)
  {
    pumpScheduled = true;
  }
  else if (pumpScheduled)
  {
    pumpScheduled = false;
  }
}

// Toggle pump on/off
int ManualPump(String message)
{
  if (!pumpStatus)
  {
    Particle.publish("Running Pump");
    pumpStatus = true;
    digitalWrite(PUMP, HIGH);
  }
  else if (pumpStatus)
  {
    pumpStatus = false;
    stopPump();
  }
}
