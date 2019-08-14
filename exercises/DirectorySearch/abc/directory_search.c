#include "../../global_include.h"
#include "../../get_num.h"

#include <dirent.h>
#define TXT ".txt"

int main(int argc, char *argv[]){

  if(argc < 2){
    fprintf(stderr, "usage: ./directory_search file-name");
    exit(1);
  }

  // directory stream handler
  DIR *directory;
  // file descriptor for handling files with open
  int fd;
  // directory entry structure, holds information about
  // file in directory
  struct dirent* entry;

  directory = opendir(argv[1]);

  if(!directory){
    fprintf(stderr, "error opening directory %s", argv[1]);
    exit(1);
  }

  int err = errno;
  while((entry = readdir(directory))){
    // get length of file name
    int len_name = strlen(entry->d_name);
    // pointer to last 4 characters of the file name
    const char* extension = &entry->d_name[len_name - 4];
    if(strcmp(extension, TXT) == 0){
      fd = open(entry->d_name, 'r');
      if(fd == -1){
        fprintf(stderr, "error opening file %s", entry->d_name);
        exit(1);
      }
    }
  }

  if(err != errno){
    fprintf(stderr, "error opening file in directory");
    exit(1);
  }
}
