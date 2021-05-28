/*
 * processor.h
 *
 *  Created on: 19 Mar 2021
 *      Author: USER
 */

#ifndef PROCESSOR_H_
#define PROCESSOR_H_

#include "linked_list.h"
typedef struct processor Processor;
struct processor{
	Process* current_process;
	Linked_list* process_queue;
	int already_allocated;
	Process* last_process;
	int is_last_process;
	int id;
};

Processor* new_processor();
int total_processing_time_left(Processor* processesor);
Linked_list* processor_execute_current_second(Processor* processors[], Linked_list* finished_processes, int number_of_processors, int current_time);
Processor* get_processor_with_shortest_total_run_time(Processor* processors[],  int number_of_processors);
int get_total_run_time(Processor* processor);
Process* get_process_with_shortest_run_time_remaining(Processor* processor);

#endif /* PROCESSOR_H_ */
