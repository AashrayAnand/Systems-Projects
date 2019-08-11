#include "../global_include.h"
#include <errno.h>

#ifndef BUFFER_SIZE
#define  BUFFER_SIZE 1024
#endif

int main(int argc, char *argv[]){
  // file descriptors for source and destination
  int sourceFd, destFd;
  ssize_t numRead, numWritten;
  char buffer[BUFFER_SIZE];

  if(argc != 3 || strcmp(argv[1], "--help") == 0){
    fprintf(stderr, "%s old-file new-file", argv[0]);
    exit(EXIT_FAILURE);
  }

  sourceFd = open(argv[1], 'r');
  if(sourceFd == -1){
    fprintf(stderr, "opening file %s", argv[1]);
    exit(EXIT_FAILURE);
  }

  destFd = open(argv[2], 'r');
  if(destFd == -1){
    fprintf(stderr, "opening file %s", argv[2]);
    exit(EXIT_FAILURE);
  }

  while((numRead = read(sourceFd, buffer, BUFFER_SIZE)) > 0){
    if((numWritten = write(destFd, buffer, numRead)) != numRead){
      fprintf(stderr, "couldn't write whole buffer, wrote %d out of %d", (int) numWritten, (int) numRead);
      exit(EXIT_FAILURE);
    }
  }

  if(numRead == -1){
    fprintf(stderr, "read");
    exit(EXIT_FAILURE);
  }

  if(close(sourceFd) == -1){
    fprintf(stderr, "close source");
    exit(EXIT_FAILURE);
  }
  if(close(destFd) == -1){
    fprintf(stderr, "close dest");
    exit(EXIT_FAILURE);
  }
}
