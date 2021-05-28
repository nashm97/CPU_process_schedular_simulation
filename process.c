/*
 * process.c
 *
 *  Created on: 18 Mar 2021
 *      Author: USER
 */


#include "process.h"
#include <stdlib.h>

Process* new_process(int allocate_time, float id, int time_remaining, char parallelisable, float base_id, float original_run_time){
	Process* new_process = malloc(sizeof(Process));
	new_process->allocate_time = allocate_time;
	new_process->id = id;
	new_process->time_remaining = time_remaining;
	new_process->is_parallelisable = parallelisable;
	new_process->full_run_time = time_remaining;
	new_process->processes_remaining_at_finish_time = 0;
	new_process->length_of_start_times_array = 0;
	new_process->base_id = base_id;
	new_process->time_elapsed = 0;
	new_process->finished_time = -1;
	new_process->original_run_time = original_run_time;
	return new_process;
}

int compare_processes(Process* process1, Process* process2){
	if((process1->allocate_time == process2->allocate_time)&&(process1->id == process2->id)){
		return 1;
	}
	return 0;
}
