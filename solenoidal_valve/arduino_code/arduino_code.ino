/*
We automate the refilling of nitrogen. 
 
 Liquid nitrogen evaporates until Rbottom gets cold, We then open the solenoid valve until Rbottom and Rtop are full
 
 Resistors will increase their value when they are at liquid nitrogen temperature. We effectively measure the resistance by creating a voltage divider. We will see a difference in measured voltage when one of the resistors is submerged in liquid nitrogen.
 */

// the setup routine runs once when you press reset:
float V_A0_threshold = 2.8; //Rtop2 is cold if V0 is above this threshold
float V_A1_threshold = 3.19; //Rbottom2 is cold if V1 is above this threshold

int relay_pin = 13; // relay module is connected to digital pin 13
int power_pin = 11; // just making another 5V source
int ground_pin = 9; // just making another ground pin

void setup() {
  Serial.begin(9600); //Allows the arduino to print to a serial monitor
  pinMode(relay_pin, OUTPUT); //set's the digital pin as output
  pinMode(power_pin, OUTPUT);
  pinMode(ground_pin, OUTPUT);
  digitalWrite(relay_pin, LOW); //the relay switch starts out being open
  digitalWrite(ground_pin, LOW); 
  digitalWrite(power_pin, HIGH);
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue0 = analogRead(A0);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage0 = sensorValue0 * (5.0 / 1023.0);
  Serial.println(voltage0); //in case you want to read out the voltage
  
  // read the input on analog pin 1:
  int sensorValue1 = analogRead(A2);
  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  float voltage1 = sensorValue1 * (5.0 / 1023.0);
  Serial.println(voltage1); //in case you want to read out the voltage
  
  if (voltage0 > V_A0_threshold && voltage1>V_A1_threshold){ //this signifies that both resistors are at liquid nitrogen temperature
    digitalWrite(relay_pin,LOW); //make the switch open (LOW in original) -> Valve Closed
    Serial.println("BOTH IN NITROGEN");
  }
 else if (voltage1 < V_A1_threshold && voltage0 < V_A0_threshold){ //this conditional signifies that the liquid nitrogen level is below Rbottom
    digitalWrite(relay_pin, HIGH); //make the switch close (HIGH in original code) -> Valve Open
    Serial.println("BOTH OUT OF NITROGEN");
  }
 else{
  Serial.println("BOTTOM IN. TOP OUT.");
 }

  delay(1000); //wait 1s (1000ms) so that the arduino doesn't work so hard.
}

