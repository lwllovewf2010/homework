#include <stdio.h>
#include <pthread.h>
#include <linux/futex.h>
#include <syscall.h>

#define NUM 5

int futex_addr;

int futex_wait(void* addr, int val1) {
  return syscall(SYS_futex, addr, val1, NULL, NULL, 0);
}
int futex_wake(void* addr, int n) {
  return syscall(SYS_futex, addr, FUTEX_WAKE, n, NULL, NULL, 0);
}

void* thread_f(void* par) {
  int id = (int)par;
  
  /*go to sleep*/
  futex_addr = 0;
  futex_wait(&futex_addr, 0);
        
  printf("Thread %d starting to work!\n", id);
  return NULL;
}

int main() {
  pthread_t threads[NUM];
  int i;

  for (i = 0; i < NUM; i++) {
    pthread_create(&threads[i], NULL, thread_f, (void *)i);
  }

  printf("Everyone wait...\n");
  sleep(1);
  printf("Now go!\n");

  /*wake threads*/
  futex_wake(&futex_addr, 4);

  /*give the threads time to complete their tasks*/
  sleep(1);

  printf("Main is quitting...\n");
  return 0;
}

