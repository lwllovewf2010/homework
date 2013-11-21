#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


typedef struct _usb_handle
{
    int fd;
//    adb_cond_t notify;
//    adb_mutex_t lock;
} usb_handle;


int usb_write(usb_handle *h, const void *data, int len)
{
    int n;

    printf("about to write (fd=%d, len=%d)\n", h->fd, len);
    n = write(h->fd, data, len);
    if(n != len) {
        printf("ERROR: fd = %d, n = %d, errno = %d (%s)\n",
            h->fd, n, errno, strerror(errno));
        return -1;
    }
    printf("[ done fd=%d ]\n", h->fd);
    return 0;
}


int usb_read(usb_handle *h, void *data, int len)
{
    int n;

    printf("about to read (fd=%d, len=%d)\n", h->fd, len);
    n = read(h->fd, data, len);
    if(n != len) {
        printf("ERROR: fd = %d, n = %d, errno = %d (%s)\n",
            h->fd, n, errno, strerror(errno));
        return -1;
    }
    printf("[ done fd=%d ]\n", h->fd);
    return 0;
}


int main( int argc, char **argv ) {

	usb_handle *usb;
	int fd;
	char buf[ 4096 ];

	// Open device
	fd = open( "/dev/android_adb", O_RDWR );
	if( fd < 0 ) {

		fprintf( stderr, "Cannot open android_adb device\n" );
		return -1;
	}

	// Allocate memory
	usb = calloc( 1, sizeof( usb_handle ) );
	if( !usb ) {

		fprintf( stderr, "Out of memory\n" );
		goto ErrExit;
	}

	// Fill in data
	usb->fd = fd;

	// Read USB
	usb_write( usb, buf, sizeof( buf ) );
	usb_read( usb, buf, sizeof( buf ) );

//ErrExit1:
	free( usb );

ErrExit:
	// Close device
	close( fd );

	return 0;
}


