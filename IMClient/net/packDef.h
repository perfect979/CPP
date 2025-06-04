#pragma once
#include<string.h>
#define _DEF_UDP_SERVER_PORT			(1234)
#define _DEF_NAME_SIZE					(100)
#define _DEF_CONTENT_SIZE				(1024)
#define _DEF_TCP_SERVER_PORT			(1234)
#define _DEF_TCP_SERVER_IP				("192.168.2.149")
#define _DEF_TCP_PROTOCOL_COUNT         (10)

// ����Э��ͷ
#define _DEF_PROTOCOL_BASSE				(1000)
//��������
#define _DEF_PROTOCOL_UDP_ONLINE_RQ		(_DEF_PROTOCOL_BASSE + 1)
//���߻ظ�
#define _DEF_PROTOCOL_UDP_ONLINE_RS		(_DEF_PROTOCOL_BASSE + 2)
//��������
#define _DEF_PROTOCOL_UDP_OFFLINE_RQ	(_DEF_PROTOCOL_BASSE + 3)
//��������
#define _DEF_PROTOCOL_UDP_CHAT_RQ		(_DEF_PROTOCOL_BASSE + 4)

//����ṹ��
//��������:Э��ͷ����������ip��ַ(recvfrom�������Ѿ��з��Ͷ�ip��ַ�ˣ����ﲻ�ô���)
struct  STRU_ONLINE{
	STRU_ONLINE() :nType(_DEF_PROTOCOL_UDP_ONLINE_RQ) {
		memset(name, 0, _DEF_NAME_SIZE);
	}
	int nType;
	char name[_DEF_NAME_SIZE];
};
//���߻ظ�:Э��ͷ��������


//��������:Э��ͷ
struct STRU_OFFLINE_RQ {
	STRU_OFFLINE_RQ() : nType(_DEF_PROTOCOL_UDP_OFFLINE_RQ){
	}
	int nType;
};

//��������:Э��ͷ����������
struct  STRU_CHAT_RQ {
	STRU_CHAT_RQ(): nType(_DEF_PROTOCOL_UDP_CHAT_RQ){
		memset(content, 0, _DEF_CONTENT_SIZE);
	}
	int nType;
	char content[_DEF_CONTENT_SIZE];
};

/*TCPЭ��ͷ*/

// ע������
#define _DEF_PROTOCOL_TCP_REGISTER_RQ		(_DEF_PROTOCOL_BASSE + 10)
// ע��ظ�
#define _DEF_PROTOCOL_TCP_REGISTER_RS		(_DEF_PROTOCOL_BASSE + 11)
// ��¼����
#define _DEF_PROTOCOL_TCP_LOOGIN_RQ			(_DEF_PROTOCOL_BASSE + 12)
// ��¼�ظ�
#define _DEF_PROTOCOL_TCP_LOOGIN_RS			(_DEF_PROTOCOL_BASSE + 13)
// ������Ϣ
#define _DEF_PROTOCOL_TCP_FRIEND_INFO		(_DEF_PROTOCOL_BASSE + 14)
// ��������
#define _DEF_PROTOCOL_TCP_CHAT_RQ			(_DEF_PROTOCOL_BASSE + 15)
// ����ظ�
#define _DEF_PROTOCOL_TCP_CHAT_RS			(_DEF_PROTOCOL_BASSE + 16)
// ��Ӻ�������
#define _DEF_PROTOCOL_ADD_FRIEND_RQ			(_DEF_PROTOCOL_BASSE + 17)
//��Ӻ��ѻظ�
#define _DEF_PROTOCOL_ADD_FRIEND_RS			(_DEF_PROTOCOL_BASSE + 18)
// ����
#define _DEF_PROTOCOL_TCP_OFFLINE_RQ		(_DEF_PROTOCOL_BASSE + 19)


//�������궨��
//ע����
#define REGISTER_SUCCESS		(0)
#define NAME_IS_EXIST			(1)
#define TEL_IS_EXIST			(2)
// ��¼���
#define LOGIN_SUCCESS			(0)
#define PASSWORD_ERROR			(1)
#define USER_NOT_EXIT			(2)
//�û�״̬
#define STATUS_ONLINE			(0)
#define STATUS_OFFLINE			(1)
//����������
#define	SEND_SUCCESS			(0)
#define SEND_FAIL				(1)
//��Ӻ���������
#define ADD_SUCCESS				(0)
#define NO_THIS_USER			(1)
#define USER_OFFLINE			(2)
#define USER_REFUSE				(3)



/*TCP����ṹ��*/
typedef int PackType;


// ע������
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

// ע��ظ�:Э��ͷ��ע����
typedef struct  STRU_REGISTER_RS {
	STRU_REGISTER_RS() :type(_DEF_PROTOCOL_TCP_REGISTER_RS), result(USER_NOT_EXIT) {}
	PackType type;
	int result;
}STRU_REGISTER_RS;

// ��¼����:Э��ͷ���绰�������
typedef struct  STRU_LOGIN_RQ {
	STRU_LOGIN_RQ():type(_DEF_PROTOCOL_TCP_LOOGIN_RQ) {
		memset(password, 0, _DEF_NAME_SIZE);
		memset(tel, 0, _DEF_NAME_SIZE);
	}
	PackType type;
	char password[_DEF_NAME_SIZE];
	char tel[_DEF_NAME_SIZE];
}STRU_LOGIN_RQ;

// ��¼�ظ�:Э��ͷ��ע�������Լ���ID
typedef struct  STRU_LOGIN_RS {
	STRU_LOGIN_RS() :type(_DEF_PROTOCOL_TCP_LOOGIN_RS), result(TEL_IS_EXIST), userId(0){}
	PackType type;
	int result;
	int userId;
}STRU_LOGIN_RS;

// ������Ϣ:Э��ͷ���û�ID���ǳơ�ͷ��ID������״̬��ǩ��
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

// ��������:Э��ͷ������ID���������ݡ�����ID
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

// ����ظ�:Э��ͷ���Լ�ID������ID������������
typedef struct  STRU_TCP_CHAT_RS {
	STRU_TCP_CHAT_RS() :type(_DEF_PROTOCOL_TCP_CHAT_RS),result(SEND_FAIL),friendId(0) {}
	PackType type;
	int userId;
	int friendId;
	int result;
}STRU_TCP_CHAT_RS;

// ��Ӻ�������:Э��ͷ���Լ���ID�����ѵ��ǳơ��Լ����ǳ�
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

//��Ӻ��ѻظ�:Э��ͷ���Լ���ID�����ѵ�ID����ӽ�������ѵ��ǳ�
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

// ����:Э��ͷ���Լ���ID��	
typedef struct  STRU_TCP_OFFLINE_RQ {
	STRU_TCP_OFFLINE_RQ() :type(_DEF_PROTOCOL_TCP_OFFLINE_RQ), userId(0) {}
	PackType type;
	int userId;
}STRU_TCP_OFFLINE_RQ;
