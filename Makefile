r: recieve_str.c
	gcc $< -o rs -I/usr/local/include -L/usr/local/lib -lwiringPi

s: send_str.c 
	gcc $< -o ss -I/usr/local/include -L/usr/local/lib -lwiringPi

str: recieve_str.c send_str.c
	gcc recieve_str.c -o rs -I/usr/local/include -L/usr/local/lib -lwiringPi
	gcc send_str.c -o ss -I/usr/local/include -L/usr/local/lib -lwiringPi

run: rs ss
	./rs & ./ss
