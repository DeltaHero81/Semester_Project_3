
// AEK-MOT-2DC40Y1 H-Bridge Connection PINs
#define Motor_driver_enA 9  // PWM
#define Motor_driver_enB 11  // PWM
#define Motor_driver_in3 7  // Dir Motor A
#define Motor_driver_in4 8  // Dir Motor A
#define Motor_driver_in1 12  // Dir Motor A
#define Motor_driver_in2 13  // Dir Motor A

// Interpret Serial Messages
bool is_right_wheel_cmd = false;
bool is_left_wheel_cmd = false;
bool is_right_wheel_forward = true;
bool is_left_wheel_forward = true;
char value[] = "00.00";
uint8_t value_idx = 0;
bool is_cmd_complete = false;

// PID
// Setpoint - Desired
double right_wheel_cmd_vel = 0.0;     // rad/s
double left_wheel_cmd_vel = 0.0;      // rad/s
// Input - Measurement
double right_wheel_meas_vel = 0.0;    // rad/s
double left_wheel_meas_vel = 0.0;     // rad/s
// Output - Command
double right_wheel_cmd = 0.0;         // 0-255
double left_wheel_cmd = 0.0;          // 0-255
// Tuning
double Kp_r = 11.5;
double Ki_r = 7.5;
double Kd_r = 0.1;
double Kp_l = 12.8;
double Ki_l = 8.3;
double Kd_l = 0.1;
// Controller
PID rightMotor(&right_wheel_meas_vel, &right_wheel_cmd, &right_wheel_cmd_vel, Kp_r, Ki_r, Kd_r, DIRECT);
PID leftMotor(&left_wheel_meas_vel, &left_wheel_cmd, &left_wheel_cmd_vel, Kp_l, Ki_l, Kd_l, DIRECT);

void setup() {
  // Init L298N H-Bridge Connection PINs
  pinMode(Motor_driver_enA, OUTPUT);
  pinMode(Motor_driver_enB, OUTPUT);
  pinMode(Motor_driver_in1, OUTPUT);
  pinMode(Motor_driver_in2, OUTPUT);
  pinMode(Motor_driver_in3, OUTPUT);
  pinMode(Motor_driver_in4, OUTPUT);

  // Set Motor Rotation Direction
  digitalWrite(Motor_driver_in1, HIGH);
  digitalWrite(Motor_driver_in2, LOW);
  digitalWrite(Motor_driver_in3, HIGH);
  digitalWrite(Motor_driver_in4, LOW);

  rightMotor.SetMode(AUTOMATIC);
  leftMotor.SetMode(AUTOMATIC);
  Serial.begin(115200);
}

void loop() {
  // Read and Interpret Wheel Velocity Commands
  if (Serial.available())
  {
    char chr = Serial.read();
    // Right Wheel Motor
    if(chr == 'r')
    {
      is_right_wheel_cmd = true;
      is_left_wheel_cmd = false;
      value_idx = 0;
      is_cmd_complete = false;
    }
    // Left Wheel Mo tor
    else if(chr == 'l')
    {
      is_right_wheel_cmd = false;
      is_left_wheel_cmd = true;
      value_idx = 0;
    }
    // Positive direction
    else if(chr == 'p')
    {
      if(is_right_wheel_cmd && !is_right_wheel_forward)
      {
        // change the direction of the rotation
        
        is_right_wheel_forward = true;
      }
      else if(is_left_wheel_cmd && !is_left_wheel_forward)
      {
        // change the direction of the rotation
        digitalWrite(Motor_driver_in1, HIGH - digitalRead(Motor_driver_in1));
        digitalWrite(Motor_driver_in2, HIGH - digitalRead(Motor_driver_in2));
        digitalWrite(Motor_driver_in3, HIGH - digitalRead(Motor_driver_in3));
        digitalWrite(Motor_driver_in4, HIGH - digitalRead(Motor_driver_in4));
        is_left_wheel_forward = true;
      }
    }
    // Negative direction
    else if(chr == 'n')
    {
      if(is_right_wheel_cmd && is_right_wheel_forward)
      {
        // change the direction of the rotation

        is_right_wheel_forward = false;
      }
      else if(is_left_wheel_cmd && is_left_wheel_forward)
      {
        // change the direction of the rotation
        digitalWrite(Motor_driver_in1, HIGH - digitalRead(Motor_driver_in1));
        digitalWrite(Motor_driver_in2, HIGH - digitalRead(Motor_driver_in2));
        digitalWrite(Motor_driver_in3, HIGH - digitalRead(Motor_driver_in3));
        digitalWrite(Motor_driver_in4, HIGH - digitalRead(Motor_driver_in4));
        is_left_wheel_forward = false;
      }
    }
    // Separator
    else if(chr == ',')
    {
      if(is_right_wheel_cmd)
      {
        right_wheel_cmd_vel = atof(value);
      }
      else if(is_left_wheel_cmd)
      {
        left_wheel_cmd_vel = atof(value);
        is_cmd_complete = true;
      }
      // Reset for next command
      value_idx = 0;
      value[0] = '0';
      value[1] = '0';
      value[2] = '.';
      value[3] = '0';
      value[4] = '0';
      value[5] = '\0';
    }
    // Command Value
    else
    {
      if(value_idx < 5)
      {
        value[value_idx] = chr;
        value_idx++;
      }
    }
  }
}
