#include "schedulers.h"
#include "task.h"
#include "list.h"
#include "cpu.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

// head node of the list
struct node* g_head;

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
Task *pickNextTask() {
  // if list is empty, nothing to do
  if (!g_head)
    return NULL;

  struct node *temp;
  temp = g_head;
  Task *best_sofar = temp->task;

  while (temp != NULL) {
    if (comesBefore(temp->task->name, best_sofar->name))
      best_sofar = temp->task;
    temp = temp->next;
  }
  // delete the node from list, Task will get deleted later
  delete (&g_head, best_sofar);
  return best_sofar;
}

// invoke the scheduler
void schedule()
{
    // iterate through the list of tasks
    while (g_head != NULL)
    {
        Task* task = (Task*) malloc(sizeof(Task));

        // identify the next task to be run
        task = pickNextTask();

        // run the next task
        run(task, task->burst);

        // increment the current time
        time += task->burst;
        printf("    Time is now: %d\n", time);
    }
}

