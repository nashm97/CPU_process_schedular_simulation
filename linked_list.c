/*
 * Doubly_linked_list.c
 *
 *  Created on: 20 Mar 2021
 *      Author: USER
 */
#include "linked_list.h"
#include "process.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

Linked_list* new_linked_list(){
	Linked_list* new_linked_list = malloc(sizeof(Linked_list));
	 set_head(new_linked_list, NULL);
	 return new_linked_list;
}

// sets the value for the head and the foot if appropriate
void set_head(Linked_list* list, node* value){
  if(value == NULL){
    list->foot = NULL;
  }
  if((list->head == NULL) && (list->foot == NULL)){
    list->foot = value;
  }
  list->head = value;
}

// sets the value for the foot and the head if appropritate
void set_foot(Linked_list* list, node* value){
  if(value == NULL){
    list->head = NULL;
  }
  if((list->head == NULL) && (list->foot == NULL)){
    list->head = value;
  }
  list->foot = value;
}

void free_linked_list(Linked_list *list) {
  node* tmp;
  while(list->head != NULL){
    tmp = list->head;
    set_head(list, list->head->next);
    free(tmp);
  }
}


void linked_list_add_to_head(Linked_list *list, Process *process) {
  node* new_node = malloc(sizeof(node));
  new_node->process = process;
  //if head and foot are NULL initialise
  if((list->head == NULL) && (list->foot == NULL)){
    set_head(list, new_node);
    list->head->prev = NULL;
    list->head->next = NULL;
  }
  //otherwise push to the head
  else{
    new_node->next = list->head;
    new_node->prev = NULL;
    list->head->prev = new_node;
    set_head(list, new_node);
  }
}


void linked_list_add_to_foot(Linked_list *list, Process *process) {
  node* new_node = malloc(sizeof(node));
  new_node->process = process;
  //if head and foot are NULL initialise
  if((list->head == NULL) && (list->foot == NULL)){
    set_foot(list, new_node);
    list->head->prev = NULL;
    list->head->next = NULL;
  }
  else{
    //otherwise insert to the foot
    new_node->prev = list->foot;
    new_node->next = NULL;
    list->foot->next = new_node;
    set_foot(list, new_node);
  }
}


Process* linked_list_remove_from_head(Linked_list *list) {
  Process* process = list->head->process;
  //if final node set head and foot
  if(list->head == list->foot){
    set_head(list, list->head->next);
  }
  //otherwise pop appropriatly
  else{
    set_head(list, list->head->next);
    list->head->prev = NULL;
    free(list->head->prev);
  }
  return process;
}


Process* linked_list_remove_from_foot(Linked_list *list) {
  Process* process = list->foot->process;
  //if final node set head and foot
  if(list->head == list->foot){
    set_foot(list, list->foot->prev);
  }
  else{
    set_foot(list, list->foot->prev);
    list->foot->next = NULL;
    free(list->foot->next);
  }
  return process;
}

int linked_list_remove_process(Linked_list* list, Process* process){
	if(compare_processes(process, list->head->process)){
		linked_list_remove_from_head(list);
		return 1;
	}
	if(compare_processes(process, list->foot->process)){
		linked_list_remove_from_foot(list);
		return 1;
	}
	node* current_node = list->head;
	while(current_node){
		if(compare_processes(process, current_node->process)){
			current_node->next->prev = current_node->prev;
			current_node->prev->next = current_node->next;
			current_node->prev = NULL;
			current_node->next = NULL;
			return 1;
		}
		current_node = current_node->next;
	}
  return 0;
}

int linked_list_length(Linked_list* list){
	node* current_node = list->head;
	int list_length =0;
	while(current_node){
    if(current_node->process->id == -1){
      printf(" THIS THING HAPPENED \n");
      break;
    }
		list_length+=1;
		current_node = current_node->next;
	}
	return list_length;
}

void print_linked_list(Linked_list* list){
	node* current_process = list->head;
	while(current_process){
		printf("id = %f allocate_time = %d time_rem = %d is parralisable = %c finished_time = %d time_elapsed = %d\n"
    ,current_process->process->id, current_process->process->allocate_time, current_process->process->time_remaining, 
    current_process->process->is_parallelisable, current_process->process->finished_time, current_process->process->time_elapsed);
		
    current_process = current_process->next;
	}
}

Linked_list* make_processes_unique(Linked_list* list){
  node* current_process = list->head;
  while(current_process){
    node* process_to_compare = list->head;
    while(process_to_compare){
      //remove the process that has the same base id
      if((current_process->process->id!= process_to_compare->process->id)&&(current_process->process->base_id==process_to_compare->process->base_id)){
        linked_list_remove_process(list, process_to_compare->process);
      }
      process_to_compare = process_to_compare->next;
    }
    current_process = current_process->next;
  }

  return list;
}

