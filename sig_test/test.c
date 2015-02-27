#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <sys/mman.h>
#include <stdint.h>

sigjmp_buf env;

void handler(int sig) {
  printf("SIG = %d\n", sig);
	siglongjmp(env, sig);
}

void code() {
  __asm__ volatile ( \
    "mov $0x0, %%eax\n" \
    :: \
    : "rax", "rbx", "rcx", "rdx" \
  );
}

void CatchSignal(int sig) {
  struct sigaction new_act, old_act;
  sigset_t new_set, old_set;

  sigemptyset(&new_set);
  sigaddset(&new_set, sig);
  memset(&new_act, 0, sizeof(struct sigaction));
  new_act.sa_handler = handler;
  new_act.sa_mask = new_set;
  sigaction(sig, &new_act, &old_act);
}

int main(int argc, char** argv) {
	int i;
	char* p = (char*)0x00001010;
	struct sigaction new_act, old_act;
	sigset_t new_set, old_set;
  void (*f)(void) = (void(*)(void))code;

	printf("SIGILL  = %d\n", SIGILL);
	printf("SIGSEGV = %d\n", SIGSEGV);

  uint64_t addr = (uint64_t)code;
  addr &= ~0xFFF;
	mprotect((void*)addr, 4096, PROT_READ | PROT_WRITE | PROT_EXEC);

	uint32_t* patch = (uint32_t*)(code + 5);
	printf("patch = %p\n", patch);

#if 0
	// SIGSEGV
	sigemptyset(&new_set);
	sigaddset(&new_set, SIGSEGV);
	memset(&new_act, 0, sizeof(struct sigaction));
	new_act.sa_handler = handler;
	new_act.sa_mask = new_set;
	sigaction(SIGSEGV, &new_act, &old_act);
#endif

	CatchSignal(SIGTRAP);
	CatchSignal(SIGILL);
	CatchSignal(SIGSEGV);
	CatchSignal(SIGFPE);
	CatchSignal(SIGBUS);
  CatchSignal(SIGABRT);
  CatchSignal(SIGALRM);
  CatchSignal(SIGCHLD);
  CatchSignal(SIGINT);
  CatchSignal(SIGQUIT);
  CatchSignal(SIGSTOP);
  CatchSignal(SIGTERM);
  CatchSignal(SIGTSTP);
  CatchSignal(SIGSYS);
  CatchSignal(SIGXCPU);
  CatchSignal(SIGXFSZ);

#if 0
	// SIGILL
	sigfillset(&new_set);
	sigaddset(&new_set, SIGSEGV);
	sigaddset(&new_set, SIGTRAP);
#if 0
	sigdelset(&new_set, SIGILL);
  sigdelset(&new_set, SIGTRAP);
  sigdelset(&new_set, SIGFPE);
  sigdelset(&new_set, SIGBUS);
  sigdelset(&new_set, SIGSEGV);
#endif

	memset(&new_act, 0, sizeof(struct sigaction));
	new_act.sa_handler = handler;
	new_act.sa_mask = new_set;

	//sigprocmask(SIG_SETMASK, &new_act.sa_mask, &old_set);
	sigaction(SIGILL, &new_act, &old_act);
#endif

	for (i = 0; i < 100; ++i) {
		*patch = 0xCCCCCCD0 + i;
		if (sigsetjmp(env, 1) == 0) {
    	printf("Testing SIGILL(0x%8.8X)...", *patch);
    	f();
    	printf("done\n");
  	} else {
			printf("fail\n");
		}
	}

#if 0
  if (sigsetjmp(env, 1) == 0) {
    printf("Testing SIGSEGV...");
    //*p = 123;
    printf("done\n");
  } else {
    printf("fail\n");
  }
#endif

	return 0;
}

