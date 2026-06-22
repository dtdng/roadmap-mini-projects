#include<iostream>
#include"CliController.cpp"

int main(int argc, char* argv[]) {
    if(argc < 2) {
        std::cout << "Please check input again" << std::endl;
        return 1;
    }
    CliController controller = CliController();
    controller.handleCmd(argc, argv);
}