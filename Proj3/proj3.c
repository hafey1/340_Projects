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
	//assert(sem_wait(&wholeText->empty) == 0);
	assert(pthread_mutex_lock(&wholeText->queue_lock) == 0);
	
	wholeText->bufferLine[wholeText->fill] = (char*) line;
	wholeText->fill = (wholeText->fill + 1) % (wholeText->q_len + 1);
	
	assert(pthread_mutex_unlock(&wholeText->queue_lock) == 0);
	//assert(sem_post(&wholeText->full) == 0);
}

char* get(QUEUE *wholeText) {
	//assert(sem_wait(&wholeText->full) == 0);
	assert(pthread_mutex_lock(&wholeText->queue_lock) == 0);
	char *temp = wholeText->bufferLine[wholeText->use];
	wholeText->use = (wholeText->use + 1) % (wholeText->q_len + 1);

	assert(pthread_mutex_unlock(&wholeText->queue_lock) == 0);
	//assert(sem_post(&wholeText->empty) == 0);

	return temp;
}


//the THREAD FUNCTION

void *wordCount(void *taskNum) {
	int *taskNumInt = (int *) taskNum;
	printf("\nThis is the taskNum = %d\n", (*taskNumInt));
	
	pthread_exit(NULL);

}

int main(int argc, char **arg) {
	
	if (argc != 2) {
		printf("Please enter int:<tasksToRun>\n");
	}

	char *thing = "the meme\n\0";
	char *secthing = "anothermeme\n\0";	
	//this is the queue being intialized
	//char *cbuffer[8];
	//QUEUE q = {0, 0, 8, cbuffer, PTHREAD_MUTEX_INITIALIZER};
	//assert(sem_init(&q.empty, 0, 8) == 0);
	//assert(sem_init(&q.full, 0, 0) == 0);
	//first round testing
	//put(&q, thing);
	//put(&q, thing);
	//char *an = get(&q);
	//char *gn = get(&q);
	
	//printf("\nThis is the meme: %s\n", thing);
	//printf("\nThis is the sec meme: %s\n",secthing);

	int lineCount = 0;
	char *text = calloc(1,1), buffer[BUFFERSIZE];
	//now to get stuff from stdin black magic dont question
	while (fgets(buffer, BUFFERSIZE, stdin)) {
		text = realloc( text, strlen(text)+1+strlen(buffer));
		printf("\n\nThe length of line is %ld\n\n", strlen(buffer));
		lineCount++;
		if (!text) {
			printf("Error text pointer is null");

			exit(1);
		}
		strcat(text, buffer);
		//printf("%s\n", buffer);
	}
	//making sure it has trailing null
	strcat(text, "\0");
	printf("\ntext:\n%s", text);
	
	char *cBuffer[lineCount];
        QUEUE q = { 0, 0, lineCount, cBuffer, PTHREAD_MUTEX_INITIALIZER};
        //assert(sem_init(&q.empty, 0, lineCount) == 0);
        //assert(sem_init(&q.full, 0, 0) == 0);


	//now turning the text string into lines by delimiting with \n then adding on a null
	char *lineOnQueue = malloc(sizeof(BUFFERSIZE));
	lineOnQueue = strtok(text, "\n");
	printf("lineOnQueue = %s\n", lineOnQueue);
	for (int i = 0; i < lineCount - 2; i++) {
		lineOnQueue = strtok(NULL, "\n");
		lineOnQueue = strcat(lineOnQueue, "\0");
		printf("lineOnQueue = %s\n", lineOnQueue);
		char *goingOn = malloc(sizeof(BUFFERSIZE));
		strcpy(goingOn, lineOnQueue);
		printf("this is goingOn = %s\n", goingOn);
		put(&q, goingOn);
	
	}
	
	//testing getting from the queue
	for (int i = 0; i < lineCount - 2; i++) {


		char *pleaseGod = get(&q);
		printf("please god be a line: %s\n", pleaseGod);
	}

	int tasksToRun = atoi(arg[1]);
	
	pthread_t threadID[tasksToRun];
	
	int taskNum[tasksToRun];
	for (int i = 0; i < tasksToRun; i++){
		taskNum[i] = i;
	}
	for (int i = 0; i < tasksToRun; i++){ 
		
		int errorCheck = pthread_create(&threadID[i], NULL, &wordCount, (void *) taskNum[i]);
		if (errorCheck) {
			printf("Failed to create Thread %d", i);
			exit(1);	
		}
		
	}

	
	for (int i = 0; i < tasksToRun; i++){
		pthread_join(threadID[i], NULL);
	}

	return 0;
}
