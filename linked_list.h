/*
 * linked_list.h
 *
 *  Created on: 20 Mar 2021
 *      Author: USER
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#include "process.h"

typedef struct linked_list Linked_list;
typedef struct node node;

struct node {
  Process* process;
  node* next;
  node* prev;
};

struct linked_list {
  node* head;
  node* foot;
};



Linked_list* new_linked_list();
void set_head(Linked_list* list, node* value);
void set_foot(Linked_list* list, node* value);
void free_linked_list(Linked_list *list);
void linked_list_add_to_head(Linked_list *list, Process *process);
void linked_list_add_to_foot(Linked_list *list, Process *process);
Process* linked_list_remove_from_head(Linked_list *list);
Process* linked_list_remove_from_foot(Linked_list *list);
int linked_list_remove_process(Linked_list* list, Process* process);
int linked_list_length(Linked_list* list);
void print_linked_list(Linked_list* list);
Linked_list* make_processes_unique(Linked_list* list);


#endif /* LINKED_LIST_H_ */
