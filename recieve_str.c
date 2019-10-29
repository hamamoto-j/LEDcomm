#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <stdio.h>

#define SS_PORT 8
#define SPI_CHANNEL 0                   //SPIチャンネル
#define MILLI_SEC 1000000               //ms
#define TIME 2
#define THRESHOLD 320                   //LED brightness threshold
//#define STR_LEN 5

struct str{
  unsigned int a0 :1;
  unsigned int a1 :1;
  unsigned int a2 :1;
  unsigned int a3 :1;
  unsigned int a4 :1;
  unsigned int a5 :1;
  unsigned int a6 :1;
  unsigned int a7 :1;
};

union chr{
  char c;
  struct str a;
};

void btoc(union chr c, int data[8]){
  c.a.a7 = data[7];
  c.a.a6 = data[6];
  c.a.a5 = data[5];
  c.a.a4 = data[4];
  c.a.a3 = data[3];
  c.a.a2 = data[2];
  c.a.a1 = data[1];
  c.a.a0 = data[0];
}

int check_v(unsigned char v[2]){
  if( (v[0]*256 + v[1]) >= THRESHOLD ){
    return 1;
  }
  else return 0;
}

int main(void)
{
  int i,j,k,n = 0;
  int speed = 10000000;                         //通信速度(100Hz)
  unsigned char spi_buff[2];                   //送受信用バッファ
  int data[8];
  int num;
  char str[1024];
  union chr c;
  char chara;
  int str_len =0;
  int flag = 0;
  int try_num = 1000;
  //バッファ初期化
  
  //SPIチャンネル初期化
  if((wiringPiSPISetup (SPI_CHANNEL, speed)) < 0){
    printf("wiringPiSPISetup error \n");
    return -1 ;
  }
  
  printf("Setup SPI...\n");
  
  //GPIO初期化
  if(wiringPiSetupGpio() == -1){
    printf("wiringPiSetupGpio error\n");
    return -1;
  }
  pinMode(SS_PORT, OUTPUT);                       //22pinを出力に設定
  digitalWrite(SS_PORT, 1);                       //SS信号初期化
  
  printf("Start SPI...\n");
  while(n < try_num && flag == 0){
    
    //SPI通信実行
    digitalWrite(SS_PORT, 0);                   //SS信号をLOW出力にして通信開始
    spi_buff[0] = 0b01100000;
    wiringPiSPIDataRW(SPI_CHANNEL, spi_buff, 2);             //データ送受信
    digitalWrite(SS_PORT, 1);                   //SS信号をHIGH出力にして通信終了
    
    //インターバル
    printf("%d\n", spi_buff[0]*256+spi_buff[1]);                 //受信データを出力
    
    if(check_v(spi_buff)){
      printf("Data input.\n");
      delay(TIME);
      flag = 1;
      for(i=0; i<8; i++){
	num=0;
	for(j=0; j<3; j++){
	  digitalWrite(SS_PORT,0);
	  spi_buff[0] = 0b01100000;
	  wiringPiSPIDataRW(SPI_CHANNEL, spi_buff,2);
	  digitalWrite(SS_PORT,1);
	  spi_buff[0] = spi_buff[0] << 6;
	  spi_buff[0] = spi_buff[0] >> 6;
	  spi_buff[1] &= 0b11111111;
	  //printf("\t%d\n", spi_buff[0]*256+spi_buff[1]);     //受信データを出力
	  if(check_v(spi_buff))num += 1;
	  delay(TIME);
	}
	if (num >= 2) num= 1;
	else {
	  num = 0;
	  flag = 0;
	}
	data[i] = num;
	//printf("%d. num = %d\n",i,num);
      }
      c.a.a7 = data[0];
      c.a.a6 = data[1];
      c.a.a5 = data[2];
      c.a.a4 = data[3];
      c.a.a3 = data[4];
      c.a.a2 = data[5];
      c.a.a1 = data[6];
      c.a.a0 = data[7];
      chara = c.c;
      printf("char is [%c]\n", chara);
      str[k] = chara;
      k++;
      delay(TIME*2);
    }
    n++;
  }
  str[k-1] = '\0';
  printf("str is [%s]\n", str);
  return 0;
}
