/*
 * process.h
 *
 *  Created on: 19 Mar 2021
 *      Author: USER
 */

#ifndef PROCESS_H_
#define PROCESS_H_
#define HIGHEST_NUMBER 9999999


struct process {
	int allocate_time;
	int time_remaining;
	int full_run_time;
	int actual_start_times[100];
	int finished_time;
	float id;
	char is_parallelisable;
	int processor_index;
	int processes_remaining_at_finish_time;
	int time_remaining_at_start_time[100];
	int length_of_start_times_array;
	float base_id;
	int current_start_time_index_to_print;
	int time_elapsed;
	int original_run_time;
};

typedef struct process Process;

Process* new_process(int start_time, float id, int run_time, char parallelisable, float base_id, float original_run_time);
int compare_processes(Process* process1, Process* process2);

#endif /* PROCESS_H_ */
