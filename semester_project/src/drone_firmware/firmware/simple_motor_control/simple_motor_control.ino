// AEK-MOT-2DC40Y1 H-Bridge Connection PINs
#define Motor_driver_enA 9  // PWM
#define Motor_driver_in1 12  // Dir Motor A
#define Motor_driver_in2 13  // Dir Motor A

float cmd = 0;

void setup() {
  // Set pin modes
  pinMode(Motor_driver_enA, OUTPUT);
  pinMode(Motor_driver_in1, OUTPUT);
  pinMode(Motor_driver_in2, OUTPUT);
  
  // Set Motor Rotation Direction
  digitalWrite(Motor_driver_in1, HIGH);
  digitalWrite(Motor_driver_in2, LOW);

  Serial.begin(115200);
}

void loop() {
  if (Serial.available())
  {
    cmd = Serial.readString().toFloat();
  }
  analogWrite(Motor_driver_enA, cmd*100);
}