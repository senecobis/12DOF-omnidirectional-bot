#include <Servo.h>
const int n_leg = 4;
const int n_motori = 3;
Servo motori[n_leg][n_motori];

void setup() {
}

void loop() {
  
attachment();
//testing_angoli_motori(); 
motori[0][0].write(135);
motori[0][1].write(44);
motori[0][2].write(37);
}


void testing_angoli_motori(void){
  
for (int i = 0; i<n_leg; i++){
  for(int j = 1; j<n_motori; j++)
  if(i==0 || i==3){
    motori[i][j].write(45);
    }
  else{
    motori[i][j].write(135);
    }
 //nella posizione base in giunto 0 può essere tipicamente 90 gradi i giunti 1 e 2
 //se appartenenti alla gambe di sinistra 0 e 3 devono essere a 45 se alle gambe di 
 //destra devono essere a 135
  }  
 for (int k = 0;k<n_leg;k++){
  motori[k][0].write(45);
  }
//angoli del motore 0 parte sinistra sono avanti per 135 gradi
//angoli del motore 0 parte destra sono avanti per 45 gradi

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
