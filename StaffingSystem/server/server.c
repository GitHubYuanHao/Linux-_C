#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "server.h"

void* start(void *arg)
{
	int datafd = *(int *)arg;
	do_client(datafd);
	return NULL;
}

int do_client(int datafd)
{
	sqlite3 *db;
	MSG msg;//存放接受到的消息结构体
	MSG buf;//存放从数据库获得的消息结构体
	MSG *pmsg = &msg;
	MSG *pbuf = &buf;
	
	//MSG buf2={{0,1,"123",1,"yuanhao","china",22,"18845671236",12345.0},{1,1,1,1,1,"123"}};
	
	database_open("test.db",&db);
	//database_create("test.db",&db);
	//database_insert(db,&buf2);
	
label_login:
	recv_pdu(datafd,pmsg);
	pbuf->info = pmsg->info;
	database_login(db,pbuf);
	
	if(pbuf->info.flag == NOPERSON)
	{
		pmsg->info.flag = ERROR_NUM;
		send_pdu(datafd,pmsg);
		goto label_login;
	}
	else if(strcmp(pmsg->info.password,pbuf->info.password))
	{
		pmsg->info.flag = ERROR_PAWD;
		send_pdu(datafd,pmsg);
		goto label_login;
	}
	else
	{
		pmsg->info.flag = LOGIN_SUCCESS;
		pmsg->info.type = pbuf->info.type;
		pmsg->info.num = pbuf->info.num;
		send_pdu(datafd,pmsg);
	}

	while(1)
	{
		recv_pdu(datafd,pmsg);
		switch(msg.info.func)
		{
			case USERADD:add_user(datafd,pmsg,db);break;
			case USERDEL:del_user(datafd,pmsg,db);break;
			case USERMOD:mod_user(datafd,pmsg,db);break;
			case USERFIND:find_user(datafd,pmsg,db);break;
			case USEREXIT:return 0;
			case PWDMOD:mod_pwd(datafd,pmsg,db);break;
			case PERSONINFO:transmit_user(datafd,pmsg,db);break;
			default:break;
		}
	}
}

int add_user(int datafd,MSG *pbuf,sqlite3 *db)
{
	database_insert(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}

int del_user(int datafd,MSG *pbuf,sqlite3 *db)
{
	database_query_int(db,pbuf);
	if(pbuf->info.flag == NOPERSON)
	{
		send_pdu(datafd,pbuf);
		return 0;
	}
	database_delete_information(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}

int mod_user(int datafd,MSG *pbuf,sqlite3 *db)
{
	MSG tmp;
	tmp = *pbuf;
	
	database_query_int(db,&tmp);
	if(tmp.info.flag == NOPERSON)
	{
		send_pdu(datafd,&tmp);
		return 0;
	}
	database_update_information(db,pbuf);
	database_query_int(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}

int find_user(int datafd,MSG *pbuf,sqlite3 *db)
{
	database_query_int(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}

int mod_pwd(int datafd,MSG *pbuf,sqlite3 *db)
{
	database_update_information(db,pbuf);
	database_query_int(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}

int transmit_user(int datafd,MSG *pbuf,sqlite3 *db)
{
	database_query_int(db,pbuf);
	send_pdu(datafd,pbuf);
	return 0;
}







