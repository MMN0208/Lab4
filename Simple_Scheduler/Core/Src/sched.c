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
	SCH_tasks_G.cur = NULL;
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

	uint8_t index = 0;
	if(SCH_tasks_G.size == 0) {
		node->next = SCH_tasks_G.head;
		SCH_tasks_G.head = node;
	}
	else {
		TaskNode * cur = SCH_tasks_G.head;
		TaskNode * prev = NULL;
		while(cur && node->task.Delay > cur->task.Delay) {
			node->task.Delay -= cur->task.Delay;
			prev = cur;
			cur = cur->next;
			index++;
		}
		node->next = cur;
		prev->next = node;
		if(cur) {
			cur->task.Delay -= node->task.Delay;
		}
	}

	SCH_tasks_G.size++;

	return index;
}

void SCH_Delete_Task(uint8_t POSITION) {
	if(POSITION < 0 || POSITION >= SCH_tasks_G.size) return;

	TaskNode * cur = SCH_tasks_G.head;

	if(POSITION == 0) {
		SCH_tasks_G.head = SCH_tasks_G.head->next;
		free(cur);
	}
	else {
		uint8_t index = 0;
		while(index < POSITION) {
			cur = cur->next;
			index += 1;
		}
		TaskNode * temp = cur->next;
		cur->next = temp->next;
		free(temp);
	}
	SCH_tasks_G.size--;
}

void SCH_Dispatch_Tasks(void) {
	TaskNode * cur = SCH_tasks_G.head;
	uint8_t index = 0;
	uint8_t remove_task = 0;
	while(cur) {
		if(cur->task.RunMe > 0) {
			(*(cur->task.pTask))();
			cur->task.RunMe--;
			remove_task = 1;

			if(cur->task.Period > 0) {
				SCH_Add_Task(cur->task.pTask, cur->task.Delay, cur->task.Period);
			}
		}
		cur = cur->next;
		if(remove_task) {
			SCH_Delete_Task(index);
		}
		index += 1;
	}
}

void SCH_Update(void) {
	if(!SCH_tasks_G.cur) {
		SCH_tasks_G.cur = SCH_tasks_G.head;
	}
	if(SCH_tasks_G.cur->task.Delay == 0) {
		SCH_tasks_G.cur->task.RunMe += 1;

		if(SCH_tasks_G.cur->task.Period > 0) {
			SCH_tasks_G.cur->task.Delay = SCH_tasks_G.cur->task.Period;
		}

		SCH_tasks_G.cur = SCH_tasks_G.cur->next;
	}
	else {
		SCH_tasks_G.cur->task.Delay--;
	}
}
