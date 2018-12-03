#include <iostream>
#include <tuple>
#include <map>
#include <vector>
#include <sstream>

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
public:
  // Получить статистику по статусам задач конкретного разработчика
  const TasksInfo& GetPersonTasksInfo(const string& person) const;

  // Добавить новую задачу (в статусе NEW) для конкретного разработчитка
  void AddNewTask(const string& person);

  // Обновить статусы по данному количеству задач конкретного разработчика,
  // подробности см. ниже
  tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string& person, int task_count);

private:
  map<string, TasksInfo> PersonsTasks;
};

const TasksInfo& TeamTasks::GetPersonTasksInfo(const string& person) const
{
    return PersonsTasks.at(person);
}


void TeamTasks::AddNewTask(const string& person)
{
    if (PersonsTasks[person].count(TaskStatus::NEW) == 0) PersonsTasks[person][TaskStatus::NEW] = 0;
    if (PersonsTasks[person].count(TaskStatus::IN_PROGRESS) == 0) PersonsTasks[person][TaskStatus::IN_PROGRESS] = 0;
    if (PersonsTasks[person].count(TaskStatus::TESTING) == 0) PersonsTasks[person][TaskStatus::TESTING] = 0;
    if (PersonsTasks[person].count(TaskStatus::DONE) == 0) PersonsTasks[person][TaskStatus::DONE] = 0;
    ++PersonsTasks[person][TaskStatus::NEW];
}

tuple<TasksInfo, TasksInfo> TeamTasks::PerformPersonTasks(const string& person, int task_count)
{
    TasksInfo updated_tasks, untouched_tasks;
    TasksInfo updatedPersonsTasks = PersonsTasks[person];
    for (auto it = PersonsTasks[person].begin(); it != PersonsTasks[person].end(); ++it)
    {
        switch (it->first) {
            case TaskStatus::NEW :
                if (it->second >= task_count)
                {
                    updatedPersonsTasks[TaskStatus::NEW] -= task_count;
                    updatedPersonsTasks[TaskStatus::IN_PROGRESS] += task_count;
                    if (task_count != 0) updated_tasks[TaskStatus::IN_PROGRESS] += task_count;
                    if (it->second - task_count != 0) untouched_tasks[TaskStatus::NEW] += it->second - task_count;
                    task_count = 0;
                } else {
                    updatedPersonsTasks[TaskStatus::IN_PROGRESS] += it->second;
                    if (it->second != 0) updated_tasks[TaskStatus::IN_PROGRESS] += it->second;
                    updatedPersonsTasks[TaskStatus::NEW] -= it->second;
                    task_count -= it->second;
                }
                break;
            case TaskStatus::IN_PROGRESS :
                if (it->second >= task_count)
                {
                    updatedPersonsTasks[TaskStatus::IN_PROGRESS] -= task_count;
                    updatedPersonsTasks[TaskStatus::TESTING] += task_count;
                    if (task_count != 0) updated_tasks[TaskStatus::TESTING] = task_count;
                    if (it->second - task_count != 0) untouched_tasks[TaskStatus::IN_PROGRESS] = it->second - task_count;
                    task_count = 0;
                } else {
                    updatedPersonsTasks[TaskStatus::TESTING] += it->second;
                    if (it->second != 0) updated_tasks[TaskStatus::TESTING] = it->second;
                    updatedPersonsTasks[TaskStatus::IN_PROGRESS] -= it->second;
                    task_count -= it->second;
                }
                break;
            case TaskStatus::TESTING :
                if (it->second >= task_count)
                {
                    updatedPersonsTasks[TaskStatus::TESTING] -= task_count;
                    updatedPersonsTasks[TaskStatus::DONE] += task_count;
                    if (task_count != 0) updated_tasks[TaskStatus::DONE] = task_count;
                    if (it->second - task_count != 0) untouched_tasks[TaskStatus::TESTING] = it->second - task_count;
                    task_count = 0;
                } else {
                    updatedPersonsTasks[TaskStatus::DONE] += it->second;
                    if (it->second != 0) updated_tasks[TaskStatus::DONE] = it->second;
                    updatedPersonsTasks[TaskStatus::TESTING] -= it->second;
                    task_count -= it->second;
                }
                break;
            case TaskStatus::DONE :
                break;
        }
    }
    if (updatedPersonsTasks.count(TaskStatus::NEW) == 1)
    {
        if (updatedPersonsTasks[TaskStatus::NEW] == 0) updatedPersonsTasks.erase(TaskStatus::NEW);
    }
    if (updatedPersonsTasks.count(TaskStatus::IN_PROGRESS) == 1)
    {
        if (updatedPersonsTasks[TaskStatus::IN_PROGRESS] == 0) updatedPersonsTasks.erase(TaskStatus::IN_PROGRESS);
    }
    if (updatedPersonsTasks.count(TaskStatus::TESTING) == 1)
    {
        if (updatedPersonsTasks[TaskStatus::TESTING] == 0) updatedPersonsTasks.erase(TaskStatus::TESTING);
    }
    if (updatedPersonsTasks.count(TaskStatus::DONE) == 1)
    {
        if (updatedPersonsTasks[TaskStatus::DONE] == 0) updatedPersonsTasks.erase(TaskStatus::DONE);
    }
    PersonsTasks[person] = updatedPersonsTasks;
    return make_tuple(updated_tasks, untouched_tasks);
}

