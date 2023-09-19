# readable

Recursively count readable files.

## Description

This program is designed to take as input some path and recursively count the files that are readable. If a file is not readable, it won't be counted. The program will return the negative value of any error codes it encounters. For more details, refer to the CS 360 Files and Directories lectures.

## Functions

1. `int readable(char* inputPath)`: The primary function that initiates the counting process. If no input path is specified, it defaults to the current directory.

2. `int rec_read(char* inputPath, int first)`: A recursive function that reads the given path. If the path is a directory, it processes its contents. The `first` parameter helps in distinguishing the main directory from subdirectories.

3. `int file_search_loop(DIR *dir, char *current)`: Iterates through the contents of a directory, calling `rec_read` for each item.

## Dependencies

The program uses the following headers:

- `stdio.h`
- `stdlib.h`
- `string.h`
- `unistd.h`
- `dirent.h`
- `sys/types.h`
- `sys/stat.h`
- `errno.h`

## Error Handling

Errors are printed to the standard error stream with details, including the error number and corresponding message. Error handling covers scenarios such as:

- Failure to get the current directory.
- `lstat` failing to read the input path.
- Failure to access the specified path.
- Failure to change the working directory.
- Failure to open a directory.
- Errors encountered while reading a directory.