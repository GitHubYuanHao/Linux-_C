#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "client.h"

int start(int sockfd)
{
	MSG msg;
	MSG *pmsg = &msg;
label:	
	printf("*****************************************************************\n");
	printf("*                     欢迎使用员工管理系统                      *\n");
	printf("*****************************************************************\n");
	printf("*  请输入用户名和密码:                                          *\n");
	printf("*         用户名长度限制为20个字符,密码的长度限制为10个字符     *\n");
	printf("*****************************************************************\n");
	printf("员工编号:");
	scanf("%d",&pmsg->info.num);
	printf("密码:");
	scanf("%s",pmsg->info.password);
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,pmsg);
	switch(pmsg->info.flag)
	{
		case LOGIN_SUCCESS:
			switch(pmsg->info.type)
			{
				case ADMINISTRATOR:login_admin(pmsg,sockfd);break;
				case STAFF:login_user(pmsg,sockfd);break;
				default:break;
			}
		break;
		case ERROR_NUM:printf("员工编号错误\n");goto label;
		case ERROR_PAWD:printf("密码错误\n");goto label;
		default:break;
	}
	printf("退出成功!\n");
	return 0;
}

int login_admin(MSG *pmsg, int sockfd)
{
	int num;
label:
	system("clear");
	printf("*****************************************************************\n");
	printf("*                   员工管理系统(管理员模式)                    *\n");
	printf("*****************************************************************\n");
	printf("*                                                               *\n");
	printf("* 1.增加用户    2.删除用户    3.修改用户    4.查找用户    5.退出*\n");
	printf("*                                                               *\n");
	printf("*****************************************************************\n");
	printf("请选择:");
	scanf("%d",&num);
	switch(num)
	{
		case 1:user_add(pmsg,sockfd);break;
		case 2:user_del(pmsg,sockfd);break;
		case 3:user_mod(pmsg,sockfd);break;
		case 4:user_find(pmsg,sockfd);break;
		case 5:pmsg->info.func = USEREXIT;
			   send_pdu(sockfd,pmsg);
			   return 0;
		default:goto label;break;
	}
	goto label;
}

int login_user(MSG *pmsg, int sockfd)
{
	int num;
label:
	system("clear");
	printf("*****************************************************************\n");
	printf("*                   员工管理系统(用户模式)                      *\n");
	printf("*****************************************************************\n");
	printf("*                                                               *\n");
	printf("* 1.修改个人密码            2.查看个人信息            3.退出系统*\n");
	printf("*                                                               *\n");
	printf("*****************************************************************\n");
	printf("请选择:");
	scanf("%d",&num);
	switch(num)
	{
		case 1:pwd_mod(pmsg,sockfd);break;
		case 2:person_info(pmsg,sockfd);break;
		case 3:pmsg->info.func = USEREXIT;
			   send_pdu(sockfd,pmsg);
			   return 0;
		default:goto label;break;
	}
	goto label;
	return 0;
}

int user_add(MSG *pmsg, int sockfd)
{
	MSG msg;
	pmsg->info.func = USERADD;
	system("clear");
	printf("*****************************************************************\n");
	printf("*             员工管理系统(管理员模式)----员工添加              *\n");
	printf("*****************************************************************\n");
	printf("请输入员工权限(0:管理员,1:普通员工):");
	scanf("%d",&pmsg->user.type);
	printf("请输入员工编号:");
	scanf("%d",&pmsg->user.num);
	printf("请输入员工初始密码(长度限制%d个字符):",PASSWORD_NUM-1);
	scanf("%s",pmsg->user.password);
	printf("请输入员工等级:");
	scanf("%d",&pmsg->user.level);
	printf("请输入员工名(长度限制%d个字符):",NAME_NUM-1);
	scanf("%s",pmsg->user.name);
	printf("请输入员工地址(长度限制%d个字符):",ADDR_NUM-1);
	scanf("%s",pmsg->user.addr);
	printf("请输入员工年龄:");
	scanf("%d",&pmsg->user.age);
	printf("请输入员工手机号(长度限制%d个字符):",PHONE_NUM-1);
	scanf("%s",pmsg->user.phone);
	printf("请输入员工工资:");
	scanf("%lf",&pmsg->user.salary);
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	user_show(&msg);
	printf("用户添加完成,按任意键返回\n");
	getchar();
	getchar();
	return 0;
}

