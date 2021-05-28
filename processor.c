/*
 * processor.c
 *
 *  Created on: 18 Mar 2021
 *      Author: USER
 */
#include "processor.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#define HIGHEST_NUMBER 9999999

Processor* new_processor(int id){
	Processor* new_processor = malloc(sizeof(Processor));
	new_processor->current_process = new_process(-1, -1, -1, 'p', -1, -1);
	new_processor->process_queue = new_linked_list();
	new_processor->already_allocated = 0;
	new_processor->last_process = new_process(-1, -1, -1, 'p', -1, -1);
	new_processor->is_last_process = 0;
	new_processor->id = id;
	return new_processor;
}

Linked_list* processor_execute_current_second(Processor* processors[], Linked_list* finished_processes, int number_of_processors, int current_time){
	Linked_list* processes_that_just_finished = new_linked_list();
	//for each processor
	for(int i=0;i<(number_of_processors);i++){
		//execute one second of the current running process
		if(processors[i]->current_process&&processors[i]->already_allocated){
			processors[i]->current_process->time_remaining-=1;
			processors[i]->current_process->time_elapsed+=1;
		}
		if(processors[i]->already_allocated){
			//if the current process is finished
			if(processors[i]->current_process->time_remaining<=0){
				linked_list_add_to_foot(processes_that_just_finished, processors[i]->current_process);
				processors[i]->current_process->finished_time = current_time;
				
				//add it to the finished processes list.
				linked_list_add_to_foot(finished_processes, processors[i]->current_process);
				linked_list_remove_process(processors[i]->process_queue, processors[i]->current_process);
				if(linked_list_length(processors[i]->process_queue)==0){
					processors[i]->already_allocated =0;
				}
				//processors[i]->current_process = NULL;
			}
		}
	}
	node* process = processes_that_just_finished->head;
	while(process){
		//iterate over the processors counting how many processes have already been allocated and not completed
		Linked_list* processes_already_allocate_at_finish_time = new_linked_list();
		for(int h=0;h<number_of_processors;h++){
			if(processors[h]->already_allocated==1){
				node* current_process = processors[h]->process_queue->head;
				while(current_process){
					//TODO: check that you arent counting sub processes
					if(current_process->process->time_remaining>0){
						linked_list_add_to_foot(processes_already_allocate_at_finish_time, current_process->process);
					}
					current_process = current_process->next;
				}
			}
		}
		processes_already_allocate_at_finish_time  = make_processes_unique(processes_already_allocate_at_finish_time);
		process->process->processes_remaining_at_finish_time = linked_list_length(processes_already_allocate_at_finish_time);
		process = process->next;
	}
	return finished_processes;
}

Processor* get_processor_with_shortest_total_run_time(Processor* processors[], int number_of_processors){
	//iterate over processors
	int shortest_run_time = HIGHEST_NUMBER;
	Processor* shortest_run_time_processor;
	//get the processor with the shortest run time remaining across all of the processes allocated to it
	for(int i=0;i<number_of_processors;i++){
		//if there is a processor not allocated to, return it
		if(!processors[i]->already_allocated){
			return processors[i];
		}
		//otherwise get the processor with the shortest run time remaining
		if(processors[i]->already_allocated){
			int total_run_time = get_total_run_time(processors[i]);
			if(shortest_run_time> total_run_time){
				shortest_run_time = total_run_time;
				shortest_run_time_processor = processors[i];
			}
		}
	}
	return shortest_run_time_processor;
}

int get_total_run_time(Processor* processor){
	// calculate the total run time remaining across all of the processes allocated to it
	node* current_process = processor->process_queue->head;
	int total_run_time_remaining = 0;
	while(current_process){
		total_run_time_remaining += current_process->process->time_remaining;
		current_process = current_process->next;
	}
	return total_run_time_remaining;
}

Process* get_process_with_shortest_run_time_remaining(Processor* processor){
	if(linked_list_length(processor->process_queue)==1){

		return processor->process_queue->head->process;
	}
	node* current_process = processor->process_queue->head;
	int shortest_run_time_remaining = HIGHEST_NUMBER;
	Process* shortest_run_time_remaining_process;
	// get the process with the shortest run time remaining that is allocated to this processor
	while(current_process){
		if(current_process->process->time_remaining<shortest_run_time_remaining){
			shortest_run_time_remaining = current_process->process->time_remaining;
			shortest_run_time_remaining_process = current_process->process;
		}
		current_process = current_process->next;
	}
	//return the process with the shortest run time remaining
	return shortest_run_time_remaining_process;
}
