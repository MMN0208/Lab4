/*
 * sched.h
 *
 *  Created on: Nov 17, 2022
 *      Author: ADMIN
 */

#ifndef INC_SCHED_H_
#define INC_SCHED_H_

#include "task.h"

#define SYSTEM_DELAY 	10

typedef struct {
	TaskNode * head;
	TaskNode * cur;

	uint32_t size;
} TaskSLL;

void SCH_Init(void);
uint8_t SCH_Add_Task(void (* pFunction)(void), uint32_t DELAY, uint32_t PERIOD);
void SCH_Delete_Task(uint8_t TASK_ID);
void SCH_Dispatch_Tasks(void);
void SCH_Update(void);

#endif /* INC_SCHED_H_ */
