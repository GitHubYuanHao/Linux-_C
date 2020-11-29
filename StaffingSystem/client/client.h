#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "protocal.h"
int start(int);
int login_admin(MSG *pmsg,int sockfd);//管理员登入函数
int login_user(MSG *pmsg,int sockfd);//员工登入函数
int user_add(MSG *pmsg,int sockfd);//管理员函数
int user_del(MSG *pmsg,int sockfd);//管理员函数
int user_mod(MSG *pmsg,int sockfd);//管理员函数
int user_find(MSG *pmsg,int sockfd);//管理员函数
int pwd_mod(MSG *pmsg,int sockfd);//用户函数
int person_info(MSG *pmsg,int sockfd);//用户函数

#endif