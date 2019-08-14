#include "../../global_include.h"
#include "../../get_num.h"

#ifndef B_M
#define BYTE 1
#define OVERWRITE 'w'
#define APPEND 'a'
#define BUFFER_SIZE 4096
#define METHOD_LEN 8
#endif


static int append = 0;

int parse_arguments(int argc, char *argv[]){

  char opt;
  while((opt = getopt(argc, argv, "a")) != -1){
      switch(opt)
      {
          case 'a':
              append = 1;
              printf("a found\n");
              break;
          case '?':
              if(isprint(optopt)){
                fprintf(stderr, "unknown arg -%c\n", optopt);
              } else {
                fprintf(stderr, "unknown arg character\n");
              }
              return 1;
              break;
          default:
              abort();
      }
  }

  return 0;
}

// tee reads standard input until an end-of-file, then
// writes the standard input to standard output, and
// to a file
int main(int argc, char *argv[]){
  if(argc < 2){
    fprintf(stderr, "%s output-file", argv[0]);
    exit(1);
  }

  int fd;
  int numWritten, numRead;
  char buffer[BUFFER_SIZE];
  char *mode;

  if(parse_arguments(argc, argv)){
    exit(1);
  }

  mode = (append ? "a" : "w");

  if(append){
    fd = open(argv[2], O_WRONLY);
  } else {
    fd = open(argv[1], O_WRONLY | O_TRUNC);
  }

  if(lseek(fd, 0, SEEK_END) == -1){
    fprintf(stderr, "error setting initial offset");
    exit(1);
  }

  if(fd == -1){
    fprintf(stderr, "error on file open %s\n", strerror( errno ));
    exit(1);
  }

  printf("entered text will be written to %s until you exit (^C)", argv[1]);
  fflush(stdout);
  while((numRead = read(STDIN_FILENO, buffer, BUFFER_SIZE)) > 0){
    printf("numRead is %d", numRead);
    fflush(stdout);
    if((numWritten = write(fd, &buffer, numRead)) != numRead){
      fprintf(stderr, "couldn't write whole buffer, wrote %d of %d", numWritten, numRead);
      exit(1);
    }
    if(numRead == -1){
      fprintf(stderr, "error on reading from stdin");
      exit(1);
    }
  }

  if(close(fd) == -1){
    fprintf(stderr, "error on closing file descriptor");
    exit(1);
  }

  exit(0);
}
