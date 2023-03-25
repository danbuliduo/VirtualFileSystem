#pragma once

#include<bits/stdc++.h>
#include"./utils.hpp"
class VFolder;
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

    VFolder* pareant() {
        return this->_parent;
    }
    VFolder *set_parent(VFolder *folder) {
        this->_parent = folder;
        return folder;
    }

    virtual bool mv_parent(VFolder *folder){
        return false;
    }
    virtual bool rname(std::string name) {
        return false;
    }
    void chmod(bool read, bool write) {
        this->_read = read;
        this->_write = write;
    }

    bool is_read() {
        return this->_read;
    }
    bool is_write() {
        return this->_write;
    }

    void printInfo() {
        std::string auto_size = ByteUtils::byte_string(size());
        printf(
            "|%-8s|%-16s|%-16s|%d|%d|  %s\n",
            _type.c_str(), _name.c_str(), auto_size.c_str(), _read, _write, _time.c_str()
        );
    }


    virtual long long size() {
        return 0;
    }

protected:
    void LMT() {
        time_t tt = time(NULL);
        tm *t = localtime(&tt);
        char chars[24];
        sprintf(
            chars, "%d-%02d-%02d %02d:%02d:%02d", 
            t->tm_year + 1900, t->tm_mon + 1, t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec
        );
        this->_time = std::string(chars);
    }


protected:
    VFolder *_parent = nullptr;
    std::string _name;
    std::string _type;
    std::string _time;
    bool _read;
    bool _write;
};

class VFolder : public VIR {
public:
    VFolder(std::string name) : VIR(name, "dir", true, true) {}

    bool contains(const std::string &name) {
        return context.count(name) > 0;
    }

    bool create(VIR *file) {
        if(_write && !contains(file->name())) {
            LMT();
            context[file->name()] = file;
            file->set_parent(this);
            return true;
        }
        return false;
    }

