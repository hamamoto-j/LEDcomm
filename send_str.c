#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GP2 2
#define TIME 2

int dtob(int decimal){
  int binary = 0;
  int base = 1;
  while(decimal > 0){
    binary = binary + (decimal % 2) * base;
    decimal = decimal / 2;
    base = base * 10;
  }
  //printf("binary is %d\n",binary);
  return binary;
}

void ctob(int data, int binary_data[8]){
  int i;
  char str_binary[8];
  sprintf(str_binary, "%d", dtob(data));

  //printf("ctob :");
  for (int i = 0; i <= 7; ++i){
    int value = (data & (1 << (7-i)) ) ? 1 : 0;
    binary_data[i] = value;
    //printf("%d",value);
  }
  //printf("\n");
}

int main(void){
  int i,j;
  int data[] = {'D', 'O', 'G', 'C', 'A', 'T'};
  int binary_data[8]={};
  int data_len = sizeof data / sizeof data[0];
  
  digitalWrite(GP2, 0);
  
  if(wiringPiSetupGpio() == -1)return 1;
  pinMode(GP2, OUTPUT);
  
  for(i=0; i<data_len; i++){
    printf("send : %d [%c]\n", i, data[i]);
    
    digitalWrite(GP2, 1);
    delay(TIME);
    
    ctob(data[i], binary_data);
    
    for(j=0; j<8; j++){
      //printf("%d", binary_data[j]);
      digitalWrite(GP2,binary_data[j]);
      delay(TIME * 3);
    } 
    //printf("\n");
    
    digitalWrite(GP2, 0);
    delay(TIME*3);    
  }
  
  digitalWrite(GP2, 1);
  delay(TIME*50);
  digitalWrite(GP2, 0);
  
  return 0;
}
