/*
 * task.h
 *
 *  Created on: Nov 17, 2022
 *      Author: ADMIN
 */

#ifndef INC_TASK_H_
#define INC_TASK_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	void (* pTask)(void);

	// Delay (ticks) until the function will (next) be run
	uint32_t Delay;

	// Interval (ticks) between subsequent runs
	uint32_t Period;

	// Incremented (by scheduler) when task i s due to execute
	uint8_t RunMe;

	uint32_t Task_ID;
} Task;

typedef struct TaskNode{
	Task task;

	struct TaskNode * next;
} TaskNode;

#endif /* INC_TASK_H_ */
