#pragma once

#include "./virfile.hpp"
#include <string>
#include <vector>

class Command {
public:
    static void exit();
    static void help();
    static void pwd(VFolder *workFolder);
    static void ls(VFolder *workFolder);
    static VFolder* cd(VFolder *workFolder, std::string name);
    static bool mkdir(VFolder *workFolder, VFolder *subFolder);
    static bool rmdir(VFolder *workFolder, std::string name);
    static bool create(VFolder *workFolder, VFile *subFile);
    static bool rm(VFolder *workFolder, std::string name);
    static VFile *open(VFolder *workFolder, std::string name);
    static bool cat(VFile *file);
    static bool write(VFile *file, std::string str);
    static void df(VFileSystem *system);
    static bool mv(VFileSystem *system, VIR *vir, std::string name);
};