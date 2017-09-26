#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <stdio.h>

void write_back(char* msg);
int poll_input();
void read_in(char *buffer, int len);
int to_read_length();
void write_fixed(char* msg, int len, char * reason);

#define MAX_READ 1023


int main(int argc, char *argv[]) {
  char buffer[MAX_READ + 1];
  for(;;) {
    int res = poll_input();
    if(res > 0) {
      int len = to_read_length();
      if (len > MAX_READ) {err(EXIT_FAILURE, "Too large message to read.");}

      // len being less than zero indicates STDIN has been closed - exit
      if (len < 0) {return 1;}

      read_in(buffer, len);
      write_back(buffer);
    }
  }
}

/**
 * Write a len characters, pointed to by msg, to STDIN. The reason is used
 * as debug information should the write fail.
 */
void write_fixed(char *msg, int len, char *reason) {
  int written = 0;
  while(written < len) {
    int this_write = write(STDOUT_FILENO,  msg + written, len - written);
    if (this_write <= 0 && errno != EINTR) {
      err(EXIT_FAILURE, "%s: %d", reason, this_write);
    }
    written += this_write;
  }
}

/**
 * Send the zero-terminated msg back the BEAM by writing to stdout.
 */
void write_back(char* msg) {
  unsigned long len = strlen(msg);
  char size_header[2] = {(len >> 8 & 0xff), (len & 0xff)};
  write_fixed(size_header, 2, "header write");
  write_fixed(msg, len, "data write");
}


/**
 * Reads len chars from STDIN to buffer. Returns len if successful, or -1 if STDIN has been closed
 *
 */
int read_fixed(char* buffer, int len) {
  int read_count = 0;
  while(read_count < len) {
    int this_read = read(STDIN_FILENO, buffer + read_count, len - read_count);

    // 0 is returned from read if EOF is STDIN has been closed.
    if (this_read == 0) {
      return -1;
    }

    // errno is set to EINTR if interrrupted by a signal before any data is sent.
    if(this_read < 0 && errno != EINTR) {
      err(EXIT_FAILURE, "read failed");
    }
    read_count += this_read;
  }
  return len;
}

/**
 * Read len number of bytes from the stream and populate the buffer. Zero-terminates the buffer.
 **/
void read_in(char *buffer, int len) {
  int read_count = 0;
  do {
    int this_read = read(STDIN_FILENO, buffer + read_count, len - read_count);
    if(this_read < 0 && errno != EINTR) {
      err(EXIT_FAILURE, "read failed");
    }
    read_count += this_read;
  } while(read_count < len);
  buffer[len] = '\0';
}

/**
 * The first two bytes indicates the length of the message, with the first byte being most significant.
 * Read this and return as an int. Returns -1 if STDIN is closed
 **/
int to_read_length() {
  unsigned char size_header[2];
  int r = read_fixed((char *) size_header, 2);
  if(r < 0) {
    return -1;
  }
  return (size_header[0] << 8) | size_header[1];
}

/**
 * See http://man7.org/linux/man-pages/man2/poll.2.html
 * Also based on https://stackoverflow.com/questions/4656388/poll-stdin-and-other-fds
 *
 * Waits for input to be available on stdin. If it is return value is greater than zero.
 * Times out every 5 seconds.
**/
int poll_input() {
  int timeout = 5000;
  struct pollfd fd;
  fd.fd = STDIN_FILENO;
  fd.events = POLLIN;
  fd.revents = 0;
  return poll(&fd, 1, timeout);
}
