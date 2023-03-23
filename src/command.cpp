#include"./command.h"
#include "./utils.hpp"
#include <iostream>


void Command::ls(VFolder *workFolder) {
    if(workFolder->subfiles().size() == 0) {
        std::cout << "INFO: Folder is empty." << std::endl;
        return;
    }
    std::cout << "|type    |name            |size            |r|w|Latest Modification Time" << std::endl;
    for(auto &item : workFolder->subfiles()) {
        item->printInfo();
    }
}

bool Command::mkdir(VFolder *workFolder, VFolder *subFolder) {
    if(workFolder->create(subFolder)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "INFO: mkdir " << subFolder->name() << " Filed!" << std::endl;
    return false;
}

bool Command::rmdir(VFolder *workFolder, std::string name) {
    if(workFolder->delect(name)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "INFO: rmdir " << name << " Filed!" << std::endl;
    return false;
}

bool Command::create(VFolder *workFolder, VFile *subFile) {
    if(workFolder->create(subFile)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "INFO: create " << subFile->name() << " Filed!" << std::endl;
    return false;
}

bool Command::rm(VFolder *workFolder, std::string name) {
    if(workFolder->delect(name)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "INFO: rm " << name << " Filed!" << std::endl;
    return false;
}

VFolder* Command::cd(VFolder *workFolder, std::string name) {
    VFolder *folder = workFolder;
    if (name == "..") {
        if (workFolder->pareant() != nullptr) {
            workFolder = workFolder->pareant();
            std::cout << "INFO: OK." << std::endl;
            return workFolder;
        }
        std::cout << "WARN: The root directory does not have a parent directory!" << std::endl;
    } else {
        for(auto &item : StringUtils::split(name, "/")) {
            if (workFolder->contains(item) && workFolder->subfile(item)->type() == "dir") {
                workFolder = (VFolder *)workFolder->subfile(item);
            } else {
                std::cout << "ERROR: The directory does not exist!" << std::endl;
                return folder;
            }
        }
    }
    std::cout << "INFO: OK." << std::endl;
    return workFolder;
}

void Command::exit() {
    std::cout << "Bye ~ (^_^) : Looking forward to our next encounter!" << std::endl;
    std::exit(0);
}

void Command::help() {
    std::cout << "@System> Command Help:\n"
              << " 01.Exit system....................................(exit)\n"
              << " 02.Show help information..........................(help)\n"
              << " 03.Show current directory..........................(pwd)\n"
              << " 04.File list of current directory...................(ls)\n"
              << " 05.Enter the specified directory..........(cd <dirname>)\n"
              << " 06.Return last directory.........................(cd ..)\n"
              << " 07.Create a new directory..............(mkdir <dirname>\n"
              << " 08.Delete the directory................(rmdir <dirname>\n"
              << " 09.Create a new file....................(create <fname>)\n"
              << " 10.Open a file............................(open <fname>)\n"
              << " 11.Read the file...................................(cat)\n"
              << " 12.Write the file....................(write + <content>)\n"
              << " 13.Copy a file..............................(cp <fname>)\n"
              << " 14.Delete a file............................(rm <fname>)\n"
              << std::endl;
}

void Command::pwd(VFolder *workFolder) {
    VFolder *folder = workFolder;
    std::vector<std::string> folders;
    while (folder->pareant() != nullptr) {
        folders.push_back(folder->name());
        folder = folder->pareant();
    }
    std::cout << "/";
    for(int i = folders.size() -2; i > 0; --i) {
        std::cout << folders.at(i) << "/";
    }
    std::cout << folders.size() << std::endl;
}