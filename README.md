# LEDcomm
Raspberrypi上にてLED可視光通信を行うためのプログラム
wiringPiを必要とする

## send_str.c
文字列を光として送信するプログラム
LEDの+端子をGPIO2に接続して使用する

## recieve_str.c
光に変換された文字列を受信するプログラム
A/DコンバータをRaspberrypiに接続して使用する