int user_del(MSG *pmsg, int sockfd)
{
	MSG msg;
	pmsg->info.func = USERDEL;
	system("clear");
	printf("*****************************************************************\n");
	printf("*             员工管理系统(管理员模式)----员工删除              *\n");
	printf("*****************************************************************\n");
	printf("请输入员工编号:");
	scanf("%d",&pmsg->user.num);
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	if(msg.info.flag == NOPERSON)
	{
		printf("用户不存在,按任意键返回\n");
	}
	else
	{
		printf("用户删除完成,按任意键返回\n");
	}
	getchar();
	getchar();
	return 0;
}

int user_mod(MSG *pmsg, int sockfd)
{
	int num = -1;
	MSG msg;
	pmsg->info.func = USERMOD;
	system("clear");
	printf("*****************************************************************\n");
	printf("*           员工管理系统(管理员模式)----员工信息修改            *\n");
	printf("*****************************************************************\n");
	
	printf("请输入员工编号:");
	scanf("%d",&pmsg->user.num);
label:
	system("clear");
	printf("*****************************************************************\n");
	printf("*    1.修改员工密码      2.修改员工等级      3.修改员工地址     *\n");
	printf("*    4.修改员工年龄      5.修改员工手机号    6.修改员工工资     *\n");
	printf("*****************************************************************\n");
	
	scanf("%d",&num);
	switch(num)
	{
	case MODPWD:
		printf("请输入员工密码(长度限制%d个字符):",PASSWORD_NUM-1);
		scanf("%s",pmsg->user.password);
		pmsg->info.func2 = MODPWD;
	break;
	case MODLEVEL:
		printf("请输入员工等级:");
		scanf("%d",&pmsg->user.level);
		pmsg->info.func2 = MODLEVEL;
	break;
	case MODADDR:
		printf("请输入员工地址(长度限制%d个字符):",ADDR_NUM-1);
		scanf("%s",pmsg->user.addr);
		pmsg->info.func2 = MODADDR;
	break;
	case MODAGE:
		printf("请输入员工年龄:");
		scanf("%d",&pmsg->user.age);
		pmsg->info.func2 = MODAGE;
	break;
	case MODPHONE:
		printf("请输入员工手机号(长度限制%d个字符):",PHONE_NUM-1);
		scanf("%s",pmsg->user.phone);
		pmsg->info.func2 = MODPHONE;
	break;
	case MODSALARY:
		printf("请输入员工工资:");
		scanf("%lf",&pmsg->user.salary);
		pmsg->info.func2 = MODSALARY;
	break;
	default:
		goto label;
	}
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	if(msg.info.flag == NOPERSON)
	{
		printf("用户不存在,按任意键返回\n");
	}
	else
	{
		user_show(&msg);
		printf("用户信息修改完成,按任意键返回\n");
	}
	getchar();
	getchar();
	return 0;
}

int user_find(MSG *pmsg, int sockfd)
{
	MSG msg;
	pmsg->info.func = USERFIND;
	printf("请输入员工号:");
	scanf("%d",&pmsg->user.num);
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	
	if(msg.info.flag == NOPERSON)
	{
		printf("用户不存在,按任意键返回\n");
	}
	else 
	{
		user_show(&msg);
		printf("用户已找到,按任意键返回\n");
	}
	getchar();
	getchar();
	return 0;
}

int pwd_mod(MSG *pmsg, int sockfd)
{
	MSG msg;
	pmsg->info.func = PWDMOD;
	pmsg->info.func2 = MODPWD;
	pmsg->user.num = pmsg->info.num;
	printf("请输入新密码(长度限制%d个字符):",PASSWORD_NUM-1);
	scanf("%s",pmsg->user.password);
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	user_show(&msg);
	printf("用户密码修改完成,按任意键返回\n");
	getchar();
	getchar();
	return 0;
}

int person_info(MSG *pmsg, int sockfd)
{
	MSG msg;
	pmsg->info.func = PERSONINFO;
	pmsg->user.num = pmsg->info.num;
	send_pdu(sockfd,pmsg);
	
	recv_pdu(sockfd,&msg);
	user_show(&msg);
	printf("按任意键返回\n");
	getchar();
	getchar();
	return 0;
}
