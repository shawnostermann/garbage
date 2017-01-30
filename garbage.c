#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

/* make big file containing garbage (for testing) */


/* local routines */
static void usage(char *prog);
static int getnum(char *str);

int
main(argc,argv)
     int argc;
     char *argv[];
{
    int debug = 0;
    int i;
    int size = -1;
    int binary = 0;
    int numbered = 0;
    char *file = NULL;
    int base;
    int range;
    int ch;
    FILE *f;

    /* parse the args */
    for (i=1; i < argc; ++i) {
	if (strcmp(argv[i],"-b") == 0) {
	    ++binary;
	} else if (strcmp(argv[i],"-n") == 0) {
	    binary = 0; ++numbered;
	} else if (strcmp(argv[i],"-d") == 0) {
	    ++debug;
	} else if (size == -1) {
	    static char fbuf[100];

	    size = getnum(argv[i]);
	    if (size <= 0)
		usage(argv[0]);

	    sprintf(fbuf,"gf%s",argv[i]);
	    file = fbuf;
	} else if (!file) {
	    file = argv[i];
	} else {
	    usage(argv[0]);
	}
    }

    if (!file)
	usage(argv[0]);
    if (size <= 0)
	usage(argv[0]);

    if (debug) {
	fprintf(stderr,"size: %d\n", size);
	fprintf(stderr,"file: %s\n", file);
	fprintf(stderr,"binary: %s\n", binary?"TRUE":"FALSE");
	fprintf(stderr,"numbered: %s\n", numbered?"TRUE":"FALSE");
	fprintf(stderr,"debug flag: %d\n", debug);
    }


    if ((f = fopen(file,"w+")) == NULL) {
	perror(file);
	exit(-1);
    }

    srand(getpid());

    if (binary) {
	/* binary data */
	base = 0;
	range = 256;
    } else {
	/* only printable */
	base = '(';
	range = 80;
    }

    if (numbered) {
	int line = 1;
	char buf[15];
	for (i=size; i > 0; i -= 10) {
	    sprintf(buf,"%09d\n",line++);
	    if (i < 10) {
		buf[i-1] = '\n';
		buf[i] = '\00';
	    }
	    if (fputs(buf,f) < 0) {
		perror("fputs");
		exit(-3);
	    }
	}
    } else {
	for (i=1; i <= size; ++i) {
	    if (!binary && ((i % 70) == 0))
		ch = '\n';
	    else
		ch = (rand()%range) + base;
	    if (fputc(ch,f) != ch) {
		perror("fputc");
		exit(-3);
	    }
	}
    }
    
    printf("Created file '%s' with size %d bytes%s\n", file,size,
	   binary?"(binary data)":"");

    exit(0);
}


static int
getnum(
    char *str)
{
	double strtod();
	double n;
	char *str_end;

	n = strtod(str, &str_end);
	if (*str_end != '\00') {
		/* there was more after the arg */
		if (tolower(*str_end) == 'k')
		    n *= 1024;
		else if (tolower(*str_end) == 'm')
		    n *= 1024 * 1024;
		else if (tolower(*str_end) == 'g')
		    n *= 1024 * 1024 * 1024;
		else {
			fprintf(stderr,"illegal number: '%s'\n", str);
			exit(-1);
		}
	}

	return((int)(n));
}


static void
usage(char *prog)
{
    fprintf(stderr,"usage: %s [-n] [-d] [-b] [-i size] size [fname]\n", prog);
    fprintf(stderr,"\
\t-d\tWhistle while you work\n\
\t-b\tCreate 8-bit data.  Default is only printable characters\n\
\t-n\tCreaate ASCII, numbered lines\n\
\tsize\tnumber with optional (k,m,g) for file size\n\
");
    exit(-1);
}
