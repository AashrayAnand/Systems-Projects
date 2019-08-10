// this file should consist of the server implementation
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

int main(int argc, char **argv){

  struct sockaddr_un sock_name;

  unsigned int master_socket, data_socket, answer, nguesses;
  int ret;
  int guess;
  int correct;
  char buffer[BUFFER_SIZE];

  // remove socket if it still exists
  unlink(SOCK_NAME);

  // create master socket of stream type, return
  // the master socket file descriptor
  master_socket = socket(AF_UNIX, SOCK_STREAM, 0);

  // for robustness, clear master socket name structure
  memset(&sock_name, 0, sizeof(struct sockaddr_un));

  // specify master socket credentials
  sock_name.sun_family = AF_UNIX;
  strncpy(sock_name.sun_path, SOCK_NAME, sizeof(sock_name.sun_path) - 1);

  // bind socket to previously defined socket name structure
  ret = bind(master_socket,
    (const struct sockaddr *) &sock_name, sizeof(struct sockaddr_un));

  if(ret == -1){
    perror("bind");
    exit(EXIT_FAILURE);
  }

  printf("bind() call succeeded\n");

  // after we have successfully bound the master file socket descriptor
  // to the socket name, we should now prepare to accept client connections,
  // with a backlog of up to 20 requests
  ret = listen(master_socket, 20);
  if(ret == -1){
    perror("listen");
    exit(EXIT_FAILURE);
  }

  // permanently listen for connections, until server is terminated
  for(;;){
    nguesses = 0;
    printf("waiting on accept() sys call\n");

    data_socket = accept(master_socket, NULL, NULL);

    if(data_socket == -1){
      perror("accept");
      exit(EXIT_FAILURE);
    }

    printf("client connection accepted\n");
    answer = rand() % 100 + 1;
    printf("generating random number in [1,100] for guessing game, answer is %d\n", answer);
    for(;;){

      // prepare  buffer to receive user guess data
      memset(buffer, 0, BUFFER_SIZE);

      printf("waiting for guess from client\n");
      ret = read(data_socket, &guess, sizeof(int));

      if(ret == -1){
        perror("read");
        exit(EXIT_FAILURE);
      }

      // increment the number of guesses taken
      nguesses++;
      // store whether or not the guess was correct,
      // this value will be written to the data handler
      // for the given client
      correct = guess == answer;
      printf("correct = %d, answer = %d, guess = %d\n", correct, answer, guess);
      ret = write(data_socket, &correct, sizeof(int));

      if(ret == -1){
        perror("write");
        exit(EXIT_FAILURE);
      }

      if(correct){
        break;
      }
    }

    memset(buffer, 0, BUFFER_SIZE);
    sprintf(buffer, "answer = %d, number of guesses = %d\n", answer, nguesses);

    printf("sending final result to client\n");

    // write data to data socket created for this client, and close socket
    ret = write(data_socket, buffer, BUFFER_SIZE);
    if(ret == -1){
      perror("write");
      exit(EXIT_FAILURE);
    }

    // game is over for this client, close the data socket
    close(data_socket);
  }

  close(master_socket);
  printf("connection closed, goodbye...\n");

  unlink(SOCK_NAME);
  exit(EXIT_SUCCESS);
}
