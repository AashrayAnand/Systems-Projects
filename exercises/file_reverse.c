#include "../global_include.h"
#include "../get_num.h"

#ifndef B_M
#define BYTE 1
#define MODE "rb"
#endif

int main(int argc, char *argv[]){

  int i, f_size;
  char buffer = '\0';
  FILE* fd;

  if(argc != 2){
    fprintf(stderr, "%s file-name", argv[0]);
    exit(1);
  }

  fd = fopen(argv[1], MODE);
  if(fd == NULL){
    fprintf(stderr, "error opening file %s", argv[1]);
    exit(1);
  }

  // use ftell to get the size in bytes of the file

  // use fseek to set file descriptor offset to end of file
  if(fseek(fd, 0, SEEK_END) == -1){
    fprintf(stderr, "error setting initial offset");
    exit(1);
  }

  // get the size of the file in bytes, will use this to,
  // iterate through the entirety of the file byte-by-byte
  if((f_size = ftell(fd)) == -1){
    fprintf(stderr, "error getting size of file");
    exit(1);
  }

  // iterate over the file in reverse order, by reading a single
  // byte at a time, and changing the offset to be 2 bytes back
  // after the read
  for(i = 0; i < f_size - 1; i++){
    if(fread(&buffer, BYTE, BYTE, fd) == -1){
      fprintf(stderr, "error reading");
      exit(1);

    }
    printf("%c", buffer);
    if(fseek(fd, -2, SEEK_CUR) == -1){
      fprintf(stderr, "error updating offset");
      exit(1);
    }
  }

  // fencepost: read the first byte of the file
  if(fread(&buffer, BYTE, BYTE, fd) == -1){
    fprintf(stderr, "error reading");
    exit(1);
  }
  printf("%c", buffer);

  // close the file
  if(fclose(fd) == -1){
    fprintf(stderr, "error closing file %s", argv[1]);
    exit(1);
  }

  exit(EXIT_SUCCESS);
}
