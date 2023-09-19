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
int rec_read(char* inputPath, int first);
int file_search_loop(DIR *dir, char *current);
/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/


int readable(char* inputPath){
    
    char *current=NULL;
    int result = 0;
    
    if (!inputPath){ //if an input path was not specified use current
        current = getcwd(NULL, 0);
        if (!current) {
            fprintf(stderr, "Error: failed to get current directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            return -errno;
        }
        inputPath = current;
    }

    result = rec_read(inputPath, 1); //run recread on the path
    if (current) free(current);
    return result;
}


int rec_read(char* inputPath, int first){

    DIR *dir;
    struct stat buf;
    char *current;
    int access_val;

    if (lstat(inputPath, &buf) == -1) { // read in lstat into buf
        fprintf(stderr, "Error: lstat failed to read input path. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
        return -errno;
    }

    if (S_ISLNK(buf.st_mode))return 0; // check if the path is a symbolic link
    
    access_val = access(inputPath, R_OK);   // read in access rights into access_val
    if (access_val == -1) {
        fprintf(stderr, "Error: failed to access path. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
        return -errno;
    }

    if (S_ISREG(buf.st_mode))return access_val == 0 ? 1 : 0; //handle regular file, return 1 if its accesable
    
    if (S_ISDIR(buf.st_mode)) { //handle directory
        if(access_val == 1 && !first)return 0; //if first a subdir and not readable return 0
        
        current = getcwd(NULL, 0); // save the current working directory
        if (!current) {
            fprintf(stderr, "Error: failed to get current directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            return -errno;
        }
        
        if (chdir(inputPath) == -1) { //change working dir to input path
            fprintf(stderr, "Error: failed to change working directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            free(current);
            return -errno;
        }

        dir = opendir("."); //open current dir

        if (!dir) {
            fprintf(stderr, "Error: failed to open directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
            free(current);
            return -errno;
        }
        
        return file_search_loop(dir, current);
    }
    return 0; // for other file types return 0
}


int file_search_loop(DIR *dir, char *current){

    struct dirent *ent;
    int count = 0;
    errno = 0;
    while ((ent = readdir(dir)) != NULL) {

        if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) { //skip . and ..
            continue;
        }

        int result = rec_read(ent->d_name, 0);
        
        if (result < 0) {// there was an error
            free(current);
            return result;
        } else {
            count += result;
        }
    }

    if (errno) { // Handle errors with readdir
        fprintf(stderr, "Error reading directory. Error Num: %d, Error Msg: %s\n", errno, strerror(errno));
        return(errno);
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