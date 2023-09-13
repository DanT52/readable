#include <stdio.h>

int readable(char *dir);


int main(int argc, char *argv[]){
	printf("number of readable files: %d \n", readable("/workspaces/readable/test_folders"));
	return 0;
}