#pragma once
#include<string>

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