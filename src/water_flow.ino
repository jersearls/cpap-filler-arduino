byte sensorInterrupt = 2;  // 0 = digital pin 2
byte sensorPin       = 2;

volatile int pulseCount;

void setup()
{
  Serial.begin(9600);

  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;

  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
}
// 1,380 pulses per Liter?
// other source 2760 pulses per liter

void loop()
{
    Serial.print("PulseCount: ");
    Serial.println(int(pulseCount));  // Print the integer part of the variable
    delay(1000);
}

void pulseCounter()
{
  pulseCount++;
}
