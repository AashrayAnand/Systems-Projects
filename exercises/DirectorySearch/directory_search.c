#include "../../global_include.h"
#include "../../get_num.h"

#include <dirent.h>
#define TXT ".txt"
#define BUFFER_SIZE 1024
#define SLASH "/"

int main(int argc, char *argv[]){

  if(argc < 2){
    fprintf(stderr, "usage: ./directory_search file-name\n");
    exit(1);
  }

  // directory stream handler
  DIR *directory;
  // file descriptor for handling files with open
  // and integer to store length of directory
  int fd, len_dir;
  // directory entry structure, holds information about
  // file in directory
  struct dirent* entry;
  // buffer for reading from files and writing to stdout
  char buffer[BUFFER_SIZE];
  // buffer for concatenating directory with the names of
  // each text file, will need to do so to open the file
  char *full_path;
  // trackers for number of bytes read to and written from buffer
  // for each iteration of loop

  int numRead, numWritten;
  const char* last_char = &argv[1][strlen(argv[1]) - 1];
  char* dir;
  // if there is no slash at the end of the directory name,
  // add "/"
  if(strcmp(last_char, SLASH) != 0){
    dir = strncat(argv[1], SLASH, 1);
  } else {
    dir = argv[1];
  }

  len_dir = strlen(dir);
  directory = opendir(dir);

  if(!directory){
    fprintf(stderr, "error opening directory %s\n", dir);
    exit(1);
  }

  // allocate enough memory for length of directory, will need
  // to reallocate to size of directory plus file name
  full_path = (char *)malloc(len_dir * sizeof(char));

  int err = errno;
  while((entry = readdir(directory))){
    // get length of file name
    int len_name = strlen(entry->d_name);
    // pointer to last 4 characters of the file name
    const char* extension = &entry->d_name[len_name - 4];
    if(strcmp(extension, TXT) == 0){
      full_path = (char *)realloc(full_path, (len_dir + len_name) * sizeof(char));
      memset(full_path,0,strlen(full_path));
      strncat(full_path, dir, len_dir);
      strncat(full_path, entry->d_name, len_name);
      fd = open(full_path, 'r');
      if(fd == -1){
        fprintf(stderr, "error opening file %s\n", entry->d_name);
        exit(1);
      }
      while((numRead = read(fd, buffer, BUFFER_SIZE)) > 0){
        if((numWritten = write(STDOUT_FILENO, buffer, numRead)) != numRead){
          fprintf(stderr, "Error: only wrote %d of %d bytes of file read", numWritten, numRead);
          exit(1);
        }
      }
    }
  }

  free(full_path);

  if(err != errno){
    fprintf(stderr, "error opening file in directory\n");
    exit(1);
  }
}
