#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>

void write_back(char* msg);
int poll_input();
void read_in(char *buffer, int len);
int to_read_length();
void write_out(char* msg, int len, char * reason);

#define MAX_READ 1023


int main(int argc, char *argv[]) {
  char buffer[MAX_READ + 1];
  for(;;) {
    int res = poll_input();
    if(res > 0) {
      int len = to_read_length();
      if (len > MAX_READ) {err(EXIT_FAILURE, "Too large message to read.");}

      // A zero length message is sent when the port closes.
      if (len == 0) return 1;

      read_in(buffer, len);
      write_back(buffer);
    }
  }
}

void write_out(char *msg, int len, char *reason) {
  int written = 0;
  do {
    int this_write = write(STDOUT_FILENO,  msg + written, len - written);
    if (this_write < 0 && errno != EINTR) {
      err(EXIT_FAILURE, "%s: %d", reason, this_write);
    }
    written += this_write;
  } while (written < len);
}

/**
 * Send the zero-terminated msg back the BEAM by writing to stdout.
 */
void write_back(char* msg) {
  unsigned long len = strlen(msg);
  char size_header[2] = {(len >> 8 & 0xff), (len & 0xff)};
  write_out(size_header, 2, "header write");
  write_out(msg, len, "data write");
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
 * Read this and return as an int.
 **/
int to_read_length() {
  unsigned char size_header[2] = {0, 0};
  read(STDIN_FILENO, size_header, 2);
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
