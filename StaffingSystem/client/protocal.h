#ifndef __PROTOCAL_H__
#define __PROTOCAL_H__

#define NAME_NUM     20
#define PASSWORD_NUM 10
#define ADDR_NUM     40
#define PHONE_NUM    12

#define ADMINISTRATOR 0
#define STAFF         1


#define LOGIN_SUCCESS 0
#define ERROR_NUM     1
#define ERROR_PAWD    2
#define NOPERSON      3

/*管理员功能函数*/
#define USERADD       0
#define USERDEL       1
#define USERMOD       2
#define USERFIND      3
#define USEREXIT      4

/*普通用户功能函数*/
#define PWDMOD        5
#define PERSONINFO    6

/*员工信息修改*/

#define MODPWD        1
#define MODLEVEL      2
#define MODADDR       3
#define MODAGE        4
#define MODPHONE      5
#define MODSALARY     6

#define MODTYPE       7
#define MODNAME       8

#include <sqlite3.h>

typedef struct 
{
	int  type;                  //权限标志位
	int  num;              		//编号
	char password[PASSWORD_NUM];//密码
	int level;            		//等级
	char name[NAME_NUM];        //姓名
	char addr[ADDR_NUM];  		//地址
	int age;             		//年龄
	char phone[PHONE_NUM];		//手机号
	double salary;       		//工资
}USER;

typedef struct 
{
	int flag;                   //功能执行是否成功
	int  func;                  //功能选择位
	int  func2;					//功能选择位2
	int  type;                  //权限标志位
	int  num;              		//编号
	char password[PASSWORD_NUM];//密码
}INFO;

typedef struct
{
	USER user;
	INFO info;
}MSG;


int send_pdu(int sockfd,MSG *);
int recv_pdu(int sockfd,MSG *);

int tcp_accept_datafd(int sockfd);
int tcp_server_test(int sockfd);
int tcp_client_test(int sockfd);
int tcp_server_create(int argc, char *argv[3]);
int tcp_client_create(int argc, char *argv[3]);

int msg_print(MSG *pbuf,int num);
int user_show(MSG *pbuf);

int callback_query(void *data,int argc,char *argv[],char **azColName);
int callback_login(void *data,int argc,char *argv[],char **azColName);
int database_open(char *path,sqlite3 **p_db);
int database_close(sqlite3 **p_db);
int database_create(char *path,sqlite3 **p_db);
int database_insert(sqlite3 *db,MSG *pbuf);
int database_delete_information(sqlite3 *db,MSG *pbuf);
int database_query_int(sqlite3 *db,MSG *pbuf);
int database_login(sqlite3 *db,MSG *pbuf);
int database_update_information(sqlite3 *db,MSG *pbuf);

#endif