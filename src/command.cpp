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
    if(workFolder->delect(name, true)) {
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
    if(workFolder->delect(name, true)) {
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
                if(!workFolder->is_read()) {
                    std::cout << "WRAN: Dir not readable." << std::endl;
                    return folder;
                }
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
    std::cout << "---------------------------------------------------------\n"
              << " 01.Exit system....................................(exit)\n"
              << " 02.Show help information..........................(help)\n"
              << " 03.Show current directory..........................(pwd)\n"
              << " 04.File list of current directory...................(ls)\n"
              << " 05.Enter the specified directory..........(cd <dirname>)\n"
              << " 06.Return last directory.........................(cd ..)\n"
              << " 07.Create a new directory..............(mkdir <dirname>)\n"
              << " 08.Delete the directory................(rmdir <dirname>)\n"
              << " 09.Create a new file....................(create <fname>)\n"
              << " 10.Open a file............................(open <fname>)\n"
              << " 11.Read the file...................................(cat)\n"
              << " 12.Write the file.................(write [-a] <content>)\n"
              << " 13.Move a file.............. .........(mv <file> <path>)\n"
              << " 14.Delete a file............................(rm <fname>)\n"
              << " 15.Modify permissions.........(chmod <file> [r|w|rw|--])\n"
              << " 16.Modify File Name.....................(rename <fname>)\n"
              << " 17.View Disk Usage..................................(df)\n"
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
    for(int i = folders.size() -1; i > 0; --i) {
        std::cout << folders.at(i) << "/";
    }
    std::cout << folders.at(0) << std::endl;
}

void Command::df(VFileSystem *system) {
    long double p = (((long double) system->size()) / ((long double) system->maxsize())) * 100.0;
    std::string s = ByteUtils::byte_string(system->size());
    std::string m = ByteUtils::byte_string(system->maxsize());
    long long _p = (long long) (p / 2);
    std::cout << "[";
    for(int i = 0; i < 50; i++) {
        std::cout << (i < _p ? "#" : "-");
    }
    std::cout << "]\n";
    std::cout << "Maximum Disk Memory: " << m << "\n"
              << "Disk Used Memory: " << s << "\n"
              << "Disk Memory Usage: " << p << "%" <<std::endl;
}

VFile *Command::open(VFolder *workFolder, std::string name) {
    if(workFolder->contains(name) && workFolder->subfile(name)->type() == "file") {
        std::cout << "INFO: OK." << std::endl;
        return (VFile*) workFolder->subfile(name);
    }
    std::cout << "WRAN: Open "<< name << " Filed!" << std::endl;
    return nullptr;
}

bool Command::cat(VFile *file) {
    if (file->is_read()) {
        file->printInfo();
        std::cout << ">>>\n" << file->content() << std::endl;
        return true;
    }
    std::cout << "INFO: File not readable." << std::endl;
    return false;
}

bool Command::write(VFile *file, std::string str, bool cover)
{
    if (file->is_write() && file->write_content(str, cover)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "INFO: File not writeable." << std::endl;
    return false;
}

bool Command::mv(VFileSystem *system, VIR *vir, std::string path) {
    VFolder* folder = system;
    for(auto &item : StringUtils::split(path, "/")) {
        if(item.size() > 0) {
            if (folder->contains(item) && folder->subfile(item)->type() == "dir") {
                folder = (VFolder *) folder->subfile(item);
            } else {
                std::cout << "ERROR: Move Filed!" << std::endl;
                return false;
            }
        }
    }
    if(vir->mv_parent(folder)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "WRAN: File already exist" << std::endl;
    return false;
}

bool Command::rename(VIR *vir, std::string newname) {
    if(vir->rname(newname)) {
        std::cout << "INFO: OK." << std::endl;
        return true;
    }
    std::cout << "WRAN: Rename Filed!" << std::endl;
    return false;
}

bool Command::chmod(VIR *vir, std::string prom) {
    if(prom == "r") {
        vir->chmod(true, false);
        return true;
    } else if(prom == "w") {
        vir->chmod(false, true);
        return true;
    } else if(prom == "rw") {
        vir->chmod(true, true);
        return true;
    } else if(prom == "--") {
        vir->chmod(false, false);
        return true;
    }
    return false;
}