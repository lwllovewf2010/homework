#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define NR_SAMPLES		65535
#define ROOF					2000
#define FLOOR					350
#define SPAN					25
#define THRESHOLD			10

enum {
	HIGH = 1,
	MED,
	LOW,
};

int main(int argc, char **argv) {

	int vals[NR_SAMPLES];
	int i;
  int high = FLOOR, low = ROOF;
	int med, rangeh, rangel;
	int prev, curr;
	int thres = 0;
	int cmd;

	printf("Initializing....\n");
	for (i = 0; i < NR_SAMPLES; ++i) {
try_again:
    srandom(time(NULL));
		vals[i] = (random() % (ROOF - FLOOR) + FLOOR);
		usleep(10);

		if ((vals[i] < FLOOR) || (vals[i] > ROOF)) {
			printf("Retry[%d] = %d\n", i, vals[i]);
			goto try_again;
		}
	}

	for (i = 0; i < NR_SAMPLES; ++i) {
		if (vals[i] > high) {
			high = vals[i];
		}
		if (vals[i] < low) {
			low = vals[i];
		}
	}

	med = (high + low) / 2;
  rangeh = med + SPAN;
	rangel = med - SPAN;
	printf("high = %d, low = %d\n", high, low);
	printf("median = %d\n", med);
	printf("range from %d to %d\n", rangeh, rangel);

  printf("Re-initializing....\n");
  for (i = 0; i < NR_SAMPLES; ++i) {
try_again1:
    srandom(time(NULL));
    vals[i] = (random() % (ROOF - FLOOR) + FLOOR);
    usleep(10);

    if ((vals[i] < FLOOR) || (vals[i] > ROOF)) {
      printf("Retry[%d] = %d\n", i, vals[i]);
      goto try_again1;
    }
  }

	prev = curr = MED;
	for (i = 0; i < NR_SAMPLES; ++i) {
		prev = curr;
		if ((vals[i] >= rangel) && (vals[i] <= rangeh)) {
			curr = MED;
		} else if (vals[i] > rangeh) {
			curr = HIGH;
		} else {
			curr = LOW;
		}
		if (curr != prev) {
			thres = 0;
			cmd = 0;
		} else {
			thres++;
		}

		if (thres > THRESHOLD) {
			if (curr == LOW) {
				if (!cmd) {
					printf("(LOW)BOOST the CPU, DO GC, DO OOM KILLER(%d:%d)\n", i, vals[i]);
					cmd = 1;
				}
			} else {
				if (!cmd) {
					printf("(MED or HIGH)TRUN to ONDEMAND, RELAX the CPU(%d:%d)\n", i, vals[i]);
					cmd = 1;
				}
			}
		}
	}

	return 0;
}

