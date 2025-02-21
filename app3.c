#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
//#include<pthread.h>

#define MAX 12

int fd;
int8_t write_buf[MAX]={0};
int size = sizeof(write_buf);

int8_t number[10][MAX] = {
       //4 g c . d e 1 a f 2 3 b 
	{1,0,1,0,1,1,1,1,1,1,1,1},//0
	{1,0,1,0,0,0,1,0,0,1,1,1},//1
	{1,1,0,0,1,1,1,1,0,1,1,1},//2
	{1,1,1,0,1,0,1,1,0,1,1,1},//3
	{1,1,1,0,0,0,1,0,1,1,1,1},//4
	{1,1,1,0,1,0,1,1,1,1,1,0},//5
	{1,1,1,0,1,1,1,1,1,1,1,0},//6
	{1,0,1,0,0,0,1,1,0,1,1,1},//7
	{1,1,1,0,1,1,1,1,1,1,1,1},//8
	{1,1,1,0,1,0,1,1,1,1,1,1},//9
};

int8_t off[4][MAX]={
       //4 g c . d e 1 a f 2 3 b 
	{1,1,0,0,0,0,1,0,0,1,1,0},//-
	{1,1,1,0,1,1,1,0,0,1,1,0},//o
	{1,1,0,0,0,1,1,1,1,1,1,0},//f
	{1,1,0,0,0,1,1,1,1,1,1,0} //f	
};
int8_t err[4][MAX]={
       //4 g c . d e 1 a f 2 3 b 
	{1,1,0,0,0,0,1,0,0,1,1,0},//-	
	{1,1,0,0,1,1,1,1,1,1,1,0},//E
	{1,1,0,0,0,1,1,0,0,1,1,0},//r
	{1,1,0,0,0,1,1,0,0,1,1,0} //r
};

void display(int8_t array[4][MAX]);
int main(){
	
	char i, j, len;
	int count;
	int8_t array[4][MAX];
	char option, num[5];

	fd = open("/dev/ext_device", O_RDWR);
	if(fd < 0){
		printf("cannot create file descriptor\n");
		return 0;
	}

	while(1){
		system("clear");
		printf("***** Enter the number to Display (4 digits max) or '-1' for counting or 'time'for HR:MIN format or 'exit' ********\n");
		scanf("%s", num);
		len = strlen(num);
		
		//exit from app
		if(strcmp(num, "exit")==0){	
			for(i=0; i < 200; i++)
				display(off);	
			memset(write_buf, 0, size);
			write(fd, write_buf, size);
			return 0;
		}
		//number count and time count HR:MIN
		else if((strcmp(num, "-1")==0)||((strcmp(num, "time")) == 0)){
			count = 0;
			while(1){
				memcpy(array[0], number[count /1000], size);
				memcpy(array[1], number[(count/100)%10], size);
				memcpy(array[2], number[(count/10 )%10], size);
				memcpy(array[3], number[count %10], size);
				
				count++;
				if(strcmp(num, "time") == 0){//time in HR:MIN format
					array[1][3] = 1;//for .(dot)
					if(count %100 == 60)
						count += 40;
					if(count == 6000)
						break;
				}
				else if(count>100)//count 1 - 100
					break;
				for(i=0; i<125; i++)//time delay calculated using actual time(1 sec) by demo
					display(array);
			}	
		}
		//Range Error
		else if(len>4){
			err:
			printf("Value out of Range !!!\n");
			for(i=0; i < 200; i++)
				display(err);		
		}else{
			for(i=0, j=0; (i<(4-len) || (num[j])); i++){
				if(i < (4-len))	//if the digits less than 4 fill the segments with '0'
					memcpy(array[i], number[0], size);
				else if(num[j]){//remaining segments with the respective numbers
					memcpy(array[i], number[num[j]-'0'], size);
					j++;
				}
			}
			for(i=0; num[i]; i++)//if the characters other than '-1', 'exit', 'time', valid numbers appears, the segment will show error
				if((num[i]<'0')||(num[i]>'9'))
					goto err;
			for(i=0; i<125; i++)
				display(array);
			
		}
		memset(write_buf, 0, size);
		write_buf[1] = 1;//to indicate the segment is ON like '----'
		write(fd, write_buf, size);
	}
}

void display(int8_t array[4][MAX]){
	for(int8_t i=0; i<4; i++){
		memcpy(write_buf, array[i], size);
		switch(i){
			case 0 : write_buf[6] = 0; break; //write_buf[6] = D1
			case 1 : write_buf[9] = 0; break; //write_buf[9] = D2
			case 2 : write_buf[10]= 0; break; //write_buf[10]= D3
			case 3 : write_buf[0] = 0; break; //write_buf[0] = D4
		}
		write(fd, write_buf, size);
		usleep(2*1000);		
	}
}

