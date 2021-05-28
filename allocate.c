/*
 * allocate.c
 *
 *  Created on: 18 Mar 2021
 *      Author: MILO NASH
 */
#include "process.h"
#include "allocate.h"
#include "processor.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#define HIGHEST_NUMBER 9999999

int current_time;

int main(int argc, char* argv[]){
	//TODO: read in line by line the process information
	current_time = -1;
	int opt;
	Linked_list* finished_processes = new_linked_list();
	char* file_name;
	int number_of_processors;
	int cflag = 0;
	Linked_list* processes;

	while((opt = getopt(argc, argv, "fp:c"))!=-1){

		switch(opt){

			case 'f':
				file_name = argv[optind];
				break;
			case 'p':
				//TODO: this only reads one digit
				number_of_processors = atoi(optarg);
				break;
			case 'c':
				cflag = 1;

				break;
			case '?':
				printf("option arguments missing");
				break;
			case ':':
				break;
		}
	}
	Processor* processors[number_of_processors];
	for(int i=0;i<number_of_processors;i++){
		processors[i] = new_processor(i);
	}
	if(cflag){
		processes = create_processes_from_input(file_name, number_of_processors, 5);
	}
	else{
		processes = create_processes_from_input(file_name, number_of_processors, 1);
	}

	node* current_process = processes->head;
		
	processes = sort_processes_by_id(processes);
	int initial_number_of_processes_after_parallelising = linked_list_length(processes);

 	//count how many processes and subprocesses there are in total, in order to gauge when the processing is finished
	current_process = processes->head;
	int number_of_processes_and_sub_processes = 0;
	while(current_process){
		number_of_processes_and_sub_processes +=1;
		current_process = current_process->next;

	}
	while((current_time++)+2){

		finished_processes = processor_execute_current_second(processors, finished_processes, number_of_processors, current_time);
		//count how many processes and subprocesses have been finished
		current_process = finished_processes->head;
		//int number_of_finished_processes = linked_list_length(finished_processes);

		if(linked_list_length(finished_processes)==initial_number_of_processes_after_parallelising){
			break;
		}

		allocate_processes_for_next_second(processors, processes, number_of_processors);


	}
	//int turnaround_time = calculate_turnaround_time(finished_processes);
	print_results(finished_processes, current_time);
	return 0;
}


Linked_list* create_processes_from_input(char* file_name, int number_of_processors, int restriction_on_number_of_sub_porcesses){
	FILE *file;
	Linked_list* processes = new_linked_list();
	file = fopen(file_name, "r");
	if(!file){
		printf("ERROR_FILE_DID_NOT_OPEN");
	}
	int allocate_time, id;
	float run_time;
	char parallelisable;
	char line[100];
	while(fscanf(file,"%[^\n]\n", line) != EOF){
		sscanf(line, "%d %d %f %c",&allocate_time, &id, &run_time, &parallelisable);
		//if the process is parallelisable, break it into k subprocesses with run_time = runtime/k
		if(parallelisable == 'p'){
			int i;
			float original_run_time = run_time;
			for(i=1; (((run_time/i)>restriction_on_number_of_sub_porcesses)&&(i<number_of_processors));i++){
			}
			float rounded_run_time;
			if((run_time/i) - (int)(run_time/i)>0){
				rounded_run_time = (int)(run_time/i) +1;
			}
			else{
				rounded_run_time = (int)(run_time/i);
			}
			for(int j=0;j<i;j++){
				linked_list_add_to_foot(processes, new_process(allocate_time, id+(j/10.0), (int)rounded_run_time+1, parallelisable, (float)id, original_run_time));
			}
		}
		else{
			// otherwise add the process normally
			linked_list_add_to_foot(processes, new_process(allocate_time, (float)id, (int)run_time, parallelisable, (float)id, (int)run_time));
		}
	}
	return processes;
	/*
	char current_char='';
	for(int i=0;current_char!='\n';i++){


	}*/
}


Linked_list* sort_processes_by_id(Linked_list* processes){
	Linked_list* processes_sorted = new_linked_list();
	while(processes->head){
		node* current_process = processes->head;
		node* next_earliest_process = malloc(sizeof(node));
		next_earliest_process->process = new_process(HIGHEST_NUMBER, HIGHEST_NUMBER, HIGHEST_NUMBER, 'n', HIGHEST_NUMBER, HIGHEST_NUMBER);
		while(current_process){
			if((current_process->process->allocate_time<next_earliest_process->process->allocate_time)||
					((current_process->process->allocate_time<=next_earliest_process->process->allocate_time)
					&&(current_process->process->id<next_earliest_process->process->id))){
				next_earliest_process = current_process;
			}
			current_process = current_process->next;
		}
		linked_list_remove_process(processes, next_earliest_process->process);
		linked_list_add_to_foot(processes_sorted, next_earliest_process->process);
	}
	return processes_sorted;
}


