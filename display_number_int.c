#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>

#define size 4
#define MAX 12

int fd, write_buf[MAX]={0};

int number[10][MAX] = {
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

int off[4][MAX]={
       //4 g c . d e 1 a f 2 3 b 
	{1,1,0,0,0,0,1,0,0,1,1,0},//-
	{1,1,1,0,1,1,1,0,0,1,1,0},//o
	{1,1,0,0,0,1,1,1,1,1,1,0},//f
	{1,1,0,0,0,1,1,1,1,1,1,0} //f	
};
int err[4][MAX]={
       //4 g c . d e 1 a f 2 3 b 
	{1,1,0,0,0,0,1,0,0,1,1,0},//-	
	{1,1,0,0,1,1,1,1,1,1,1,0},//E
	{1,1,0,0,0,1,1,0,0,1,1,0},//r
	{1,1,0,0,0,1,1,0,0,1,1,0} //r
};

void display_num(char *num);
void display_off(int array[4][MAX]);
int main(){
	
	int i, j, len;
	int array[4][MAX];
	char option, num[5];

	fd = open("/dev/ext_device", O_RDWR);
	if(fd < 0){
		printf("cannot create file descriptor\n");
		return 0;
	}

	while(1){
		system("clear");
		printf("***** Enter the number to Display (4 digits max) or 'exit' ********\n");
		scanf("%s", num);
		
		//exit from code
		if(strcmp(num, "exit")==0){	
			for(i=0; i < 300; i++)
				display_off(off);	
			bzero(write_buf, sizeof(write_buf));
			write(fd, write_buf, sizeof(write_buf));
			return 0;
		}
		
		//Display Range Error
		len = strlen(num);
		if(len>4){
			printf("Value out of Range !!!\n");
			for(i=0; i < 200; i++)
				display_off(err);		
		//Display Number 
		}else{
			len = strlen(num);
			for(i=0, j=0; (i<(4-len) || (num[j])); i++){
				if(i < (4-len))	//if the digits less than 4 fill the segments with '0'
					memcpy(array[i], number[0], sizeof(write_buf));
				else if(num[j]){//remaining segments with the respective numbers
					memcpy(array[i], number[num[j]-'0'], sizeof(write_buf));
					j++;
				}
			}
				
			for(int i=0; i < 200; i++)
				display_off(array);
		}
		bzero(write_buf, sizeof(write_buf));
		write(fd, write_buf, sizeof(write_buf));
	}
}

void display_off(int array[4][MAX]){
	for(int i=0; i<4; i++){
		memcpy(write_buf, array[i], sizeof(write_buf));
		switch(i){
			case 0 : write_buf[6] = 0; break; //write_buf[6] = D1
			case 1 : write_buf[9] = 0; break; //write_buf[9] = D2
			case 2 : write_buf[10]= 0; break; //write_buf[10]= D3
			case 3 : write_buf[0] = 0; break; //write_buf[0] = D4
		}
		write(fd, write_buf, sizeof(write_buf));
		usleep(2*1000);		
	}
}
