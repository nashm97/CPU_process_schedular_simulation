/*
 * allocate.h
 *
 *  Created on: 19 Mar 2021
 *      Author: USER
 */

#ifndef ALLOCATE_H_
#define ALLOCATE_H_
#include "linked_list.h"
#include "processor.h"

Linked_list* create_processes_from_input(char* file_name, int number_of_processors, int restriction_on_number_of_sub_porcesses);
Linked_list* sort_processes_by_id(Linked_list* processes);
void allocate_processes_for_next_second(Processor* processors[], Linked_list* processes,  int number_of_processors);
void print_results(Linked_list* finished_processes, int total_run_time);
int calculate_turnaround_time(Linked_list* finished_processes);
float calculate_max_time_overhead(Linked_list* finished_processes);
float calculate_average_time_overhead(Linked_list* finished_processes);



#endif /* ALLOCATE_H_ */
