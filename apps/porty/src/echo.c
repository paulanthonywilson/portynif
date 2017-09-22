#include <stdio.h>
#include "erl_comm.h"

int main() {
  int av = input_available();
  printf("Hello matey boy %d\n", av);
}
