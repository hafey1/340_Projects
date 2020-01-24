#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
//#include </usr/include/linux/sched.h>
#include <regex.h>
#include <string.h>

// this code is borrowed from geeksforgeeks.org/c-program-list-files-sub-directories-directory/

//we have 1 PID, we can get 4  PPID and 23 VSIZE and 2 COMM from fopen and fscanthen we can cross reference every PID against every other process's PPID and when there is a match the PID needs to link to the child process. Then some sort ofQUEUE to help with the tabbing of the output to console.


// Structure of a process
struct process{
	int pid;
	int ppid;
	char *comm;
	unsigned long int vsize;
	int level;
	struct process *next;
};

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

	int rv;
	regmatch_t match[1000];
	regex_t exp;
	rv = regcomp(&exp, "^[0-9]", REG_EXTENDED);
	FILE *fp;
	//struct pid *superpoint = task_pid(1);
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
	//printf("%s\n", de->d_name);
	    rv = regexec(&exp, de->d_name, 1000, match, REG_NOTEOL);
	   // printf("%d", rv);
	    if( rv == 0)   {
			char comm[250] = "cat";
			char state;
			int pid, ppid, pgrrp, session, tty_nr, tpgid;
			unsigned int flags;
			unsigned long int minflt, cminflt, majflt, cmajflt, utime, stime, vsize;
			long int cutime, cstime, priority, nice, num_threads, itrealvalue;
			unsigned long long int starttime;
			char *procnum = malloc(12 + strlen(de->d_name));
			//procnum = strcat("/proc/", de->d_name);
			//procnum = strcat(procnum, "/stat");

			strcpy( procnum, "/proc/");
			strcat( procnum, de->d_name);
			strcat( procnum, "/stat");
		    	fp = fopen(procnum, "r");
			   // printf("%s\n", de->d_name);
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

    closedir(dr);
    return 0;
}
