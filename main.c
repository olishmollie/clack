#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFSIZE 150

int main(void)
{
    while (1) {
	char *buf = malloc(sizeof(BUFSIZE));
	size_t size = 0;
	printf(">> ");
	getline(&buf, &size, stdin);
	free(buf);
	if (strcmp(".exit\n", buf) == 0) {
	    exit(0);
	} else {
	    printf("%s", buf);
	}
    }

    return 0;
}
