#include <iostream>
#include <vector>
#include "Task.cpp"
#include "Data.cpp"

class CliController {
public:
    int handleCmd(int argc, char* argv[]){
        preprocessCmd(argc, argv);

        if(cmd == "add") {
            std::cout << "Adding task with description: " << (args.empty() ? "" : args[0]) << std::endl;
            addTask(args.empty() ? "" : args[0]);
        }
        else if (cmd == "list") {
            if (args.empty()) {
                listTasks(-1);
            } else if (args[0] == "done") {
                listTasks(STATUS_DONE);
            } else if (args[0] == "todo") {
                listTasks(STATUS_TODO);
            } else if (args[0] == "in-progress") {
                listTasks(STATUS_IN_PROGRESS);
            } else {
                std::cerr << "Usage: list [done|todo|in-progress]" << std::endl;
            }
        }
        else if (cmd == "delete") {
            if (args.empty()) {
                std::cerr << "Usage: delete <id>" << std::endl;
            } else {
                deleteTask(std::stoi(args[0]));
            }
        }  
        else if (cmd == "update") {
            if (args.size() < 2) {
                std::cerr << "Usage: update <id> <new description>" << std::endl;
            } else {
                updateTask(std::stoi(args[0]), args[1]);
            }
        }
        else {
            std::cout << "Error: Not find this command. You need to choose one of existing commands." << std::endl;
        }

        return 0;
    }

    void addTask(const std::string& description) {
        int newID = dataFile.getLastID() + 1;
        Task newTask(newID, description);
        
        if(dataFile.append(newTask.toJsonStringFormat()) != 0) {
            std::cerr << "Error: Failed to add task." << std::endl;
        }else {
            std::cout << "Task added: " << newTask.toJsonStringFormat() << std::endl;
        }
    }

    void listTasks(int statusFilter) {
        std::vector<Task> tasks = dataFile.readAllTasks();

        // Print a header describing what is being listed
        if (statusFilter == STATUS_DONE)           std::cout << "=== Done tasks ===" << std::endl;
        else if (statusFilter == STATUS_TODO)      std::cout << "=== Todo tasks ===" << std::endl;
        else if (statusFilter == STATUS_IN_PROGRESS) std::cout << "=== In-progress tasks ===" << std::endl;
        else                                       std::cout << "=== All tasks ===" << std::endl;

        int count = 0;
        for (const auto& task : tasks) {
            if (statusFilter == -1 || task.getStatus() == statusFilter) {
                std::cout << task.toJsonStringFormat() << std::endl;
                ++count;
            }
        }

        if (count == 0) {
            std::cout << "(no tasks)" << std::endl;
        }
    }

    void deleteTask(int id) {
        if (dataFile.deleteTask(id) == 0) {
            std::cout << "Task " << id << " deleted successfully." << std::endl;
        } else {
            std::cerr << "Error: Failed to delete task " << id << "." << std::endl;
        }
    }

    void updateTask(int id, const std::string& newDescription) {
        std::vector<Task> tasks = dataFile.readAllTasks();
        for (auto& task : tasks) {
            if (task.getId() == id) {
                task.setDescription(newDescription);
                if (dataFile.updateTask(task) == 0) {
                    std::cout << "Task " << id << " updated: " << task.toJsonStringFormat() << std::endl;
                } else {
                    std::cerr << "Error: Failed to update task " << id << "." << std::endl;
                }
                return;
            }
        }
        std::cerr << "Error: Task " << id << " not found." << std::endl;
    }

private:
    std::string cmd;
    std::vector<std::string> args;
    DataFile dataFile;

    int preprocessCmd(int argc, char* argv[]) {
        if(argc < 2){
            printf("Please check input again");
            return 1;
        }

        cmd = argv[1];
        for(int i = 2; i < argc; i++) {
            args.push_back(argv[i]);
        }
        return 0;
    }
};
