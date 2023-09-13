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


/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/


int readable(char* inputPath) {

	DIR *dir;
	struct dirent *ent;
	struct stat buf;
	char *file;

	char *current = getcwd(NULL, 0);

	if (inputPath){
		chdir(inputPath);
	}
	
	dir = opendir(".");
	int count = 0;
	

	while ((ent = readdir(dir)) != NULL){
		file = ent->d_name;
		if (!strcmp(file, ".")|| !strcmp(file, ".."))continue;
		printf("current: %s : ", file);

		lstat(file, &buf);

		if (S_ISREG(buf.st_mode) && !access(file, R_OK)){
			count++;
			printf("reg file\n");

		}
		

		else if (S_ISDIR(buf.st_mode)){
			printf("\n \n In Folder: %s \n", file);
			count += readable(file);
			printf("\n");
		}
		

		
	}

	chdir(current);
	free(current);
	closedir(dir);
    return count;
}

