#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>

#define BUFFERSIZE 1024              //the max size of a line

typedef struct {
	int             fill;        //next point to fill in the queue
	int             use;         //next point to get in the queue
	int             q_len;       //The MAX size of the queue
	char            **bufferLine; //buffer for adding and removing line values
	pthread_mutex_t queue_lock; 
	sem_t           empty;
	sem_t           full;
} QUEUE;

//for possible argument parameters into threads
typedef struct {
	int Task_number;
	//make sure to malloc this line
	char * lineAssigned;
} ontoQueue;

void put(QUEUE *wholeText, char *line){
	assert(sem_wait(&wholeText->empty) == 0);
	assert(pthread_mutex_lock(&wholeText->queue_lock) == 0);
	
	wholeText->bufferLine[wholeText->fill] = (char*) line;
	wholeText->fill = (wholeText->fill + 1) % wholeText->q_len;
	
	assert(pthread_mutex_unlock(&wholeText->queue_lock) == 0);
	assert(sem_post(&wholeText->full) == 0);
}

char* get(QUEUE *wholeText) {
	assert(sem_wait(&wholeText->full) == 0);
	assert(pthread_mutex_lock(&wholeText->queue_lock) == 0);

	char *temp = wholeText->bufferLine[wholeText->use];
	wholeText->use = (wholeText->use + 1) % wholeText->q_len;

	assert(pthread_mutex_unlock(&wholeText->queue_lock) == 0);
	assert(sem_post(&wholeText->empty) == 0);

	return temp;
}

int main(int argc, char **arg) {
	
	if (argc != 2) {
		printf("Please enter int:<tasksToRun>\n");
	}

	char *cbuffer[3];
	QUEUE q = {0, 0, 3, cbuffer, PTHREAD_MUTEX_INITIALIZER};
	assert(sem_init(&q.empty, 0, 3) == 3);
	assert(sem_init(&q.full, 0, 0) == 0);

	char *text = calloc(1,1), buffer[BUFFERSIZE];
	//now to get stuff from stdin
	while (fgets(buffer, BUFFERSIZE, stdin)) {
		text = realloc( text, strlen(text)+1+strlen(buffer));
		printf("\n\nThe length of line is %ld\n\n", strlen(buffer));
		if (!text) {
			printf("Error text pointer is null");

			exit(1);
		}
		strcat(text, buffer);
		printf("%s\n", buffer);
	}
	printf("\ntext:\n%s", text);


	//this will be the stdin
	//FILE *input;
	//char *fileName = arg[2];
	int tasksToRun = atoi(arg[1]);
	
	printf("%d sd", tasksToRun);

	return 0;
}
