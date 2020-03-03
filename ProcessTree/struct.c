// Structure Sample

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct context {
	int insp;
	int stkp;
	int AX;
	int BX;
};

enum proc_state {initializing, running, suspended, ready, terminated};

struct process {

   	int pid;
   	struct context context;
	enum proc_state state;
	struct process *next;
	struct process *prev;
};

int main(int argc, char *argv[]) 
{

    //Creates a double linked list of structures
    
    struct process *head = NULL;
    struct process *mi_prev = NULL;
	struct process *tail = NULL;
    
    for (int i = 0; i < 10; i++) {
    
        //Allocates and initializes a new element of the list
        
        struct process *mi_new = malloc(sizeof(struct process));
        
        mi_new->pid = i;
    
        //Attaches the new node to the list
        
        if (head == NULL)
            head = mi_new;
        else{
            mi_prev->next = mi_new;
            mi_new->prev = mi_prev;
        //Saves this nodes address
	}
        mi_prev = mi_new;
    	mi_new->next = tail;
    }

    // Walk through the list
    
    struct process *node = head;
    
    while (node != NULL) {
    
        printf("(%p) field1 <%d>, next <%p>, prev <%p>\n", node, node->pid, node->next, node->prev);
    
        node = node->next;
    
    }
	printf("\n");
	struct process *backnode = tail;
    
    while (backnode->prev != NULL) {
    
        printf("(%p) field1 <%d>, next <%p>, prev <%p>\n", 
               backnode, backnode->pid, backnode->next, backnode->prev);
    
        backnode = backnode->prev;
    
    }
	
	return 0;
}
