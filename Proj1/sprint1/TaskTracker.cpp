#include "TaskTracker.h"

#pragma once

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

// Перечислимый тип для статуса задачи
enum class TaskStatus {
	NEW,          // новая
	IN_PROGRESS,  // в разработке
	TESTING,      // на тестировании
	DONE          // завершена
};

// Объявляем тип-синоним для map<TaskStatus, int>,
// позволяющего хранить количество задач каждого статуса
using TasksInfo = map<TaskStatus, int>;

class TeamTasks {
	map<string, TasksInfo> tasks;

public:
	// Получить статистику по статусам задач конкретного разработчика
	const TasksInfo GetPersonTasksInfo(const string& person) const
	{
		auto tasks_ = tasks.at(person);
		return tasks_;
	}

	// Добавить новую задачу (в статусе NEW) для конкретного разработчитка
	void AddNewTask(const string& person)
	{
		tasks[person][TaskStatus::NEW]++;
	}

	bool PromoteTask(const string& person, TasksInfo& mutations_tasks)
	{
		int countNew = tasks[person][TaskStatus::NEW];
		int countInProgress = tasks[person][TaskStatus::IN_PROGRESS];
		int countTesting = tasks[person][TaskStatus::TESTING];
		int countDone = tasks[person][TaskStatus::DONE];
		if (countNew != 0)
		{
			tasks[person][TaskStatus::NEW]--;
			tasks[person][TaskStatus::IN_PROGRESS]++;
			mutations_tasks[TaskStatus::IN_PROGRESS]++;
			return true;
		}
		else if (countInProgress != 0)
		{
			tasks[person][TaskStatus::IN_PROGRESS]--;
			tasks[person][TaskStatus::TESTING]++;
			mutations_tasks[TaskStatus::TESTING]++;
			return true;
		}
		else if (countTesting != 0)
		{
			tasks[person][TaskStatus::TESTING]--;
			tasks[person][TaskStatus::DONE]++;
			mutations_tasks[TaskStatus::DONE]++;
			return true;
		}
		else
		{
			return false;
		}
	}

	// Обновить статусы по данному количеству задач конкретного разработчика,
	// подробности см. ниже
	tuple<TasksInfo, TasksInfo> PerformPersonTasks(
		const string& person, int task_count)
	{
		vector<pair<TaskStatus, int>> vec;
		auto tasks_before = tasks[person];

		int counter = 0;
		TasksInfo updated_tasks;
		for (; counter < task_count;)
		{
			if (PromoteTask(person, updated_tasks))
				counter++;
			else break;
		}
		auto tasks_after = tasks[person];
		TasksInfo untouched_tasks;
		for (int i = 0; i < 4; i++)
		{
			auto status = static_cast<TaskStatus>(i);
			untouched_tasks[status]= tasks_after[status] - updated_tasks[status];
		}
		return tuple(updated_tasks, untouched_tasks);
	}
};

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
	cout << tasks_info[TaskStatus::NEW] << " new tasks"s <<
		", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s <<
		", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s <<
		", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

void TaskTracker()
{
	TeamTasks tasks;
	tasks.AddNewTask("Ilia"s);
	for (int i = 0; i < 3; ++i) {
		tasks.AddNewTask("Ivan"s);
	}
	cout << "Ilia's tasks: "s;
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
	cout << "Ivan's tasks: "s;
	PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

	TasksInfo updated_tasks, untouched_tasks;

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);

	tie(updated_tasks, untouched_tasks) =
		tasks.PerformPersonTasks("Ivan"s, 2);
	cout << "Updated Ivan's tasks: "s;
	PrintTasksInfo(updated_tasks);
	cout << "Untouched Ivan's tasks: "s;
	PrintTasksInfo(untouched_tasks);

}