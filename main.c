#include <stdio.h>

int readable(char *dir);


int main(int argc, char *argv[]){
	printf("number of readable files: %d \n", readable(NULL));
	return 0;
}