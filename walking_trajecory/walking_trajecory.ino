#include <Servo.h>

const int n_leg = 4;
const int n_motori = 3;

const int coxa = 30;
const int femur = 58;
const int tibia = 82;

const int z_start = 22;
const int face_dim =71;
const int side_dim = 72;

const int z_default = -20;
const int z_up = 80;
const int x_default = 50;
const int y_start = 0;
const int y_default = x_default;
const float pi = 3.14159265358979323846;

const int vel = 500;

Servo motori[n_leg][n_motori];

void setup() {
  Serial.begin(9600);
}


void loop() {
  static float alpha,beta,gamma;
  attachment();
  move_forward(10); 
  
}


void servo_service(int x, int y, int z, int leg){
  //attua il moto della singola gamba
  static float alpha,beta,gamma;
  inverseK(alpha,beta,gamma,x,y,z,leg);
  polar_to_servo(alpha,beta,gamma,leg);
  
  Serial.println(alpha);
  Serial.println(beta);
  Serial.println(gamma);
  delay(1000);
  motori[leg][0].write(alpha);
  motori[leg][1].write(beta);
  motori[leg][2].write(gamma);
  
  }

void attachment (void){
  int j=0;
  for (int i=0; i<4; i++){
    motori[i][0].attach(j+2);
    motori[i][1].attach(j+3);
    motori[i][2].attach(j+4);
    j=j+3;
  }
}

void inverseK (float& alpha, float& beta, float& gamma, float x, float y, float z, int leg){
  int L1 = sqrt(pow(x, 2) + pow(y, 2));
  int L = sqrt(pow(z,2)+pow(L1-coxa,2));
  if (L1>0){
      alpha = atan2(y, x);
  }
  else{
      alpha = atan2(-y, -x);
  }
  beta = acos(z/L) + acos((pow(tibia,2)-pow(femur,2)-pow(L,2))/(-2*femur*L));
  gamma = acos((pow(L,2)-pow(tibia,2)-pow(femur,2))/(-2*tibia*femur));

  if (z==z_up){
    gamma = 120*pi/180;
    }
  }

void polar_to_servo(float& alpha,float& beta,float& gamma, int leg){
  alpha = alpha *180/pi;
  beta = beta *180/pi;
  gamma = gamma *180/pi;
  if (leg == 0){
    alpha = alpha;
    beta = beta - 180;
    gamma = gamma;
    }
  if (leg == 1){
    alpha = 180 - alpha;
    beta = beta - 90;
    gamma = gamma + 90;
    }
  if (leg == 2){
    alpha = 180 - alpha;
    beta = beta - 90;
    gamma = 90 + gamma;
    }
  if (leg == 3){
   alpha = alpha;
   beta = beta - 180;
   gamma = gamma;
    }

}  

void reach_point(float x,float y,float z,int leg){
  static float alpha,beta,gamma;
  inverseK(alpha,beta,gamma,x,y,z,leg);
  polar_to_servo(alpha,beta,gamma,leg);
  motori[leg][0].write(alpha);
  motori[leg][1].write(beta);
  motori[leg][2].write(gamma);
}



void starting_pos(void){
    reach_point(-x_default, y_default, z_default, 0);
    reach_point(-x_default, y_default, z_default, 1);
    reach_point(x_default, y_default, z_default, 2);
    reach_point(x_default, y_default, z_default, 3);
}


void move_forward(int stepper){
    starting_pos();
    delay(vel);
    while(stepper>=0){
        stepper = stepper - 1;
        //gamba 0 e gamba 2 spingono
        //dobbiamo fare il disattach dei motori 0 delle gambe non usate, cioè 1 e 3
        motori[1][0].detach();
        motori[3][0].detach();
        //spingiamo
        reach_point(-x_default+40, y_default+20, z_default, 0);
        reach_point(x_default+20, y_default-40, z_default, 2);
        Serial.println("spingo leg 0 e 2");
        delay(vel);
        reach_point(-x_default+40, y_default+40, z_up, 0);
        reach_point(-x_default, y_default, z_default, 0);
        delay(vel);
        reach_point(x_default+20, y_default-40, z_up, 2);
        reach_point(x_default, y_default, z_default, 2);
        delay(vel);
        
        //ora attach dei motori 0 delle gambe precedentemente non usate, cioè 1 e 3
        motori[1][0].attach(5);
        motori[3][0].attach(11);
//        reach_point(-x_default+40, y_default+20, z_up, 1);
//        reach_point(-x_default+40, y_default+20, z_default, 1);
//        delay(vel);
//        reach_point(x_default+20, y_default-40, z_up, 3);
//        reach_point(x_default, y_default, z_default, 3);
//        delay(vel);
        //e spingiamo facendo il detach dei motori 0 di gamba 0 e 2
        motori[0][0].detach();
        motori[2][0].detach();
        reach_point(-x_default+40, y_default+20, z_default, 1);
        reach_point(x_default+20, y_default-40, z_default, 3);
        delay(vel);
        reach_point(-x_default+40, y_default+20, z_up, 1);
        reach_point(-x_default , y_default, z_default, 1);
        delay(vel);
        reach_point(x_default+20, y_default-40, z_up, 3);
        reach_point(x_default, y_default, z_default, 3);
        delay(vel);
        motori[0][0].attach(2);
        motori[2][0].attach(8);
        starting_pos();
    }
}


  
