/*
 * sched.c
 *
 *  Created on: Nov 17, 2022
 *      Author: ADMIN
 */


#include "sched.h"

TaskSLL SCH_tasks_G;

void SCH_Init(void) {
	SCH_tasks_G.head = NULL;
	SCH_tasks_G.size = 0;
}

uint8_t SCH_Add_Task(void (* pFunction)(void), uint32_t DELAY, uint32_t PERIOD) {
	TaskNode *node = (TaskNode *)malloc(sizeof(TaskNode));
	node->task.pTask = pFunction;
	node->task.Delay = DELAY;
	node->task.Period = PERIOD;
	if(SYSTEM_DELAY > 0) {
		node->task.Delay /= SYSTEM_DELAY;
		node->task.Period /= SYSTEM_DELAY;
	}
	node->task.RunMe = 0;
	node->task.Task_ID = SCH_tasks_G.size;

	node->next = SCH_tasks_G.head;
	SCH_tasks_G.head = node;
	SCH_tasks_G.size++;

	return node->task.Task_ID;
}

void SCH_Delete_Task(uint8_t TASK_ID) {
	if(TASK_ID < 0 || TASK_ID >= SCH_tasks_G.size) return;

	TaskNode * cur = SCH_tasks_G.head;

	if(TASK_ID == SCH_tasks_G.size - 1) {
		SCH_tasks_G.head = SCH_tasks_G.head->next;
		free(cur);
		SCH_tasks_G.size--;
	}
	else {
		while(TASK_ID < cur->next->task.Task_ID) {
			cur = cur->next;
		}

		if(cur->next->task.Task_ID == TASK_ID) {
			TaskNode * temp = cur->next;
			cur->next = temp->next;
			free(temp);
			SCH_tasks_G.size--;
		}
	}
}

void SCH_Dispatch_Tasks(void) {
	TaskNode * cur = SCH_tasks_G.head;
	while(cur) {
		if(cur->task.RunMe > 0) {
			(*(cur->task.pTask))();
			cur->task.RunMe--;

			if(cur->task.Period == 0) {
				SCH_Delete_Task(cur->task.Task_ID);
			}
		}
		cur = cur->next;
	}
}

void SCH_Update(void) {
	TaskNode * cur = SCH_tasks_G.head;
	while(cur) {
		if(cur->task.Delay == 0) {
			cur->task.RunMe++;
			if(cur->task.Period) {
				cur->task.Delay = cur->task.Period;
			}
		}
		else {
			cur->task.Delay--;
		}
		cur = cur->next;
	}
}