void allocate_processes_for_next_second(Processor* processors[], Linked_list* processes,  int number_of_processors){
	node* current_process = processes->head;
	//allocate the processes that need to be allocated for next second
	while(current_process){
		if(current_process->process->allocate_time == current_time){
			//get the processor with shortest run time remaining across all of its already allocated processes
			Processor* processor_with_shortest_total_run_time = get_processor_with_shortest_total_run_time(processors, number_of_processors);
			//allocate the process to the processor
			processor_with_shortest_total_run_time->already_allocated = 1;
			linked_list_add_to_foot(processor_with_shortest_total_run_time->process_queue, current_process->process);
			linked_list_remove_from_head(processes);
		}
		current_process= current_process->next;
	}
	//for each processor find the process that has the shortest time_remaining of all the processes allocated to it
	for(int i=0;(i<number_of_processors&&(linked_list_length(processors[i]->process_queue)!=0));i++){
		if(processors[i]->current_process){
			processors[i]->last_process = processors[i]->current_process;
			processors[i]->is_last_process = 1;
		}
		processors[i]->current_process = get_process_with_shortest_run_time_remaining(processors[i]);
		//add the current time to the start_times of the process just selected by the processor
		//this never happens
		//if the current selected process is diffrent from the one selected last second add the start time for the new process
		if(processors[i]->is_last_process){
			if(processors[i]->last_process->id!=processors[i]->current_process->id){
				processors[i]->current_process->actual_start_times[processors[i]->current_process->length_of_start_times_array] = current_time;
				processors[i]->current_process->time_remaining_at_start_time[processors[i]->current_process->length_of_start_times_array] = processors[i]->current_process->time_remaining;
				processors[i]->current_process->length_of_start_times_array+=1;
			}
		}
		processors[i]->current_process->processor_index = i;
	}
}

void print_results(Linked_list* finished_processes, int total_run_time){
	//for each second of the simulation
	for(int i=0; i<total_run_time+1;i=i+1){
		Linked_list* running_processes_to_print = new_linked_list();
		Linked_list* candidate_finished_processes_to_print = new_linked_list();
		node* current_process = finished_processes->head;
		//iterate over the finished processes
		while(current_process){
			//if the current process finished at the current time
			if(current_process->process->finished_time == i){
				//add the possible finished processes to a list
				linked_list_add_to_foot(candidate_finished_processes_to_print, current_process->process);
			}
			//create list of running processes for next second
			//iterate over all the start times for the current process
			for(int j=0;j<current_process->process->length_of_start_times_array;j++){
				//if the current process has the current time as a start time
				if(current_process->process->actual_start_times[j] == i){
					//add the process to the next set of processes running
					current_process->process->current_start_time_index_to_print = j;
					linked_list_add_to_foot(running_processes_to_print, current_process->process);
					break;
				}
			}
			current_process = current_process->next;
		}
		//iterate over the lost of candidate finished processes to print and check that they should be finished and are not waiting for a subprocess to finish
		node* candidate_process = candidate_finished_processes_to_print->head;
		while(candidate_process){
			node* process_to_compare = finished_processes->head;
			while(process_to_compare){
				if(candidate_process->process->base_id==process_to_compare->process->base_id){
					if(process_to_compare->process->finished_time>i){
						linked_list_remove_process(candidate_finished_processes_to_print, candidate_process->process);
					}
				}
				process_to_compare = process_to_compare->next;
			}
			candidate_process = candidate_process->next;
		}
		candidate_finished_processes_to_print = make_processes_unique(candidate_finished_processes_to_print);


		node* process = candidate_finished_processes_to_print->head;
		while(process){
			printf("%d,FINISHED,pid=%.0f,proc_remaining=%d\n", process->process->finished_time, process->process->id, process->process->processes_remaining_at_finish_time);
			process = process->next;
		}
		Linked_list* sorted_list_by_id = sort_processes_by_id(running_processes_to_print);
		process = sorted_list_by_id->head;
		while(process){
			if(process->process->is_parallelisable=='p'){
				printf("%d,RUNNING,pid=%.1f,remaining_time=%d,cpu=%d\n", i, process->process->id, process->process->time_remaining_at_start_time[process->process->current_start_time_index_to_print], process->process->processor_index);
			}
			else{
				printf("%d,RUNNING,pid=%.0f,remaining_time=%d,cpu=%d\n", i, process->process->id, process->process->time_remaining_at_start_time[process->process->current_start_time_index_to_print], process->process->processor_index);
			}
			process = process->next;
		}
	}
	printf("Turnaround time %d\n", calculate_turnaround_time(finished_processes));
	printf("Time overhead %.2f %.2f\n", calculate_max_time_overhead(finished_processes), calculate_average_time_overhead(finished_processes));
	printf("Makespan %d\n", total_run_time);
}

