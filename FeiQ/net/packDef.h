#pragma once
#define _DEF_UDP_SERVER_PORT			(1234)
#define _DEF_NAME_SIZE					(100)
#define _DEF_CONTENT_SIZE				(1024)

// 请求协议头
#define _DEF_PROTOCOL_BASSE				(1000)
//上线请求
#define _DEF_PROTOCOL_UDP_ONLINE_RQ		(_DEF_PROTOCOL_BASSE + 1)
//上线回复
#define _DEF_PROTOCOL_UDP_ONLINE_RS		(_DEF_PROTOCOL_BASSE + 2)
//下线请求
#define _DEF_PROTOCOL_UDP_OFFLINE_RQ	(_DEF_PROTOCOL_BASSE + 3)
//聊天请求
#define _DEF_PROTOCOL_UDP_CHAT_RQ		(_DEF_PROTOCOL_BASSE + 4)

//请求结构体
//上线请求:协议头、主机名、ip地址(recvfrom函数里已经有发送端ip地址了，这里不用带了)
struct  STRU_ONLINE{
	STRU_ONLINE() :nType(_DEF_PROTOCOL_UDP_ONLINE_RQ) {
		memset(name, 0, _DEF_NAME_SIZE);
	}
	int nType;
	char name[_DEF_NAME_SIZE];
};
//上线回复:协议头、主机名


//下线请求:协议头
struct STRU_OFFLINE_RQ {
	STRU_OFFLINE_RQ() : nType(_DEF_PROTOCOL_UDP_OFFLINE_RQ){
	}
	int nType;
};

//聊天请求:协议头、聊天内容
struct  STRU_CHAT_RQ {
	STRU_CHAT_RQ(): nType(_DEF_PROTOCOL_UDP_CHAT_RQ){
		memset(content, 0, _DEF_CONTENT_SIZE);
	}
	int nType;
	char content[_DEF_CONTENT_SIZE];
};