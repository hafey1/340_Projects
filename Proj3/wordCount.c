#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <assert.h>
#include <semaphore.h>

#define BUFFERSIZE 1024              //the max size of a line
//heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeerrrrrrrrrrrrrrrrrrrrrreeeeeeeeeeeeeeeeee
int totalWords;
//hhhheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeerrrrreee

typedef struct {
	int             fill;        //next point to fill in the queue
	int             use;         //next point to get in the queue
	int             q_len;       //The MAX size of the queue
	char            **bufferLine; //buffer for adding and removing line values
	pthread_mutex_t queue_lock;
	//sem_t           empty;
	//sem_t           full;
} QUEUE;

//for possible argument parameters into threads
typedef struct {
	int taskNumber;
	QUEUE* bonqueque;
} ontoThread;

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

void *wordCount(void *taskInfo) {
	//struct ontoThread *taskNumInt;
	ontoThread *argsPassed = (ontoThread *) taskInfo;
	//printf("\nThis is the taskNum = %d\n", (*taskNumInt));
	int Tn = argsPassed->taskNumber;
	char *big = get(argsPassed->bonqueque);
	printf("\nComing from wordCount: %s\n", big);

	// here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	int lineWords = 1;

	for(int i = 0; big[i] != '\0'; i++){

		if(big[i] == ' ')
		{
			lineWords++;
		}
	}

	totalWords = totalWords + lineWords;

	printf("The line contains %d words from task number: %d\n", lineWords, Tn);
	//here!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	pthread_exit(NULL);

}

int main(int argc, char **arg) {

	if (argc != 2) {
		printf("Please enter int:<tasksToRun>\n");
	}

	//this is the queue being intialized
	//char *cbuffer[8];
	//QUEUE q = {0, 0, 8, cbuffer, PTHREAD_MUTEX_INITIALIZER};
	//assert(sem_init(&q.empty, 0, 8) == 0);
	//assert(sem_init(&q.full, 0, 0) == 0);
	totalWords = 0;
	int lineCount = 0;
	char *text = calloc(1,1), buffer[BUFFERSIZE];
	//now to get stuff from stdin black magic dont question
	while (fgets(buffer, BUFFERSIZE, stdin)) {
		text = realloc( text, strlen(text)+1+strlen(buffer));
		//printf("\n\nThe length of line is %ld\n\n", strlen(buffer));
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
	//printf("\ntext:\n%s", text);

	char *cBuffer[lineCount];
        QUEUE q = { 0, 0, lineCount, cBuffer, PTHREAD_MUTEX_INITIALIZER};
        //assert(sem_init(&q.empty, 0, lineCount) == 0);
        //assert(sem_init(&q.full, 0, 0) == 0);


	//now turning the text string into lines by delimiting with \n then adding on a null
	char *lineOnQueue = malloc(BUFFERSIZE);
	lineOnQueue = strtok(text, "\n");
	lineOnQueue = strcat(lineOnQueue, "\0");
	put(&q, lineOnQueue);
	//printf("lineOnQueue = %s\n", lineOnQueue);
	for (int i = 0; i < lineCount - 2; i++) {
		lineOnQueue = strtok(NULL, "\n");
		lineOnQueue = strcat(lineOnQueue, "\0");
		//printf("lineOnQueue = %s\n", lineOnQueue);
		//
		char *goingOn = malloc(BUFFERSIZE);
		strcpy(goingOn, lineOnQueue);
		//
		printf("this is goingOn = %s\n", goingOn);
		put(&q, goingOn);

	}
	//printf("now we did the thing\n");
	//testing getting from the queue

	int tasksToRun = atoi(arg[1]);
 //printf("bleh\n");

	pthread_t threadID[tasksToRun];
 //printf("dude\n");

	ontoThread onto[tasksToRun];
//	 printf("now we did the thing\n");

	int numSeq[tasksToRun];

	for (int i = 0; i < tasksToRun; i++) {
		numSeq[i] = i;
	}

	for (int i = 0; i < tasksToRun; i++) {
//		printf("\nhaha im going onto onto %d\n", i);
		onto[i].taskNumber = numSeq[i];
		int ins = onto[i].taskNumber;
		// danger
		onto[i].bonqueque = &q;
		QUEUE *bigpoint = onto[i].bonqueque;
		// danger
//		printf("we just added stuff\n%p <--address of queue\n %d\nhahapsychthiswontcompile\n", (void *)bigpoint, ins);
	}

	for (int i = 0; i < tasksToRun; i++){
//		printf("this is right before the thread creation\n");
		assert(pthread_create(&threadID[i], NULL, &wordCount, (void *) &onto[i]) == 0);
	}

	for (int i = 0; i < tasksToRun; i++){
		pthread_join(threadID[i], NULL);
	}
	//heeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeerrrrrre
	printf("\nAll the words: %d\n\n", totalWords);
	//hhhhhhhhhhhhhhhhhheeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeere
	return 0;
}