int calculate_turnaround_time(Linked_list* finished_processes){
	int turnaround_time = 0;
	node* current_process = finished_processes->head;
	int number_of_processes = 0;
	int did_parallelising_happen = 0;
	while(current_process){
		//handle paralellised processes, by getting the sub process with latest finish time 
		//and making sure to only do this calculation for the first sub process to finish
		if(current_process->process->is_parallelisable == 'p'){
			did_parallelising_happen = 1;
			node* process = finished_processes->head;
			int is_first_subprocess = 0;
			//iterate over the finished processes checking if the current process is the first sub process to finish
			while(process){
				if(current_process->process->id == process->process->id){
					is_first_subprocess =1;
					break;
				}
				else if((current_process->process->id!=process->process->id)&&(current_process->process->base_id==process->process->base_id)){
					is_first_subprocess = 0;
					break;
				}

				process = process->next;
			}
			node* paralellised_process = finished_processes->head;
			int process_finish_time = 0;
			while(paralellised_process){
				if((paralellised_process->process->base_id == current_process->process->base_id)){
					if(paralellised_process->process->finished_time>process_finish_time){
						process_finish_time = paralellised_process->process->finished_time;
					}
				}
				paralellised_process = paralellised_process->next;
			}
			if(is_first_subprocess){
				turnaround_time+=(process_finish_time - current_process->process->allocate_time);
				number_of_processes+=1;
			}
		}
		else{
			turnaround_time+=(current_process->process->finished_time - current_process->process->allocate_time);
		}
		if(current_process->process->is_parallelisable!='p'){
			number_of_processes+=1;
		}
		current_process=current_process->next;
	}
	float average_turnaround_time;
	if(did_parallelising_happen){
		average_turnaround_time = (turnaround_time/(float)number_of_processes)+1;
	}
	else{
		average_turnaround_time = (turnaround_time/(float)number_of_processes);
	}
	if((average_turnaround_time-(int)average_turnaround_time)>=0.5){
		return (int)average_turnaround_time+1;
	}
	else{
		return average_turnaround_time;
	}
}

float calculate_average_time_overhead(Linked_list* finished_processes){
	float time_overhead = 0;
	node* current_process = finished_processes->head;
	int number_of_processes = 0;
	while(current_process){
		//handle paralellised processes, by getting the sub process with latest finish time 
		//and making sure to only do this calculation for the first sub process to finish
		if(current_process->process->is_parallelisable == 'p'){
			node* process = finished_processes->head;
			int is_first_subprocess = 0;
			//iterate over the finished processes checking if the current process is the first sub process to finish
			while(process){
				if(current_process->process->id == process->process->id){
					is_first_subprocess =1;
					break;
				}
				else if((current_process->process->id!=process->process->id)&&(current_process->process->base_id==process->process->base_id)){
					is_first_subprocess = 0;
					break;
				}

				process = process->next;
			}
			node* paralellised_process = finished_processes->head;
			int process_finish_time = 0;
			while(paralellised_process){
				if((paralellised_process->process->base_id == current_process->process->base_id)){
					if(paralellised_process->process->finished_time>process_finish_time){
						process_finish_time = paralellised_process->process->finished_time;
					}
				}
				paralellised_process = paralellised_process->next;
			}
			if(is_first_subprocess){
				time_overhead+=((process_finish_time - current_process->process->allocate_time)/(float)current_process->process->original_run_time);
				number_of_processes+=1;
			}
		}
		else{
			time_overhead+=((current_process->process->finished_time - current_process->process->allocate_time)/(float)current_process->process->full_run_time);
		}
		if(current_process->process->is_parallelisable!='p'){
			number_of_processes+=1;
		}
		current_process=current_process->next;
	}
	float average_time_overhead = (time_overhead/(float)number_of_processes);

	return average_time_overhead;

}


float calculate_max_time_overhead(Linked_list* finished_processes){
	float max_time_overhead = 0;
	node* current_process = finished_processes->head;
	while(current_process){
		//handle paralellised processes, by getting the sub process with latest finish time 
		//and making sure to only do this calculation for the first sub process to finish
		if(current_process->process->is_parallelisable == 'p'){
			node* process = finished_processes->head;
			int is_first_subprocess = 0;
			//iterate over the finished processes checking if the current process is the first sub process to finish
			while(process){
				if(current_process->process->id == process->process->id){
					is_first_subprocess =1;
					break;
				}
				else if((current_process->process->id!=process->process->id)&&(current_process->process->base_id==process->process->base_id)){
					is_first_subprocess = 0;
					break;
				}

				process = process->next;
			}
			node* paralellised_process = finished_processes->head;
			int process_finish_time = 0;
			while(paralellised_process){
				if((paralellised_process->process->base_id == current_process->process->base_id)){
					if(paralellised_process->process->finished_time>process_finish_time){
						process_finish_time = paralellised_process->process->finished_time;
					}
				}
				paralellised_process = paralellised_process->next;
			}
			if(is_first_subprocess){
				if(((process_finish_time - current_process->process->allocate_time)/(float)current_process->process->original_run_time)>max_time_overhead){
					max_time_overhead = ((process_finish_time - current_process->process->allocate_time)/(float)current_process->process->original_run_time);
				}
			}
		}
		else{
			if(((current_process->process->finished_time - current_process->process->allocate_time)/(float)current_process->process->full_run_time)>max_time_overhead){
				max_time_overhead = ((current_process->process->finished_time - current_process->process->allocate_time)/(float)current_process->process->full_run_time);
			}
		}
		current_process=current_process->next;
	}

	return max_time_overhead;

}