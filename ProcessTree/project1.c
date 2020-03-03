#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

// the base of this code is can be found from geeksforgeeks.org/c-program-list-files-sub-directories-directory/
// and in the struct.c file provided along with this file in the repo
struct process{
	int pid;
	int ppid;
	char *comm;
	unsigned long int vsize;
	int level;
	struct process *next;
};

//because we are in C we must free the memory after we are done using it
void freeTheLinkedList(struct process *start) {
	
	struct process *beFree;

	while ((beFree = start) != NULL) {
	start = start->next;
	free(beFree);
	}
}


// this is a recursive function that takes a pointer to the beginning of the linked list of processes, and the pointer to the established parent process
void printOutFileSystem(struct process *start, struct process *parent) {

	//the childFinder must always check the whole list so must start from the beginning
	//the parent cross reference is passed in from the recursive function call
	struct process *childFinder = start;
	struct process *parentChecker = parent;
	
	//this piece allows us to adjust spaces to be printed out according to the level of the directory
	//which is always 1 more than the parents
	char spaces[250]= "";
	for (int i = 0; i < parentChecker->level; i++) {
		strcat(spaces, "  ");
	}

	//this is the string that will contain the desired information about the process
	char processInfo[250] = "-";
	sprintf(processInfo, "pid: %d :: %s :: ppid: %d :: memory used: %lu bytes\n", parentChecker->pid, parentChecker->comm, parentChecker->ppid, parentChecker->vsize);
	strcat(spaces, processInfo);
	printf("%s", spaces);

	//the stop condition is when the current function has checked for all possible children and is at the end of the list
	//when this happens it will wind back to previous calls and exit appropriately..... hopefully
	while (childFinder != NULL) {
		if (parentChecker->pid == childFinder->ppid) {
			//when you establish a child parent relation the level of the child must be 1 more than the parent, this is useful for the output format desired
			childFinder->level = parentChecker->level + 1;
			printOutFileSystem(start, childFinder);
		}

			//if we did not find a child at this location we need to move on to the next one
			childFinder = childFinder->next;
	}
}



int main(int argc, char **args){

	// prev will save the process to set the next
	struct process *prev = NULL;
	// starting process of zero (known as the swapper process, will be the highest level "process")
	struct process *zero = malloc(sizeof(struct process));
	zero->pid = 0;
	zero->ppid = -1;
	zero->comm = "zero";
	zero->vsize = 0;
	zero->level = 0;
	zero->next = NULL;
	prev = zero;

	//we use regex to capture processes that have the first character in the name as a digit value
	int rv;
	regmatch_t match[1000];
	regex_t exp;
	rv = regcomp(&exp, "^[0-9]", REG_EXTENDED);
	FILE *fp;
       	struct dirent *de;  // Pointer for directory entry

    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir("/proc");

    if (dr == NULL)  // opendir returns NULL if couldn't open directory
    {
        printf("Could not open current directory" );
        return 0;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()
    while ((de = readdir(dr)) != NULL){
		//this takes the pointer to the compiled regular expression and runs it through the current directory for a match
		//luckily if it is a match it returns a 0
	    rv = regexec(&exp, de->d_name, 1000, match, REG_NOTEOL);

	    if( rv == 0)   {
		    	//the black magic of importing every piece of information necessary to get to VSize :)
			//the string must be allocated in order to be assigned properly to each individual structure
		    	char *comm;
			comm = malloc(sizeof(char)*30);
			
			char state;
			int pid, ppid, pgrrp, session, tty_nr, tpgid;
			unsigned int flags;
			unsigned long int minflt, cminflt, majflt, cmajflt, utime, stime, vsize;
			long int cutime, cstime, priority, nice, num_threads, itrealvalue;
			unsigned long long int starttime;

			char *procnum = malloc(12 + strlen(de->d_name));
			strcpy( procnum, "/proc/");
			strcat( procnum, de->d_name);
			strcat( procnum, "/stat");
		    	fp = fopen(procnum, "r");

			// Defining how to read the stat file, up to the memory size(vsize)
			fscanf(fp, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %ld %ld %llu %lu",
							&pid, comm, &state, &ppid, &pgrrp, &session, &tty_nr, &tpgid, &flags,
							&minflt, &cminflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &num_threads,
							&itrealvalue, &starttime, &vsize);
				// Allocate space and set information from file
				struct process *new_proc = malloc(sizeof(struct process));
				// Linke previous to the new structure
				prev->next = new_proc;
				new_proc->pid = pid;
				new_proc->ppid = ppid;
				new_proc->comm = comm;
				new_proc->vsize = vsize;
				new_proc->level = 0;
				new_proc->next = NULL;
				prev = new_proc;
	    }
  	}

	struct process *cats = zero;
	printOutFileSystem(zero, cats);

  closedir(dr);
	
  	//two different ways of freeing the structures
	//clear that data
	/*struct process *clearPointer;
	struct process *otherPoint;
	clearPointer = zero;
	while(clearPointer != NULL){
		otherPoint = clearPointer;
		clearPointer = clearPointer->next;
		free(otherPoint);
	}*/
	
	freeTheLinkedList(zero);

    return 0;
}
