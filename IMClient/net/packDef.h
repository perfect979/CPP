#pragma once
#include<string.h>
#define _DEF_UDP_SERVER_PORT			(1234)
#define _DEF_NAME_SIZE					(100)
#define _DEF_CONTENT_SIZE				(1024)
#define _DEF_TCP_SERVER_PORT			(1234)
#define _DEF_TCP_SERVER_IP				("192.168.2.149")
#define _DEF_TCP_PROTOCOL_COUNT         (10)

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

/*TCP协议头*/

// 注册请求
#define _DEF_PROTOCOL_TCP_REGISTER_RQ		(_DEF_PROTOCOL_BASSE + 10)
// 注册回复
#define _DEF_PROTOCOL_TCP_REGISTER_RS		(_DEF_PROTOCOL_BASSE + 11)
// 登录请求
#define _DEF_PROTOCOL_TCP_LOOGIN_RQ			(_DEF_PROTOCOL_BASSE + 12)
// 登录回复
#define _DEF_PROTOCOL_TCP_LOOGIN_RS			(_DEF_PROTOCOL_BASSE + 13)
// 好友信息
#define _DEF_PROTOCOL_TCP_FRIEND_INFO		(_DEF_PROTOCOL_BASSE + 14)
// 聊天请求
#define _DEF_PROTOCOL_TCP_CHAT_RQ			(_DEF_PROTOCOL_BASSE + 15)
// 聊天回复
#define _DEF_PROTOCOL_TCP_CHAT_RS			(_DEF_PROTOCOL_BASSE + 16)
// 添加好友请求
#define _DEF_PROTOCOL_ADD_FRIEND_RQ			(_DEF_PROTOCOL_BASSE + 17)
//添加好友回复
#define _DEF_PROTOCOL_ADD_FRIEND_RS			(_DEF_PROTOCOL_BASSE + 18)
// 下线
#define _DEF_PROTOCOL_TCP_OFFLINE_RQ		(_DEF_PROTOCOL_BASSE + 19)


//请求结果宏定义
//注册结果
#define REGISTER_SUCCESS		(0)
#define NAME_IS_EXIST			(1)
#define TEL_IS_EXIST			(2)
// 登录结果
#define LOGIN_SUCCESS			(0)
#define PASSWORD_ERROR			(1)
#define USER_NOT_EXIT			(2)
//用户状态
#define STATUS_ONLINE			(0)
#define STATUS_OFFLINE			(1)
//聊天请求结果
#define	SEND_SUCCESS			(0)
#define SEND_FAIL				(1)
//添加好友请求结果
#define ADD_SUCCESS				(0)
#define NO_THIS_USER			(1)
#define USER_OFFLINE			(2)
#define USER_REFUSE				(3)



/*TCP请求结构体*/
typedef int PackType;


// 注册请求
typedef struct  STRU_REGISTER_RQ {
	STRU_REGISTER_RQ() :type(_DEF_PROTOCOL_TCP_REGISTER_RQ) {
		memset(name, 0, _DEF_NAME_SIZE);
		memset(password, 0, _DEF_NAME_SIZE);
		memset(tel, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	char name[_DEF_NAME_SIZE];
	char password[_DEF_NAME_SIZE];
	char tel[_DEF_NAME_SIZE];
}STRU_REGISTER_RQ;

// 注册回复:协议头、注册结果
typedef struct  STRU_REGISTER_RS {
	STRU_REGISTER_RS() :type(_DEF_PROTOCOL_TCP_REGISTER_RS), result(USER_NOT_EXIT) {}
	PackType type;
	int result;
}STRU_REGISTER_RS;

// 登录请求:协议头、电话号嘛、密码
typedef struct  STRU_LOGIN_RQ {
	STRU_LOGIN_RQ():type(_DEF_PROTOCOL_TCP_LOOGIN_RQ) {
		memset(password, 0, _DEF_NAME_SIZE);
		memset(tel, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	char password[_DEF_NAME_SIZE];
	char tel[_DEF_NAME_SIZE];
}STRU_LOGIN_RQ;

// 登录回复:协议头、注册结果、自己的ID
typedef struct  STRU_LOGIN_RS {
	STRU_LOGIN_RS() :type(_DEF_PROTOCOL_TCP_LOOGIN_RS), result(TEL_IS_EXIST), userId(0){}
	PackType type;
	int result;
	int userId;
}STRU_LOGIN_RS;

// 好友信息:协议头、用户ID、昵称、头像ID、在线状态、签名
typedef struct  STRU_FRIEND_INFO {
	STRU_FRIEND_INFO() :type(_DEF_PROTOCOL_TCP_FRIEND_INFO), userId(0),
	iconId(0), state(STATUS_OFFLINE)
	{
	memset(name, 0, _DEF_NAME_SIZE);
	memset(feeling, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	int userId;
	int iconId;
	int state;
	char name[_DEF_NAME_SIZE];
	char feeling[_DEF_NAME_SIZE];
}STRU_FRIEND_INFO;

// 聊天请求:协议头、好友ID、聊天内容、好友ID
typedef struct  STRU_TCP_CHAT_RQ {
	STRU_TCP_CHAT_RQ() :type(_DEF_PROTOCOL_TCP_CHAT_RQ), userId(0),
		friendId(0)
	{
		memset(content, 0, _DEF_CONTENT_SIZE);

	}
	PackType type;
	int userId;
	int friendId;
	char content[_DEF_CONTENT_SIZE];
}STRU_TCP_CHAT_RQ;

// 聊天回复:协议头】自己ID、好友ID、聊天请求结果
typedef struct  STRU_TCP_CHAT_RS {
	STRU_TCP_CHAT_RS() :type(_DEF_PROTOCOL_TCP_CHAT_RS),result(SEND_FAIL),friendId(0) {}
	PackType type;
	int userId;
	int friendId;
	int result;
}STRU_TCP_CHAT_RS;

// 添加好友请求:协议头、自己的ID、好友的昵称、自己的昵称
typedef struct  STRU_ADD_FRIEND_RQ {
	STRU_ADD_FRIEND_RQ() :type(_DEF_PROTOCOL_ADD_FRIEND_RQ),userId(0) {
		memset(userName, 0, _DEF_NAME_SIZE);
		memset(friendName, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	int userId;
	char userName[_DEF_NAME_SIZE];
	char friendName[_DEF_NAME_SIZE];
}STRU_ADD_FRIEND_RQ;

//添加好友回复:协议头、自己的ID、好友的ID、添加结果、好友的昵称
typedef struct  STRU_ADD_FRIEND_RS {
	STRU_ADD_FRIEND_RS() :type(_DEF_PROTOCOL_ADD_FRIEND_RS), userId(0),
	friendId(0), result(USER_REFUSE)
	{
		memset(friendName, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	int userId;
	int friendId;
	int result;
	char friendName[_DEF_NAME_SIZE];
}STRU_ADD_FRIEND_RS;

// 下线:协议头、自己的ID、	
typedef struct  STRU_TCP_OFFLINE_RQ {
	STRU_TCP_OFFLINE_RQ() :type(_DEF_PROTOCOL_TCP_OFFLINE_RQ), userId(0) {}
	PackType type;
	int userId;
}STRU_TCP_OFFLINE_RQ;
