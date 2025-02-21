#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
//#include<sys/types.h>
//#include<sys/stat.h>
//#include<pthread.h>

#define size 4
#define MAX 12

int fd;
char write_buf[MAX]={0};

char number[10][MAX+1] = {
       //4gc.de1af23b 
	"101011111111",//0
	"101000100111",//1
	"110011110111",//2
	"111010110111",//3
	"111000101111",//4
	"111010111110",//5
	"111011111110",//6
	"101000110111",//7
	"111011111111",//8
	"111010111111"//9
};

char off[4][MAX+1]={
       //4gc.de1af23b 
	"110000100110",//-
	"111011100110",//o
	"110001111110",//f
	"110001111110" //f	
};

char err[4][MAX+1]={
       //4gc.de1af23b 
	"110000100110",//-	
	"110011111110",//E
	"110001100110",//r
	"110001100110" //r
};

void display(char array[4][MAX+1]);

int main(){

	int i, j, len;
	char option, num[5], array[4][MAX+1];

	fd = open("/dev/ext_device", O_RDWR);
	if(fd < 0){
		printf("cannot create file descriptor\n");
		return 0;
	}

	while(1){
		system("clear");
		printf("***** Enter the number to Display (4 digits max) or 'exit' or 'time' for time display or '-1' for count********\n");
		scanf("%s", num);
		len = strlen(num);
		
		//exit from code
		if(strcmp(num, "exit") == 0){
			for(i=0; i < 200; i++)
				display(off);
			memset(write_buf, 0, sizeof(write_buf));
			write(fd, write_buf, sizeof(write_buf));
			return 0;
		}
		//time
		else if(strcmp(num, "time") == 0){
			
		}
		//count 
		else if(strcmp(num, "-1") == 0){
			
		}
		//Range Error
		if(len>4){
			printf("Value out of Range !!!\n");
			for(i=0; i < 200; i++)
				display(err);
		}else{
			for(i=0, j=0; (i < (4-len) || (num[j])); i++){
				if(i < (4-len))	//if the digits less than 4 fill the segments with '0'
					strcpy(array[i], number[0]);
				else if(num[j]){//remaining segments with the respective numbers
					strcpy(array[i], number[num[j]-'0']);
					j++;
				}
			}
			for(i=0; i < 200; i++)
				display(array);
		}
		memset(write_buf, 0, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
	}
}

void display(char array[4][MAX+1]){
	for(int i=0; i<4; i++){
		strcpy(write_buf, array[i]);
		switch(i){
			case 0 : write_buf[6] = '0'; break; //write_buf[6] = D1
			case 1 : write_buf[9] = '0'; break; //write_buf[9] = D2
			case 2 : write_buf[10]= '0'; break; //write_buf[10]= D3
			case 3 : write_buf[0] = '0'; break; //write_buf[0] = D4
		}
		write(fd, write_buf, sizeof(write_buf));
		usleep(2*1000);		
	}
}

