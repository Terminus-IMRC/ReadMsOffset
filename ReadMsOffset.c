#define _FILE_OFFSET_BITS 64
#define _LARGEFILE64_SOURCE
#include <sys/types.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define MSFILE "ms5saf.txt"
#define X 5
#define CHARSPERLINE (1*9+2*16+1*24+1)

char *progname;

void usage_and_exit()
{
	fprintf(stderr, "Usage: %s #ms\n", progname);
	exit(1);
}

int main(int argc, char *argv[])
{
	int fd;
	char str[CHARSPERLINE];
	off64_t off, off2;
	ssize_t n;

	progname=argv[0];

	if(argc!=2){
		fprintf(stderr, "%s: invalid arguments\n", progname);
		usage_and_exit();
	}

	fd=open(MSFILE, O_RDONLY|O_LARGEFILE);
	if(fd==-1){
		fprintf(stderr, "%s: open: %s: %s\n", progname, MSFILE, strerror(errno));
		usage_and_exit();
	}

	off=(atoll(argv[1])-1)*CHARSPERLINE;
	if((off2=lseek64(fd, off, SEEK_SET))==-1){
		fprintf(stderr, "%s: lseek64: %"PRId64": %s\n", progname, off, strerror(errno));
		usage_and_exit();
	}

	if(off!=off2){
		fprintf(stderr, "error: off!=off2\n");
		usage_and_exit();
	}

	if((n=read(fd, str, CHARSPERLINE))==-1){
		fprintf(stderr, "%s: read: %d: %s\n", progname, CHARSPERLINE, strerror(errno));
		usage_and_exit();
	}

	if(n!=CHARSPERLINE){
		fprintf(stderr, "%s: read: returned with unexpected value\n", progname);
		usage_and_exit();
	}

	if(write(STDOUT_FILENO, str, n)==-1){
		fprintf(stderr, "%s: write: %s\n", progname, strerror(errno));
		usage_and_exit();
	}

	return 0;
}
