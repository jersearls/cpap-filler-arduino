//define pins
#define PUMP A0
int FLOAT_SWITCH = D6;

//define variables
LEDSystemTheme theme; // Enable custom theme
volatile int count = 0;
int floatState = 1;
int pumpDurationInt;
bool pumpScheduled = true;
bool fillFromEmpty = false;
bool pump = false;
String pumpDuration = "60";
String currentTime;
String setTime = "10:30";

void setup()
{
  pinMode(PUMP, OUTPUT);
  digitalWrite(PUMP, LOW);
  Time.zone(-5);
  Time.beginDST();
  theme.setColor(LED_SIGNAL_CLOUD_CONNECTED, 0x00000000); // Set LED_SIGNAL_NETWORK_ON to no color
  theme.apply();                                          // Apply theme settings
  pinMode(FLOAT_SWITCH, INPUT_PULLUP);
  Particle.function("Pump", Pump);
  Particle.function("FillFromEmpty", FillFromEmpty);
  Particle.function("SetDailyPumpTime", SetDailyPumpTime);
  Particle.function("SetPumpDuration", SetPumpDuration);
  Particle.function("ToggleDailyPump", ToggleDailyPump);
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

  if (floatState == HIGH)
  {
    stopPump();
  }
  else if (!fillFromEmpty && count > pumpDurationInt)
  {
    stopPump();
    pumpScheduled = false;
    Particle.publish("Filling Error", "Check Water!");
  }
  else if (currentTime == setTime && !pump && floatState == LOW)
  {
    Pump("String");
  }
  else if ((pumpScheduled || fillFromEmpty) && pump && floatState == LOW)
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
  Particle.publish("Info", "Stopping Pump");
  fillFromEmpty = false;
  pump = false;
  count = 0;
}

//cloud functions
// Run pump
int Pump(String message)
{
  Particle.publish("Info", "Running Pump");
  pump = true;
}

int SetDailyPumpTime(String message)
{
  setTime = message;
}

int SetPumpDuration(String message)
{
  pumpDuration = message;
}

int ToggleDailyPump(String message)
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
int FillFromEmpty(String message)
{
  Particle.publish("Info", "Filling From Empty");
  fillFromEmpty = true;
  Pump("String");
}
