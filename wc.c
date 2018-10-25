#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <termios.h>

static int stop_thread = 0;
static char ReadBuffer[4086];
static 	int typeindex = 0;
static struct termios oldt;

void *typer(void *argp) {

	int check;
	while(!stop_thread) {
		check = getchar();
		ReadBuffer[typeindex] = check;
		typeindex++;
		//fgets(ReadBuffer, 1, stdin);
	}
	return NULL;
}

int main(int argc, char **argv) {
	int seconds_start = 0;
	clock_t start_t, end_t, total_t;
	int word_count = 0;
	FILE *fp = fopen("fox.txt", "rw");
	if(fp == NULL) {
		printf("Error opening file\n");
		return 1;
	}
	
	// Read contents of file to output
	char c;
	c = fgetc(fp);
	if ( c == '\n' || c == '\t' || c == ' ') {
		++word_count;
	}
	while(c != EOF){
		printf("%c", c);
		c = fgetc(fp);
		if ( c == '\n' || c == '\t' || c == ' ') {
			++word_count;
		}
	}
	fclose(fp);
	printf("The word count in the file is: %d\n", word_count);

	printf("-------------------------------------------\n");
	fprintf(stderr, "\n\n\nPlease Type The Program Starting NOW!\n>>> ");
	
	struct termios newt;
	tcgetattr(0, &oldt); // save terminal settings
	newt = oldt;
	newt.c_lflag &= ~(ICANON); // change settings
	tcsetattr(0, TCSANOW, &newt); // apply settings
	
	
		pthread_t tid;
	pthread_create(&tid, NULL, typer, NULL);
	struct timeval t1, t2;
	gettimeofday(&t1, NULL);
	t2.tv_sec = t1.tv_sec + 10;
	//printf("time 1: %d\n time2: %d\n", t1.tv_sec, t2.tv_sec);
	while(t1.tv_sec < t2.tv_sec) {
		gettimeofday(&t1, NULL);
	}
	
	stop_thread = 1;
	pthread_cancel(tid);
	fprintf(stderr, "\nyou entered: %s\n", ReadBuffer);
	tcsetattr(0, TCSANOW, &oldt);
	//int done;
	//scanf("%d", done);
	return 0;
}