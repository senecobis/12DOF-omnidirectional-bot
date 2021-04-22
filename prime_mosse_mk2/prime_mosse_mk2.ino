#include <FlexiTimer2.h>
#include <Servo.h>
#include <math.h>

Servo coscia[4];
Servo femore[4];
Servo piede[4];

void setup() {
Serial.begin(9600);
}

/*Dimensioni di D-H dei giunti*/
const float coxa = 30;
const float femur = 58;
const float tibia = 82;

const float pi = 3.141592;
const float z_start = 22;
const float face_dim =71 ;
const float side_dim = 72;

/* Constants for movement */
const float z_default = 20, z_up = 30;
const float x_default = 80, x_offset = 0;
const float y_start = 0, y_step = 90, y_default = 20;

void attachment (void){
  int j=0;
  for (int i=0; i<4; i++){
  coscia[i].attach(j+2);
  femore[i].attach(j+3);
  piede[i].attach(j+4);
  j=j+3;
  }
}

void loop (){
  attachment();
  
  simple_inv_kinematics(-x_default,y_default,z_default,0);
  simple_inv_kinematics(-x_default,y_default,z_default,1);
  simple_inv_kinematics(-x_default,y_default,z_default,2);
  simple_inv_kinematics(-x_default,y_default,z_default,3);

  //move_to(-80,80,0,0);
}

void move_to (int x,int y,int z,int leg){
  float alpha,beta,gamma;
  simple_inv_kinematics(x,y,z,leg);
  //cartesian_to_polar(alpha,beta,gamma,x,y,z);
  //polar_to_servo(leg,alpha,beta,gamma);
  
  }


void simple_inv_kinematics(int px,int py,int pz,int leg){
  int gamma,alpha,beta,L,L1;
  Serial.println(px);
  Serial.println(py);
  alpha = atan2(py,px);
  Serial.println(alpha/pi*180);
  delay(1000);
  L1=sqrt(pow(px,2)+pow(py,2));
  L = sqrt(pow(pz,2)+pow(L1-coxa,2));
  beta = acos(pz/L) + acos((pow(tibia,2)-pow(femur,2)-pow(L,2))/(-2*femur*L));
  gamma = acos((pow(L,2)-pow(tibia,2)-pow(femur,2))/(-2*tibia*femur));
  polar_to_servo_mio(leg,alpha,beta,gamma);
  }


void polar_to_servo_mio(int leg, float alpha, float beta, float gamma)
{

   alpha = alpha*180/pi;
   beta = beta*180/pi;
   gamma = gamma*180/pi;
   
  if (leg == 0)
  { 
   alpha = alpha;
   beta = 180 - beta;
   gamma = gamma;
  }
  else if (leg == 1)
  {
    alpha = 180-alpha;
    beta = beta;
    gamma =180 - gamma;
  }
  else if (leg == 2)
  {
    alpha = 180-alpha;
    beta = beta;
    gamma = 180 -  gamma;
  }
  else if (leg == 3)
  {
    alpha = alpha;
    beta = 180- beta;
    gamma = gamma;
  }
  
  coscia[leg].write(alpha);
  femore[leg].write(beta);
  piede[leg].write(gamma);

//  Serial.println("angoli primo, secondo e terzo giunto rispettivamente rispettivamente");
//  Serial.println(alpha);
//  Serial.println(beta);
//  Serial.println(gamma);
//  delay(1000);
}
