#include <stdio.h> 
#include <dirent.h> 
#include <stdlib.h>
//#include </usr/include/linux/sched.h>
#include <regex.h>

// this code is borrowed from geeksforgeeks.org/c-program-list-files-sub-directories-directory/
int main(int argc, char **args) 
{	int rv;
	regmatch_t match[100];
	regex_t exp;
	rv = regcom(&exp, "^[0-9]+", REG_NOSUB);

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
	if(regexec(&exp, d, 100, match) == 0)   
	    printf("%s\n", de->d_name); 
  	}
    closedir(dr);     
    return 0; 
} 

