#pragma once
#include<bits/stdc++.h>

class VIR {
public:
    VIR(std::string name, std::string type, bool read = true, bool write = true) {
        LMT();
        this->_name = name;
        this->_type = type;
        this->_read = read;
        this->_write = write;
    }
    std::string name() {
        return this->_name;
    }
    std::string type() {
        return this->_type;
    }
    std::string ts() {
        return this->_time;
    }

    bool is_read() {
        return this->_read;
    }
    bool is_write() {
        return this->_write;
    }
    void printInfo() {
        std::string auto_size;
        long double real_size = (long double)size();
        if(real_size < 1024 ) {
            auto_size = std::to_string(real_size) + "B";
        } else if(real_size >= 1024) {
            auto_size = std::to_string(real_size / 1024) + "KB";
        } else if(real_size >= 1024 * 1024) {
            auto_size = std::to_string(real_size / 1024 / 1024) + "MB";
        } else {
            auto_size = std::to_string(real_size / 1024 / 1024 /1024) + "GB";
        }
        printf("|%-8s|%-16s|%-16s|%d|%d|  %s\n", _type.c_str(), _name.c_str(), auto_size.c_str(), _read, _write, _time.c_str());
    }
    virtual long long size() {
        return 0;
    }
private:
    void LMT() {
        time_t tt = time(NULL);
        tm *t = localtime(&tt);
        char chars[24];
        sprintf(chars, "%d-%02d-%02d %02d:%02d:%02d", t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec);
        this->_time = std::string(chars);
    }
protected:
    std::string _name;
    std::string _type;
    std::string _time;
    bool _read;
    bool _write;
};

class VFile : public VIR {
public:
    VFile(std::string name) : VIR(name, "file" ,true, true) {}
    VFile(std::string name, std::string content) : VIR(name, "file", true, true) {
        this->_content = content;
    }
    std::string content() {
        return this->_content;
    }
    void write_content(std::string content, bool cover = true) {
        if(_write) {
            cover ? this->_content = content : this->_content += content;
            std::cout << "@System> " << _name << " write succeeded." << std::endl;
        } else {
            std::cout << "@System> " << _name << " insufficient write permissions." << std::endl;
        }
    }

    long long size() override {
        return this->_content.size();
    }

private:
    std::string _content;
};


class VFolder : public VIR {
public:
    VFolder(std::string name) : VIR(name, "dir", true, true) {}

    bool contains(const std::string &name) {
        return context.count(name) > 0;
    }

    bool create(VFile *file) {
        if(!contains(file->name())) {
            context[file->name()] = file;
            return true;
        }
        return false;
    }

    bool create(VFolder *folder) {
        if (!contains(folder->name())){
            context[folder->name()] = folder;
            folder->set_parent(this);
            return true;
        }
        return false;
    }

    bool delect(std::string name) {
        if(contains(name)) {
            context.erase(name);
            return true;
        }
        return false;
    }

    std::vector<VIR*> subfiles() {
        std::vector<VIR *> subfiles;
        for(auto &item: context) {
            subfiles.push_back(item.second);
        }
        return subfiles;
    }

    VIR* subfile(std::string name) {
        if (contains(name)) {
            return context[name];
        }
        return nullptr;
    }

    VFolder* pareant() {
        return this->_parent;
    }
    VFolder *set_parent(VFolder * folder) {
        this->_parent = folder;
        return folder;
    }

    long long size() override {
        long long _size = 0;
        for (auto &item : subfiles()) {
           _size += item->size();
        }
        return _size;
    }


private:
    std::map<std::string, VIR *> context;
    VFolder* _parent = nullptr;
};
