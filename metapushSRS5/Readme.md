# metaRTC 5.0 版本Linux编译教程和推流demo

## 编译环境：
1. 测试编译系统： Ubuntu20.04 和 jetson NX Ubuntu18.04

## 编译依赖：
```
1. sudo apt install libasound2-dev
2. sudo apt install libgl1-mesa-dev
3. 解压libwebsockets-4.3.1 编译安装
4. 解压libyuv-master.zip 编译安装
5. git clone https://github.com/cisco/openh264.git 编译安装
6. 解压 opus-1.3.1.tar.gz 编译安装
7. 解压 speexdsp-SpeexDSP-1.2.0.tar.gz 编译安装
8. sudo apt-get install libusrsctp-dev    // ubuntu18.04 需要从github 下载源码编译安装 git clone https://github.com/sctplab/usrsctp.git
9. sudo apt-get install libmbedtls-dev
10. sudo apt-get install libsrtp2-dev
11. sudo apt-get install openssl
```
注意：需要手动解压编译安装的库在 thirdparty/metartc5.thirdparty.src.7z

## 编译libmetartc5.a：
进入到libmetartc5目录下，根据系统架构运行对应的编译脚本，X86运行cmake_x64.sh，jetson NX 运行cmake_arm.sh

## 编译libmetartccore5.a：
进入到libmetartc5目录下，根据系统架构运行对应的编译脚本，X86运行cmake_x64.sh，jetson NX 运行cmake_arm.sh

注意：编译libmetartccore5 库时需要修改 include/yang_config.h 文件如下：
```
#if _WIN32
#define Yang_Enable_Openssl 1
#else
#define Yang_Enable_Openssl 1
#endif
```
在Ubuntu 中启用OpenSSL 然后在CMakeLists 连接库时增加 -lssl 和  -lcrypto 链接

## 编译推流demo：
1. 将上面编译的 libmetartc5.a 和 libmetartccore5.a 库复制到metapushSRS5 目录下，即可编译推流demo,流媒体服务器需要自行根据 SRS 和 ZLMediakit 官方教程搭建.
2. main.cpp 是测试推流本机摄像头到 SRS 的demo
3. metartc_zlm.cpp 是测试推流本机摄像头到 ZLMediakit 并开启 datachannel 通讯的demo
4. peerconnection.cpp 是测试读取本地视频文件，然后调用作者的API接口推流；但是推流后不能播放，还未调试成功