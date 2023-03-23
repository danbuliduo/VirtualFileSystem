#ifdef _WIN32
#include <direct.h>
#elif __APPLE__ || __linux__
#include <unistd.h>
#endif
#include <iostream>
#include <algorithm>
#include "./command.h"
#include "./utils.hpp"
#include "./virfile.hpp"

std::string CENTOS_7_SH = R"(
#!/bin/bash

curl -sSO http://download.bt.cn/install/install_panel.sh && echo y|bash install_panel.sh

wget -O install.sh http://download.bt.cn/install/plugin/tencent/install.sh && sh install.sh install

sed -i /os.system('rm\ -f\ \/www\/server\/panel\/data\/admin_path.pl')/c\ \ \ \ os.system('echo\ "\/tencentcloud"\ >\ \/www\/server\/panel\/data\/admin_path.pl') /www/server/panel/tools.py

systemctl stop firewalld
systemctl disable firewalld
)";

class Application
{
public:
    static void run(VFolder *folder)
    {
        BannerUtils::loadBanner();
        VFolder *workfolder = folder;
        while (true) {
            std::string instr;
            std::cout << "[root@Admin " << workfolder->name() << "]# ";
            std::cin >> instr;
            if (instr == "help") {
                Command::help();
            } else if(instr == "ls") {
                Command::ls(workfolder);
            } else if (instr == "pwd") {
                Command::pwd(workfolder);
            } else if(instr == "cd") {
                std::cin >> instr;
                workfolder = Command::cd(workfolder, instr);
            } else if (instr == "mkdir") {
                std::cin >> instr;
                Command::mkdir(workfolder, new VFolder(instr));
            } else if (instr == "rmdir") {
                std::cin >> instr;
                Command::rmdir(workfolder, instr);
            } else if (instr == "create") {
                std::cin >> instr;
                Command::create(workfolder, new VFile(instr));
            } else if (instr == "rm") {
                std::cin >> instr;
                Command::rm(workfolder, instr);
            } else if (instr == "exit") {
                std::cout << "INFO: Are you sure you want to exit the system? (Y/N)" << std::endl;
                std::cout << "[root@Admin " << workfolder->name() << "]# ";
                std::cin >> instr;
                if (instr == "y" || instr == "Y") {
                    Command::exit();
                }
            } else {
                Command::help();
            }
        }
    };
};

int main(int argc, char *argv[]) {
    VFolder *system = new VFolder("/");
    VFolder *root = new VFolder("root");
    root->create(new VFile("centos7.sh", CENTOS_7_SH));
    root->create(new VFile("install.sh", "#INSTALL"));
    root->create(new VFolder("home"));
    system->create(root);
    system->create(new VFolder("boot"));
    system->create(new VFolder("etc"));
    system->create(new VFolder("usr"));
    system->create(new VFolder("sys"));
    Application::run(root);
}