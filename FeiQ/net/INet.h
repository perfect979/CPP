#pragma once
#include<iostream>
#include <winsock2.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include "packDef.h"
#include <set>
/*#pragma comment(lib, "ws2_32.lib")*/  // 链接到Windows Socket库
using namespace std;

using namespace std;
class INetMediator;
class INet {
public:
	INet() {};
	virtual ~INet() = default;
	//初始化网络
	virtual bool InitNet() = 0;
	//关闭网络
	virtual void UninitNet() = 0;
	//发送数据
    virtual bool SendData(long lSendIP, char* buf, int nLen) = 0;

    //字符串类型：char* std::string QString
    // char*是基础类型，可以直接给std::string 和 QString复制
    //std::string调用c_str()函数，可以转换为char*

    //long类型ip地址转换为string类型ip地址
    static std::string GetIPString(long ip){
        SOCKADDR_IN sockAddr;
        sockAddr.sin_addr.S_un.S_addr = ip;
        return inet_ntoa(sockAddr.sin_addr);
    }
    //获取本地IP地址列表
    static std::set<long> GetValidIpList(){
        //1、获取主机名字
        char name[100] = "";
        gethostname(name, sizeof(name));
        //2、获取IP地址列表
        struct hostent* remoteHost = gethostbyname(name);
        int i = 0;
        std::set<long> setIp;
        while (remoteHost->h_addr_list[i] != 0) {
            setIp.insert(*(u_long *) remoteHost->h_addr_list[i++]);
        }
        return setIp;
    }
protected:
	//接收数据
	virtual void RecvData() = 0;
	INetMediator* m_pMediator;
};
