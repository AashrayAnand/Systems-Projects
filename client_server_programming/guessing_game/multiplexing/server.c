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
#include <time.h>

// socket name
#define SOCK_NAME "/tmp/GuessSocket"
// maximum message size accepted
#define BUFFER_SIZE 128
// maximum number of clients supported at once
#define MAX_CLIENTS 32

// array of all of the file descriptors being monitored by the server
int monitored_fd_set[MAX_CLIENTS];

// used to keep track of randomly generated numbers for each client to guess
int client_answers[MAX_CLIENTS] = {0};

// used to keep track of the number of guesses for each client
int client_results[MAX_CLIENTS] = {0};

// asd
int client_status[MAX_CLIENTS] = {0};


// remove any file descriptor, if present
static void initialize_monitor_fd_set(){
  int i = 0;
  for(; i < MAX_CLIENTS; i++){
    monitored_fd_set[i] = -1;
  }
}

// add a new file descriptor to the array of file descriptors
static void add_to_monitored_fd_set(int socket_fd){
  int i = 0;
  for(; i < MAX_CLIENTS; i++){
    if(monitored_fd_set[i] == -1){
      monitored_fd_set[i] = socket_fd;
      return;
    }
  }
}

// remove an existing file descriptor (if present) from the monitored FDs
static void remove_from_monitored_fd_set(int socket_fd){
  int i = 0;
  for(; i < MAX_CLIENTS; i++){
    if(monitored_fd_set[i] == socket_fd){
      monitored_fd_set[i] = -1;
      return;
    }
  }
}

// clone all the monitored file descriptors into
// an fd_set data structure
static void refresh_fd_set(fd_set *fd_set_ptr){
  // fd_set macro which zeroes out
  // existing values in fd_set
  FD_ZERO(fd_set_ptr);
  int i = 0;
  for(; i < MAX_CLIENTS; i++){
    if(monitored_fd_set[i] != -1){
      // fd_set macros which adds each item
      // from the monitored_fd_set array to the fd_set data structure
      FD_SET(monitored_fd_set[i], fd_set_ptr);
    }
  }
}

int get_max_fd(){
  int max = -1;
  int i = 0;
  for(; i < MAX_CLIENTS; i++){
    if(monitored_fd_set[i] > max){
      max = monitored_fd_set[i];
    }
  }
  return max;
}


int main(int argc, char **argv){

  // struct which contains information
  struct sockaddr_un sock_name;

  unsigned int master_socket, data_socket, answer;
  // built in C data structure, which is used to track the
  // master socket file descriptor, and the file descriptors
  // of the clients of the program
  fd_set fds;
  int i, ready_fd;
  int ret;
  int guess;
  int correct;
  char buffer[BUFFER_SIZE];
  initialize_monitor_fd_set();

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

  // need to monitor the master socket file descriptor, this is the
  // FD which will be set when a connection request is initiated
  add_to_monitored_fd_set(master_socket);

  // permanently listen for connections, until server is terminated
  for(;;){
    // update the fd_set to match all of the actively monitored file descriptors
    refresh_fd_set(&fds);
    printf("waiting on select() sys call\n");

    // block until a file descriptor in the range 0 -> max_fds until
    // a client is ready to write to the server, or a new connection
    // initiation request is sent, checking the file descriptors only in fds
    select(get_max_fd() + 1, &fds, NULL, NULL, NULL);
    printf("survived select\n");

    // fd_set macro which checks if a specific file descriptor is
    // "set", meaning this file descriptor is ready to read or write,
    // if the master socket file descriptor is set, a connection
    // request has been initiated, otherwise, a client is ready to write
    if(FD_ISSET(master_socket, &fds)){
      printf("Master socket is set, connection request has been initiated\n");
      data_socket = accept(master_socket, NULL, NULL);

      if(data_socket == -1){
        perror("accept");
        exit(EXIT_FAILURE);
      }

      printf("Connection accepted from client\n");

      // generate a number to guess for the client
      srand ( time(NULL) );
      answer = rand() % 100;
      printf("generating random number in [1,100] for client %d, guessing game, answer is %d\n", data_socket, answer);
      client_answers[data_socket] = answer;
      // add the new connection to the monitored file descriptors, this will be added to the
      // fd_set before the next select()
      add_to_monitored_fd_set(data_socket);
    } else if(FD_ISSET(0, &fds)){
      printf("asds\n");
      // input received from console (file descriptor 0)
      memset(buffer, 0, BUFFER_SIZE);
      ret = read(0, buffer, BUFFER_SIZE);
      printf("input read from console: %s\n", buffer);
    } else {
      // data has arrived from other sockets, need to determine
      // which client has sent data, and responds
      i = 0;
      // variable to keep track of which file descriptor
      // is ready
      ready_fd = -1;
      for(; i < MAX_CLIENTS; i++){
        // we have found the file descriptor that is ready
        if(FD_ISSET(monitored_fd_set[i], &fds)){
          ready_fd = monitored_fd_set[i];

          memset(buffer, 0, BUFFER_SIZE);

          ret = read(ready_fd, &guess, sizeof(int));
          printf("received guess from client\n");
          if(ret == -1){
            perror("read");
            exit(EXIT_FAILURE);
          }

          // increment the number of guesses taken
          client_results[ready_fd]++;
          // store whether or not the guess was correct,
          // this value will be written to the data handler
          // for the given client
          printf("guess = %d\n", guess);
          if(guess == client_answers[ready_fd]){
            client_status[ready_fd] = 1;
          }
          printf("correct is %d\n", client_status[ready_fd]);
          ret = write(ready_fd, &client_status[ready_fd], sizeof(int));

          if(ret == -1){
            perror("write");
            exit(EXIT_FAILURE);
          }

          if(client_status[ready_fd]){
            memset(buffer, 0, BUFFER_SIZE);
            sprintf(buffer, "answer = %d, number of guesses = %d\n", client_answers[ready_fd], client_results[ready_fd]);
            printf("sending final result to client\n");
            // write data to data socket created for this client, and close socket
            ret = write(ready_fd, buffer, BUFFER_SIZE);
            if(ret == -1){
              perror("write");
              exit(EXIT_FAILURE);
            }
            // game is over for this client, close the data socket
            client_answers[ready_fd] = 0;
            client_results[ready_fd] = 0;
            close(ready_fd);
            remove_from_monitored_fd_set(ready_fd);
          }
        break;
        }
      }
    }
  }

  close(master_socket);
  printf("connection closed, goodbye...\n");
  unlink(SOCK_NAME);
  exit(EXIT_SUCCESS);
}
