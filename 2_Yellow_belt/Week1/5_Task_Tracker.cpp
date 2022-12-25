#include <iostream>
#include <vector>
#include <map>
#include <tuple>
using namespace std;


class TeamTasks {
private:
	map<string, TasksInfo> person_to_TasksInfo;
public:

	const TasksInfo& GetPersonTasksInfo(const string& person) const {
		return person_to_TasksInfo.at(person);
	}

	void AddNewTask(const string& person) {
		++person_to_TasksInfo[person][TaskStatus::NEW];
	}

	tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count) {
		TasksInfo newTasks;
		TasksInfo oldTasks;

		if (person_to_TasksInfo.count(person)) {
			TasksInfo& PerTasks = person_to_TasksInfo.at(person);

			auto NEW = TaskStatus::NEW;
			auto IN_PROGRESS = TaskStatus::IN_PROGRESS;
			auto TESTING = TaskStatus::TESTING;
			auto DONE = TaskStatus::DONE;

			int t_new = 0, t_prog = 0, t_test = 0;
			if (PerTasks.count(NEW))
				t_new = PerTasks.at(NEW);
			if (PerTasks.count(IN_PROGRESS))
				t_prog = PerTasks.at(IN_PROGRESS);
			if (PerTasks.count(TESTING))
				t_test = PerTasks.at(TESTING);

			if (t_new != 0) {
				if (t_new <= task_count) {
					newTasks[IN_PROGRESS] = t_new;
					PerTasks[IN_PROGRESS] += t_new;
					PerTasks[NEW] -= t_new;
					task_count -= t_new;
				}
				else {
					if (task_count > 0) {
						newTasks[IN_PROGRESS] = task_count;
						PerTasks[IN_PROGRESS] += task_count;
						PerTasks[NEW] -= task_count;
					}
					oldTasks[NEW] = (t_new - task_count);
					task_count = 0;
				}
			}
			if (t_prog != 0) {
				if (t_prog <= task_count) {
					newTasks[TESTING] = t_prog;
					PerTasks[TESTING] += t_prog;
					PerTasks[IN_PROGRESS] -= t_prog;
					task_count -= t_prog;
				}
				else {
					if (task_count > 0) {
						newTasks[TESTING] = task_count;
						PerTasks[TESTING] += task_count;
						PerTasks[IN_PROGRESS] -= task_count;
					}
					oldTasks[IN_PROGRESS] = (t_prog - task_count);
					task_count = 0;
				}
			}
			if (t_test != 0) {
				if (t_test <= task_count) {
					newTasks[DONE] = t_test;
					PerTasks[DONE] += t_test;
					PerTasks[TESTING] -= t_test;
					task_count -= t_test;
				}
				else {
					if (task_count > 0) {
						newTasks[DONE] = task_count;
						PerTasks[DONE] += task_count;
						PerTasks[TESTING] -= task_count;
					}
					oldTasks[TESTING] = (t_test - task_count);
					task_count = 0;
				}
			}

			auto copyPerTasks = PerTasks;
			for (auto i : copyPerTasks)
				if (i.second == 0)
					PerTasks.erase(i.first);
		}

		return tie(newTasks, oldTasks);
	}

};