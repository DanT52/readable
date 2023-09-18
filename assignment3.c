/***********************************************************************
name:
	readable -- recursively count readable files.
description:	
	See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


int readable(char* inputPath);

/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/


int readable(char* inputPath) {

	DIR *dir =NULL;
	struct dirent *ent = NULL;
	struct stat buf;
	char *file = NULL;
	char *current;

	

	if (inputPath){

		if (lstat(inputPath, &buf)==-1){
			fprintf(stderr, "Error: lstat failed to read input path. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
			return -(errno);
		}

		if (S_ISREG(buf.st_mode)){
			printf("was a reg file. ");
			if (access(inputPath, R_OK) == 0)return 1;

			else if (access(inputPath, R_OK) == -1){
				fprintf(stderr, "Error: failed to access path. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
				return -(errno);

			}
			else return 0;

		}else if (S_ISDIR(buf.st_mode)){
			if (access(inputPath, R_OK) == -1){
				fprintf(stderr, "Error: failed to access path. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
				return -(errno);
			}

			if (access(inputPath, R_OK) != 0){
				if (ent == NULL){
					fprintf(stderr, "Error: Path Passed in '%s' is a unreadable directory: %d, Error Msg: %s \n",inputPath, errno, strerror(errno));
					return -(errno);
				}
				else return 0;
			}

		}else{
			//ignore other types of files.
			return 0;
		}
		current = getcwd(NULL, 0); //get the current working directory
		if (!current){
			fprintf(stderr, "Error: failed to get current directory. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
			return -(errno);
		}

		if (chdir(inputPath) == -1){//change dir to the input path (this means a dir is the filepath.)
			fprintf(stderr, "Error: failed to change working directory. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
			return -(errno);

		} 
	}

	
	
	dir = opendir("."); //open the current directory.
	if (!dir){
		fprintf(stderr, "Error: failed to open current directory. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
		return -(errno);
	}

	int count = 0;
	int add = 0;
	errno = 0;

	while ((ent = readdir(dir)) != NULL){
		
		file = ent->d_name;

		
		if (!strcmp(file, ".")|| !strcmp(file, ".."))continue;

		printf("CURRENT FILE: %s \n", file);

		add = readable(file);
		if (add < 0){
			return add;
		}
		count += add;

	}
	if(errno != 0){
		fprintf(stderr, "Error: failed reading directory: %d, Error Msg: %s \n", errno, strerror(errno));
		return -(errno);
	}

	if (chdir(current) == -1){
		fprintf(stderr, "Error: failed to change working directory to '%s'. Error Num: %d, Error Msg: %s \n",current, errno, strerror(errno));
		return -(errno);
	}
	
	if (closedir(dir)==-1){
		fprintf(stderr, "Error: failed to close directory. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
		return -(errno);
	}
	free(current);

    return count;
}