#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// size of time quantum
const int TQ = 10;

// head node of the list
struct node* g_head;

// head node for temporary list
struct node* temp_head;

// counter to increment and create tid's
int tid = 1;

// var to keep track of time
int time = 0;

// add a task to the list 
void add(char *name, int priority, int burst)
{
    // create the task to be inserted into the list
    Task* task = (Task*) malloc(sizeof(Task));
    task->name = name;
    task->priority = priority;
    task->burst = burst;
    task->tid = tid;

    // increment tid for next task
    tid++;

    // insert the task into the list
    insert(&g_head, task);
}

bool comesBefore(char *a, char *b) { return strcmp(a, b) < 0; }

// based on traverse from list.c
// finds the task whose name comes first in dictionary
Task* pickNextTask() 
{
    // if list is empty, nothing to do
    if (!g_head)
        return NULL;

    struct node* temp;
    temp = g_head;
    Task* best_sofar = temp->task;

    while (temp != NULL) {
        if (comesBefore(temp->task->name, best_sofar->name))
        {
            best_sofar = temp->task;
        }
        temp = temp->next;
    }
    // delete the node from list, Task will get deleted later
    delete (&g_head, best_sofar);
    return best_sofar;
}


// invoke the scheduler
void schedule()
{
    temp_head = NULL;
    // iterate through the list of tasks
    while (g_head != NULL)
    {
        Task* task = NULL;
        // identify the next task to be run
        task = pickNextTask();

        int run_time = -1;
        if (task->burst < TQ)
        {
            run_time = task->burst;
        }
        else if (task->burst >= TQ)
        {
            run_time = TQ;
        }

        // run the next task
        run(task, run_time);

        // subtract current run time from total burst time
        task->burst -= run_time;

        // if there is remaining burst time in the process, add it to the temp list
        if (task->burst > 0)
        {
            insert(&temp_head, task);
        }

        // increment the current time
        time += run_time;
        printf("    Time is now: %d\n", time);

        // if the main list is empty and the temp list is not, add the values from the temp list into the main list
        if (g_head == NULL && temp_head != NULL)
        {
            struct node* temp = temp_head;
            while (temp != NULL)
            {
                insert(&g_head, temp->task);
                temp = temp->next;
            }
            
            // delete all values from temp list
            while (temp_head != NULL)
            {
                delete(&temp_head, temp_head->task);
            }
        }
    }
}