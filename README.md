# VirtualFileSystem

湖北民族大学-操作系统[课程设计]-虚拟文件系统

| Author HBMZ - (TengMing.) |

@完成日期: 2023-03-25

command.h command.cpp

    class Command - 命令行执行类

utils.hpp

    class BannerUtils - banner加载器

    class StringUtils - string工具类

virfile.hpp

    class VIR 抽象文件类

    class VFolder : public VIR - 虚拟文件夹

    class VFile : public VIR - 虚拟文件

    class VFileSystem : public VFolder - 虚拟文件系统
