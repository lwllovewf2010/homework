#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int i;
  int idx;
} test_struct_t;

void destructor(void* arg) {
  printf("%s: arg = %p\n", __func__, arg);
}

static pthread_key_t pthread_key_self_;
static int cnt = 0;

int changedata(int i) {
  test_struct_t* struct_data = pthread_getspecific(pthread_key_self_);
  struct_data->i = i;
  return 0;
}

void *child(void *arg) {
  test_struct_t* mytest = NULL;
  int i;
  int tmp;

  mytest = (test_struct_t*)pthread_getspecific(pthread_key_self_);
  if (mytest == NULL) {
	mytest = (test_struct_t*)malloc(sizeof(test_struct_t));
	printf("%s: new mytest = %p\n", __func__, mytest);
  	mytest->i = (int)random();
  	mytest->idx = cnt++;
  	pthread_setspecific(pthread_key_self_, (void*)mytest);
  } else {
  	printf("%s: old mytest = %p\n", __func__, mytest);
  }

  for (i = 0; ;++i) {
  	mytest = pthread_getspecific(pthread_key_self_);
  	printf("%s: loop pthread_getspecific[%d] = %p, %d\n", __func__, mytest->idx, mytest, mytest->i);
  	tmp = (int)random();
  	printf("[%d] Set to %d\n", mytest->idx, tmp);
  	changedata(tmp);
  	sleep(2);
  }
}

int main(int argc, char** argv) {
  int ret;
  void* p;
  pthread_t tid1;
  pthread_t tid2;

  ret = pthread_key_create(&pthread_key_self_, destructor);
  printf("%s: pthread_key_create = %d\n", __func__, ret);

  ret = pthread_key_create(&pthread_key_self_, destructor);
  printf("%s: pthread_key_create = %d\n", __func__, ret);

  p = pthread_getspecific(pthread_key_self_);
  printf("%s: pthread_getspecific = %p\n", __func__, p);

  pthread_create(&tid1, NULL, (void *)child, NULL);
  pthread_create(&tid2, NULL, (void *)child, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_key_delete(pthread_key_self_);
  return 0;
}
