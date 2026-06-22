#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include "Task.cpp"


class DataFile{
public:
    DataFile() = default;

    ~DataFile() {
        if(file.is_open()) {
            file.close();
        }
    }

    int append(const std::string& data) {
        file.open(file_path, std::ios::app);
        
        if(!file.is_open()) {
            std::cerr << "Error: Unable to open file for appending." << std::endl;
            return 1;
        }

        file << data << std::endl;
        file.close();
        return 0;
    }

    int getLastID() {
        file.open(file_path, std::ios::in);
        
        if(!file.is_open()) {
            std::cerr << "Error: Unable to open file for reading." << std::endl;
            return -1;
        }

        std::string line;
        int lastID = 0;

        while(std::getline(file, line)) {
            size_t idPos = line.find("\"id\":");
            if(idPos != std::string::npos) {
                size_t start = line.find_first_of("0123456789", idPos);
                size_t end = line.find_first_not_of("0123456789", start);
                int currentID = std::stoi(line.substr(start, end - start));
                if(currentID > lastID) {
                    lastID = currentID;
                }
            }
        }

        file.close();
        return lastID;
    }

    std::vector<Task> readAllTasks() {
        std::vector<Task> tasks;
        file.open(file_path, std::ios::in);
        
        if(!file.is_open()) {
            std::cerr << "Error: Unable to open file for reading." << std::endl;
            return tasks;
        }

        std::string line;
        while(std::getline(file, line)) {
            Task task = Task::stringToTask(line);
            tasks.push_back(task);
        }

        file.close();
        return tasks;
    }

    int deleteTask(int id) {
        std::vector<Task> tasks = readAllTasks();
        bool found = false;

        file.open(file_path, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return 1;
        }

        for (const auto& task : tasks) {
            if (task.getId() == id) {
                found = true;
                continue;
            }
            file << task.toJsonStringFormat() << "\n";
        }

        file.close();

        if (!found) {
            std::cerr << "Error: Task with id " << id << " not found." << std::endl;
            return 1;
        }
        return 0;
    }

    int updateTask(const Task& updatedTask) {
        std::vector<Task> tasks = readAllTasks();
        bool found = false;

        file.open(file_path, std::ios::out | std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return 1;
        }

        for (const auto& task : tasks) {
            if (task.getId() == updatedTask.getId()) {
                file << updatedTask.toJsonStringFormat() << "\n";
                found = true;
            } else {
                file << task.toJsonStringFormat() << "\n";
            }
        }

        file.close();

        if (!found) {
            std::cerr << "Error: Task with id " << updatedTask.getId() << " not found." << std::endl;
            return 1;
        }
        return 0;
    }

private:
    std::string file_path = "./data/db.json";
    std::fstream file;
};