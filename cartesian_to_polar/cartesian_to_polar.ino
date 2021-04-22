#include <Servo.h>    //to define and control servos
#include <FlexiTimer2.h>//to set a timer to manage all servos
/* Servos --------------------------------------------------------------------*/
//define 12 servos for 4 legs
Servo servo[4][3];
//define servos' ports
const int servo_pin[4][3] = { {2, 3, 4}, {5, 6, 7}, {8, 9, 10}, {11, 12, 13} };
/* Size of the robot ---------------------------------------------------------*/
const float length_a = 82;
const float length_b = 58;
const float length_c = 30;
const float length_side = 71;
const float z_absolute = -28;
/* Constants for movement ----------------------------------------------------*/
const float z_default = -52, z_up = -31.2, z_boot = z_absolute;
const float x_default = 64.48, x_offset = 0;
const float y_start = 0, y_step = 90;
const float y_default = x_default;
const float pi = 3.1415926;


Servo femore[4];
Servo tibia[4];
Servo piede[4];
  

void setup() {
  Serial.begin(9600);
 

}  
void loop() { 
  static float alpha=0, beta=0, gamma=0; 
   attachment(); 
  cartesian_to_polar(alpha,beta,gamma,x_default,y_default,z_default);
  polar_to_servo(0,alpha,beta,gamma);
}

void attachment (void){
  int j=0;
  for (int i=0; i<4; i++){
  femore[i].attach(j+2);
  tibia[i].attach(j+3);
  piede[i].attach(j+4);
  j=j+3;
  }
}

void cartesian_to_polar(volatile float &alpha, volatile float &beta, volatile float &gamma, volatile float x, volatile float y, volatile float z)
{
  //calculate w-z degree
  float v, w;
  w = (x >= 0 ? 1 : -1) * (sqrt(pow(x, 2) + pow(y, 2)));
  v = w - length_c;
  alpha = atan2(z, v) + acos((pow(length_a, 2) - pow(length_b, 2) + pow(v, 2) + pow(z, 2)) / 2 / length_a / sqrt(pow(v, 2) + pow(z, 2)));
  beta = acos((pow(length_a, 2) + pow(length_b, 2) - pow(v, 2) - pow(z, 2)) / 2 / length_a / length_b);
  //calculate x-y-z degree
  gamma = (w >= 0) ? atan2(y, x) : atan2(-y, -x);
  //trans degree pi->180
  alpha = alpha / pi * 180;
  beta = beta / pi * 180;
  gamma = gamma / pi * 180;

  
  Serial.println("gli angoli sono");
  Serial.println(alpha);
  Serial.println(beta);
  Serial.println(gamma);
  delay(1000);
  
  
}

/*
  - trans site from polar to microservos
  - mathematical model map to fact
  - the errors saved in eeprom will be add
   ---------------------------------------------------------------------------*/
void polar_to_servo(int leg, float alpha, float beta, float gamma)
{
  if (leg == 0)
  {
    alpha = 90 - alpha;
    beta = beta;
    gamma += 90;
  }
  else if (leg == 1)
  {
    alpha += 90;
    beta = 180 - beta;
    gamma = 90 - gamma;
  }
  else if (leg == 2)
  {
    alpha += 90;
    beta = 180 - beta;
    gamma = 90 - gamma;
  }
  else if (leg == 3)
  {
    alpha = 90 - alpha;
    beta = beta;
    gamma += 90;
  }

  femore[leg].write(alpha);
  tibia[leg].write(beta);
  piede[leg].write(gamma);
}