    bool delect(std::string name, bool gc = false) {
        if(_write && contains(name)) {
            LMT();
            if(gc && context[name] != nullptr) {
                delete context[name];
            }
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

    bool mv_parent(VFolder *folder) override {
        if(_write && !folder->contains(_name)) {
            this->_parent->delect(_name);
            folder->create(this);
            return true;
        }
        return false;
    }

    bool rname(std::string name) override {
        if (_write && !_parent->contains(name)) {
            this->_parent->delect(this->_name);
            this->_name = name;
            this->_parent->create(this);
            LMT();
            return true;
        }
        return false;
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
};
class VFile : public VIR {
public:
    VFile(std::string name) : VIR(name, "file" ,true, true) {}
    VFile(std::string name, std::string content) : VIR(name, "file", true, true) {
        this->_content = content;
    }

    std::string content() {
        return _read ? this->_content : nullptr;
    }

    bool write_content(std::string content, bool cover = true) {
        if(_write) {
            LMT();
            cover ? this->_content = content : this->_content += content;
            return true;
        }
        return false;
    }

    bool mv_parent(VFolder *folder) override {
        if(_write && !folder->contains(_name)) {
            this->_parent->delect(_name);
            folder->create(this);
            return true;
        }
        return false;
    }
    bool rname(std::string name) override {
        if (_write && !_parent->contains(name)) {
            this->_parent->delect(_name);
            this->_name = name;
            this->_parent->create(this);
            LMT();
            return true;
        }
        return false;
    }

    long long size() override {
        return this->_content.size();
    }

private:
    std::string _content;
};


class VFileSystem : public VFolder
{
public:
    VFileSystem(long long max_size) : VFolder("/")
    {
        this->_type = "sys";
        VFolder *etc = new VFolder("etc");
        VFolder *usr = new VFolder("usr");
        VFolder *home = new VFolder("home");
        VFolder *boot = new VFolder("boot");
        boot->create(new VFile("banner.txt", BOOT_BANNER_TXT));
        VFolder *root = new VFolder("root");
        root->create(new VFile("centos7.sh", CENTOS7_SH));
        root->create(new VFile("install.sh", INSTALL_SH));
        root->create(new VFolder(".cache"));
        this->create(etc);
        this->create(usr);
        this->create(boot);
        this->create(root);
        this->max_size = max_size;
        std::cout << BOOT_BANNER_TXT;
    }
    long long maxsize() {
        return this->max_size;
    }

private:
    long long max_size = 0;

    std::string BOOT_BANNER_TXT = R"(
+===================================================+
|           __     _                                |
|          / /    (_)____   __  __ _  __            |
|         / /    / // __ \ / / / /| |/_/            |
|        / /___ / // / / // /_/ /_>  <              |
|       /_____//_//_/ /_/ \__,_//_/|_|              |
|                                                   |
| :: Author HBMZ - (DM. MZC.)  Version: Alpha-0.0.1 |
+===================================================+
)";

    std::string CENTOS7_SH = R"(
#!/bin/bash
yum update -y

curl -sSO http://download.bt.cn/install/install_panel.sh && echo y|bash install_panel.sh

wget -O install.sh http://download.bt.cn/install/plugin/tencent/install.sh && sh install.sh install

sed -i '/os.system('rm\ -f\ \/www\/server\/panel\/data\/admin_path.pl')/c\ \ \ \ os.system('echo\ "\/tencentcloud"\ >\ \/www\/server\/panel\/data\/admin_path.pl')' /www/server/panel/tools.py
sed -i '/a_input\ =\ input('|-是否在首次开机自动按机器配置优化PHP\/MySQL配置?(y\/n\ default:\ y):\ ')/c\ \ \ \ \ \ \ \ a_input\ =\ 'y'' /www/server/panel/tools.py
sed -i '/^    p_input\ =\ input/c\ \ \ \ p_input\ =\ '2'' /www/server/panel/tools.py

btpython /www/server/panel/tools.py package

systemctl stop firewalld
systemctl disable firewalld
)";

    std::string INSTALL_SH = R"(
#!/bin/bash
PATH=/bin:/sbin:/usr/bin:/usr/sbin:/usr/local/bin:/usr/local/sbin:~/bin
export PATH
install_tmp='/tmp/bt_install.pl'

public_file=/www/server/panel/install/public.sh
if [ ! -f $public_file ];then
	wget -O $public_file http://download.bt.cn/install/public.sh -T 5;
fi
. $public_file

download_Url=$NODE_URL

Install_tencent()
{

	pip install tencentcloud-sdk-python
	btpip install tencentcloud-sdk-python

    echo -n 'tencent' > /www/server/panel/data/o.pl

    echo '正在安装腾讯云专享版...' > $install_tmp
    mkdir -p /www/server/panel/plugin/tencent
    wget -O /www/server/panel/plugin/tencent/tencent_main.py $download_Url/install/plugin/tencent/tencent_main.py -T 5

    mkdir -p /www/server/panel/BTPanel/static/other/js
    wget -O /www/server/panel/BTPanel/static/other/js/tencent.js $download_Url/install/plugin/tencent/js/tencent.js -T 5

    mkdir -p /www/server/panel/BTPanel/static/other/css
    wget -O /www/server/panel/BTPanel/static/other/css/tencent.css $download_Url/install/plugin/tencent/css/tencent.css -T 5

    echo '正在安装腾讯云专享版...' > $install_tmp

    sed -i "s/宝塔Linux面板/宝塔面板·腾讯云专享版/g" /www/server/panel/config/config.json
    sed -i "s/\"Linux面板\"/\"·腾讯云专享版\"/g" /www/server/panel/config/config.json

    sed -i "s/\"webssh\",\"linuxsys\"/\"txcdn\",\"cosfs\",\"dnspod\"/g" /www/server/panel/config/index.json

	mkdir -p /www/server/panel/plugin/tencent

	echo '正在安装腾讯云CDN插件...' > $install_tmp
	wget -O txcdn.sh http://download.bt.cn/install/plugin/txcdn/install.sh && sh txcdn.sh install

	echo '正在安装腾讯云COSFS插件...' > $install_tmp
	wget -O cosfs.sh http://download.bt.cn/install/plugin/cosfs/install.sh && sh cosfs.sh install

    echo '正在安装DNSPod插件...' > $install_tmp
	wget -O dnspod.sh http://download.bt.cn/install/plugin/dnspod/install.sh && sh dnspod.sh install


	echo '安装完成' > $install_tmp
}

Uninstall_tencent()
{
	rm -rf /www/server/panel/plugin/tencent
    rm -rf /www/server/panel/BTPanel/static/other/js/tencent.js
    rm -rf /www/server/panel/BTPanel/static/other/css/tencent.css
}


action=$1
if [ "${1}" == 'install' ];then
	Install_tencent
else
	Uninstall_tencent
fi

)";
};
