#include <stdio.h>
#include <sys/time.h>

int main(int argc, char** argv) {
  struct timeval tv;
  double time_in_mill;

  gettimeofday(&tv, NULL);
  time_in_mill = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;

  printf("Milliseconds = %0.0f\n", time_in_mill);
  return 0;
}

