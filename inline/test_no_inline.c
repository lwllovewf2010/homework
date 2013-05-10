#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>


// Workaround
typedef int bool;
typedef long long ktime_t;
struct cpuidle_device {

	void *p;
};
ktime_t ktime_get( void ) {

	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000000) + tv.tv_usec;
}


// Copy from source code
#define CPUIDLE_STATE_MAX   8
#define CPUIDLE_NAME_LEN    16
#define CPUIDLE_DESC_LEN    32


struct cpuidle_state {
    char        name[CPUIDLE_NAME_LEN];
    char        desc[CPUIDLE_DESC_LEN];

    unsigned int    flags;
    unsigned int    exit_latency; /* in US */
    int     power_usage; /* in mW */
    unsigned int    target_residency; /* in US */
    bool        disabled; /* disabled on all CPUs */

    int (*enter)    (struct cpuidle_device *dev,
            struct cpuidle_driver *drv,
            int index);

    int (*enter_dead) (struct cpuidle_device *dev, int index);
};


struct cpuidle_driver {
    const char      *name;
    struct module       *owner;
    int                     refcnt;

    /* set to 1 to use the core cpuidle time keeping (for all states). */
    unsigned int        en_core_tk_irqen:1;
    /* states array must be ordered in decreasing power consumption */
    struct cpuidle_state    states[CPUIDLE_STATE_MAX];
    int         state_count;
    int         safe_state_index;
};


int cpuidle_wrap_enter(struct cpuidle_device *dev,
                struct cpuidle_driver *drv, int index,
                int (*enter)(struct cpuidle_device *dev,
                    struct cpuidle_driver *drv, int index))
{
	ktime_t time_start, time_end;

	time_start = ktime_get();

    index = enter(dev, drv, index);

	time_end = ktime_get();

	return index;
}


static /*inline*/ int cpuidle_enter(struct cpuidle_device *dev,
                struct cpuidle_driver *drv, int index)
{
    struct cpuidle_state *target_state = &drv->states[index];
    return target_state->enter(dev, drv, index);
}


static /*inline*/ int cpuidle_enter_tk(struct cpuidle_device *dev,
                   struct cpuidle_driver *drv, int index)
{
    return cpuidle_wrap_enter(dev, drv, index, cpuidle_enter);
}


typedef int (*cpuidle_enter_t)(struct cpuidle_device *dev,
                   struct cpuidle_driver *drv, int index);


static cpuidle_enter_t cpuidle_enter_ops;



// Test Code
int main( int argc, char **argv ) {

	int next_step = 0x03;
	int en_core_tk_irqen;

	if( argc != 2 ) {

		fprintf( stderr, "Invalid input parameter\n" );
		return 0;
	}

	en_core_tk_irqen = atoi( argv[ 1 ] );
	if( en_core_tk_irqen != 0 )
		en_core_tk_irqen = 1;

	printf( "en_core_tk_irqen = %d\n", en_core_tk_irqen );

	// Select
	cpuidle_enter_ops = en_core_tk_irqen ?
		cpuidle_enter_tk : cpuidle_enter;

	printf( "cpuidle_enter_tk   = %p\n", cpuidle_enter_tk );
	printf( "cpuidle_wrap_enter = %p\n", cpuidle_wrap_enter );
	printf( "cpuidle_enter      = %p\n", cpuidle_enter );
	printf( "cpuidle_enter_ops  = %p\n", cpuidle_enter_ops );

	cpuidle_enter_ops( (struct cpuidle_device *)0x12345678, (struct cpuidle_driver *)0xABCD1234, next_step );

	return 0;
}