// Принимаем словарь по значению, чтобы иметь возможность
// обращаться к отсутствующим ключам с помощью [] и получать 0,
// не меняя при этом исходный словарь
void PrintTasksInfo(TasksInfo tasks_info) {
  cout << tasks_info[TaskStatus::NEW] << " new tasks" <<
      ", " << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress" <<
      ", " << tasks_info[TaskStatus::TESTING] << " tasks are being tested" <<
      ", " << tasks_info[TaskStatus::DONE] << " tasks are done" << endl;
}

template <typename Collection>
string Join(const Collection& c, const string& d) {
    stringstream ss;
    bool isFirst = true;
    for (const auto& i : c) {
        if(!isFirst) {
            ss << d;
        }
        isFirst = false;
        ss << i;
    }
    return ss.str();
}
ostream& operator << (ostream& out, const TaskStatus& t)
{
    switch (t) {
        case(TaskStatus::NEW):  out << "NEW";
                                break;
        case(TaskStatus::IN_PROGRESS):  out << "IN PROGRESS";
                                break;
        case(TaskStatus::TESTING):  out << "TESTING";
                                break;
        case(TaskStatus::DONE):  out << "DONE";
                                break;
    }
    return out;
}
template <typename F, typename S>
ostream& operator <<(ostream& out, const pair<F,S>& p) {
    return out << p.first << ": " << p.second;
}
template <typename K, typename V>
ostream& operator <<(ostream& out, const map<K,V>& m){
    return out << '{' << Join(m, ", ") << '}';
}
template <typename T>
ostream& operator <<(ostream& out, const vector<T>& v) {
   return out << '['  << Join(v, ", ") << ']';
}

int main() {
  TeamTasks tasks;
  tasks.AddNewTask("Ilia");
  for (int i = 0; i < 3; ++i) {
    tasks.AddNewTask("Ivan");
  }
  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

  TasksInfo updated_tasks, untouched_tasks;

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Ivan", 2);
  cout << "Updated Ivan's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Ivan's tasks: ";
  PrintTasksInfo(untouched_tasks);

  cout << "Ilia's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"));
  cout << "Ivan's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"));

  cout << endl;
  cout << endl;

  for (int i = 0; i < 5; ++i) {
    tasks.AddNewTask("Alice");
  }

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Alice", 7);
  cout << "Updated Alice's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Alice's tasks: ";
  PrintTasksInfo(untouched_tasks);

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Alice", 5);
  cout << "Updated Alice's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Alice's tasks: ";
  PrintTasksInfo(untouched_tasks);

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Alice", 1);
  cout << "Updated Alice's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Alice's tasks: ";
  PrintTasksInfo(untouched_tasks);

  for (int i = 0; i < 5; ++i) {
    tasks.AddNewTask("Alice");
  }

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Alice", 2);
  cout << "Updated Alice's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Alice's tasks: ";
  PrintTasksInfo(untouched_tasks);

  cout << "Alice's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));

  tie(updated_tasks, untouched_tasks) =
      tasks.PerformPersonTasks("Alice", 4);
  cout << "Updated Alice's tasks: ";
  PrintTasksInfo(updated_tasks);
  cout << "Untouched Alice's tasks: ";
  PrintTasksInfo(untouched_tasks);

  cout << "Alice's tasks: ";
  PrintTasksInfo(tasks.GetPersonTasksInfo("Alice"));

  while(cin) {
      string command, person;
      int num;
      cin >> command >> person;
      if (command == "AddNewTasks") {
          cin >> num;
          while (num--)
              tasks.AddNewTask(person);
          cout << "[]" << endl;
      } else if (command == "PerformPersonTasks") {
          cin >> num;
          TasksInfo updated_tasks, untouched_tasks;
          tie(updated_tasks, untouched_tasks) =
                  tasks.PerformPersonTasks(person, num);
          cout << vector<TasksInfo>{updated_tasks, untouched_tasks } << endl;
      } else if (command == "GetPersonTasksInfo") {
          cout << tasks.GetPersonTasksInfo(person) << endl;
      } else if (command.size() && command[0] != '#'){
          throw invalid_argument("wrong command: " + command);
      }
  }

  return 0;
}

