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
    DIR *dir = NULL;
    struct dirent *ent = NULL;
    struct stat buf;
    char *current = NULL;
    int access_val;

    // if no inputPath given: assume current working directory
    if (!inputPath) {
        current = getcwd(NULL, 0);
        if (!current) {
            fprintf(stderr, "Error: failed to get current directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            return -errno;
        }
        inputPath = current;
    }

    if (lstat(inputPath, &buf) == -1) {
		if(current)free(current);
        fprintf(stderr, "Error: lstat failed to read input path. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
        return -errno;
    }

    // check if the path is a symbolic link
    if (S_ISLNK(buf.st_mode)) {
		if(current)free(current);
        return 0;
    }

    // access rights
    access_val = access(inputPath, R_OK);
    if (access_val == -1) {
		if(current)free(current);
        fprintf(stderr, "Error: failed to access path. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
        return -errno;
    }

    //regular file
    if (S_ISREG(buf.st_mode)) {
		if(current)free(current);
        return access_val == 0 ? 1 : 0;
    }

    //directory
    if (S_ISDIR(buf.st_mode)) {

		if (inputPath != current){
			current = getcwd(NULL, 0); // save the current working directory
      	    if (!current) {
            	fprintf(stderr, "Error: failed to get current directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            	return -errno;
        	}

		}
        

        if (chdir(inputPath) == -1) {
            fprintf(stderr, "Error: failed to change working directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            free(current);
            return -errno;
        }

        dir = opendir(".");
        if (!dir) {
            fprintf(stderr, "Error: failed to open directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            free(current);
            return -errno;
        }
        
        int count = 0; // keep track of readables seen
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) { //skip . and ..
                continue;
            }

            int result = readable(ent->d_name);
            if (result < 0) {
                // handle error but continue
                fprintf(stderr, "Error encountered in subdir/file '%s'. Continuing with others...\n", ent->d_name);
            } else {
                count += result;
            }
        }

		if (chdir(current) == -1){ //go back to og dir
			fprintf(stderr, "Error: failed to change working directory to '%s'. Error Num: %d, Error Msg: %s \n",current, errno, strerror(errno));
			return -(errno);
		}
		
		if (closedir(dir)==-1){ //close dir
			fprintf(stderr, "Error: failed to close directory. Error Num: %d, Error Msg: %s \n", errno, strerror(errno));
			return -(errno);
		}
		free(current); 

        return count;
    }

    // for other file times return 0
    return 0;
}