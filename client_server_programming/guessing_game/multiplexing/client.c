// this file should consist of the client implementation
// of a client server programming model, used to play
// a guessing game, a user can connect to the server, which
// will generate a random number for the user to guess
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// socket name
#define SOCK_NAME "/tmp/GuessSocket"
#define BUFFER_SIZE 128

int main(int argc, char * *argv){

  // socket address struct, will store socket family (AF_UNIX),
  // and socket name (SOCK_NAME)
  struct sockaddr_un address;
  // variable to store socket file descriptor
  unsigned int sock, ret;
  // variable to hold user guesses, to send to server
  int guess;
  int resp;
  char buffer[BUFFER_SIZE];

  // create data socket of stream type, return file descriptor
  sock = socket(AF_UNIX, SOCK_STREAM, 0);
  // for robustness, clear the address structure
  memset(&address, 0, sizeof(struct sockaddr_un));

  // connect data socket to socket address
  address.sun_family = AF_UNIX;
  strncpy(address.sun_path, SOCK_NAME, sizeof(address.sun_path) - 1);
  ret = connect(sock,
    (const struct sockaddr *) &address, sizeof(struct sockaddr_un));

  if(ret == -1){
    fprintf(stderr, "The server is down\n");
    exit(EXIT_FAILURE);
  }

  //clear the buffer
  memset(buffer, 0, BUFFER_SIZE);

  do{
    memset(buffer, 0, BUFFER_SIZE);
    printf("Enter a number between 1 and 1000: ");
    int res;
    scanf("%d", &guess);
    // send data to the server
    ret = write(sock, &guess, sizeof(int));
    if(ret == -1){
      perror("write");
      exit(EXIT_FAILURE);
    }
    printf("Guess sent to the server was: %d\n", guess);
    ret = read(sock, &resp, sizeof(int));

    if(ret == -1){
      perror("read");
      exit(EXIT_FAILURE);
    }

    // if the value written to resp by the server is 1,
    // exit the loop, as the correct  answer was received
    if(resp){
      break;
    }
  }while(1);

  // receive results from server about guessing game
  ret = read(sock, buffer, BUFFER_SIZE);
  if(ret == -1){
    perror("read");
    exit(EXIT_FAILURE);
  }

  // output the answer and the number of guesses
  printf("%s", buffer);

  return EXIT_SUCCESS;
}
