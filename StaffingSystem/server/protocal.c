#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sqlite3.h>
#include "protocal.h"


/****************************************
*网络通信
*****************************************/
int send_pdu(int sockfd,MSG *pmsg)
{
	return send(sockfd,pmsg,sizeof(*pmsg),0);
}

int recv_pdu(int sockfd,MSG *pmsg)
{
	return recv(sockfd,pmsg,sizeof(*pmsg),0);
}

int tcp_server_create(int argc, char *argv[3])
{
	int sockfd=-1, ret = -1, addr_len = 0;
	struct sockaddr_in serv_addr;
	
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	addr_len = sizeof(serv_addr);

    bzero(&serv_addr, addr_len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    

    ret = bind(sockfd,(struct sockaddr *)&serv_addr,addr_len);
    if (-1 == ret)
    {
        perror("bind");
        exit(-1);
    }
	
	ret = listen(sockfd,5);
	if (-1 == ret)
    {
        perror("listen");
        exit(-1);
    }	
	return sockfd;
}

int tcp_server_test(int sockfd)
{
	int datafd = -1, ret = -1;
	char buf[30] = "";
	datafd = accept(sockfd,NULL,NULL);
	
    ret = recv(datafd, buf, sizeof(buf), 0);
    if (-1 == ret)
    {
        perror("recv");
        exit(-1);
    }
    printf("receive from client: %s\n", buf);
	
	ret = send(datafd, buf, sizeof(buf), 0);
    if (-1 == ret)
    {
        perror("send");
        exit(-1);
    }
	
    printf("send to client: %s\n", buf);
	close(datafd);
	datafd = -1;
	return 0;
}

int tcp_client_create(int argc, char *argv[3])
{
	static int sockfd = -1;
	int ret = -1, addr_len = 0;
	struct sockaddr_in serv_addr;

	if (argc < 3)
	{
		printf("Usage : %s <serv_ip> <serv_port>\n", argv[0]);
		exit(-1);
	}
	
	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		perror("fail to socket");
		exit(-1);
	}

	addr_len = sizeof(serv_addr);
	
	bzero(&serv_addr, addr_len);
	
	serv_addr.sin_family = PF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
	serv_addr.sin_port = htons(atoi(argv[2]));
	
	ret = connect(sockfd, (struct sockaddr *)&serv_addr, addr_len);
    if (-1 == ret)
    {
        perror("connect");
        exit(-1);
    }
	return sockfd;
}

int tcp_client_test(int sockfd)
{
	int ret = -1;
	char buf[30] = "Good Job!";
	
	ret = send(sockfd, buf, sizeof(buf), 0);
    if (-1 == ret)
    {
        perror("send");
        exit(-1);
    }
    printf("send to server: %s\n", buf);

    ret = recv(sockfd, buf, sizeof(buf), 0);
    if (-1 == ret)
    {
        perror("recv");
        exit(-1);
    }
    printf("receive from server: %s\n", buf);
	return 0;
}

int tcp_accept_datafd(int sockfd)
{
	return accept(sockfd,NULL,NULL);
}


int msg_print(MSG *pbuf,int num)
{
	while(num--)
	{
		printf("%d, %d, %s, %d, %s, %s, %d, %s, %lf\n",
			pbuf->user.type,pbuf->user.num,pbuf->user.password,pbuf->user.level,
			pbuf->user.name,pbuf->user.addr,pbuf->user.age,pbuf->user.phone,pbuf->user.salary);
		pbuf++;
	}
	return 0;
}

int user_show(MSG *pbuf)
{
	printf("权限:%d\n编号:%d\n密码:%s\n等级:%d\n姓名:%s\n地址:%s\n年龄:%d\n手机号:%s\n工资:%lf\n",
			pbuf->user.type,pbuf->user.num,pbuf->user.password,pbuf->user.level,
			pbuf->user.name,pbuf->user.addr,pbuf->user.age,pbuf->user.phone,pbuf->user.salary);
	return 0;
}
/****************************************
*sqlite3数据库支持
*****************************************/
int callback_query(void *data,int argc,char *argv[],char **azColName)
{
	int i = 0;
	MSG *pbuf = (MSG *)data;
	
	pbuf->user.type = atoi(argv[i++]);
	pbuf->user.num = atoi(argv[i++]);
	strcpy(pbuf->user.password,argv[i++]);
	pbuf->user.level = atoi(argv[i++]);
	strcpy(pbuf->user.name,argv[i++]);
	strcpy(pbuf->user.addr,argv[i++]);
	pbuf->user.age = atoi(argv[i++]);
	strcpy(pbuf->user.phone,argv[i++]);
	pbuf->user.salary = atof(argv[i++]);
	return 0;
}

int callback_login(void *data,int argc,char *argv[],char **azColName)
{
	int i = 0;
	MSG *pbuf = (MSG *)data;
	
	pbuf->info.type = atoi(argv[i++]);
	pbuf->info.num = atoi(argv[i++]);
	strcpy(pbuf->info.password,argv[i++]);
	return 0;
}

int database_open(char *path,sqlite3 **p_db)
{
	if (sqlite3_open(path, p_db) == SQLITE_OK)
	{
		printf("数据库打开成功\n");
		return 0;
	}
	else
	{
		printf("error : %s\n", sqlite3_errmsg(*p_db));
		return -1;
	}
}

int database_close(sqlite3 **p_db)
{
	sqlite3_close(*p_db);
	printf("数据库关闭成功\n");
	return 0;
}

int database_create(char *path,sqlite3 **p_db)
{
	char *zErrMsg = 0;
	int  ret;
	char *sql;
	
	if (sqlite3_open(path, p_db) == SQLITE_OK)
	{
		printf("数据库打开成功\n");
	}
	else
	{
		printf("error : %s\n", sqlite3_errmsg(*p_db));
		return -1;
	}
	
	sql = "CREATE TABLE COMPANY("  \
		 "TYPE       INT     NOT NULL," \
		 "NUM            INT     NOT NULL," \
		 "PASSWORD      CHAR(10) NOT NULL," \
		 "LEVEL          INT     NOT NULL," \
		 "NAME    CHAR(20) PRIMARY KEY   NOT NULL,"\
		 "ADDRESS       CHAR(40) NOT NULL," \
         "AGE            INT     NOT NULL," \
		 "PHONE      CHAR(12) NOT NULL," \
		 "SALARY     REAL    NOT NULL);";
		 
	ret = sqlite3_exec(*p_db, sql, NULL, NULL, &zErrMsg);
	
	if( ret != SQLITE_OK )
	{
		fprintf(stderr, "数据表创建失败: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else
	{
		fprintf(stdout, "数据表创建成功\n");
	}
	return 0;		 
}

int database_insert(sqlite3 *db,MSG *pbuf)
{
	char sqlstr[512] = "";
	char *p_errmsg = NULL;
	
	sprintf(sqlstr, "insert into COMPANY values ('%d', '%d', '%s', '%d', '%s', '%s', '%d', '%s', '%lf')"
			,pbuf->user.type,pbuf->user.num,pbuf->user.password,pbuf->user.level
			,pbuf->user.name,pbuf->user.addr,pbuf->user.age,pbuf->user.phone,pbuf->user.salary);
	
	if (sqlite3_exec(db, sqlstr, NULL, NULL, &p_errmsg) == SQLITE_OK)
	{	
		printf("数据库记录插入成功\n");
		return 0;
	}
	printf("数据库记录插入失败 : %s\n",p_errmsg);
	
	sqlite3_free(p_errmsg);
	return -1;
}

int database_delete_information(sqlite3 *db,MSG *pbuf)
{
	char sqlstr[512] = "";
	char *p_errmsg;
	
	sprintf(sqlstr, "delete from COMPANY where NUM = '%d'", pbuf->user.num);
	if (sqlite3_exec(db, sqlstr, NULL, NULL, &p_errmsg) == SQLITE_OK)
	{
		printf("数据库记录删除成功\n");
		return 0;
	}
	printf("数据库记录删除失败 : %s\n",p_errmsg);
	sqlite3_free(p_errmsg);
	return -1;
}

int database_query_int(sqlite3 *db,MSG *pbuf)
{
	char sqlstr[512] = "";
	char *p_errmsg;
	char **dbResult;
	int nRow=0, nColumn=0;
	
	sprintf(sqlstr, "select * from COMPANY where NUM = '%d'", pbuf->user.num);
	sqlite3_get_table(db,sqlstr,&dbResult,&nRow,&nColumn,&p_errmsg);
	if(NULL!=p_errmsg)  
	{  
		sqlite3_free_table(dbResult);  
		p_errmsg=NULL;  
		return -1; 
	}
	
	if(nColumn == 0)
	{
		pbuf->info.flag = NOPERSON;
		return -1;
	}		
	
	if (sqlite3_exec(db, sqlstr, callback_query, (void*)pbuf, &p_errmsg) == SQLITE_OK)
	{
		printf("数据库记录查询成功\n");
		return 0;
	}
	printf("数据库记录查询失败 : %s\n",p_errmsg);
	sqlite3_free(p_errmsg);
	return -1;
}

int database_login(sqlite3 *db,MSG *pbuf)
{
	char sqlstr[512] = "";
	char *p_errmsg;
	char **dbResult;
	int nRow=0,nColumn=0;
	
	sprintf(sqlstr, "select * from COMPANY where NUM = '%d'", pbuf->info.num);
	sqlite3_get_table(db,sqlstr,&dbResult,&nRow,&nColumn,&p_errmsg);
	if(NULL!=p_errmsg)  
	{  
		sqlite3_free_table(dbResult);
		p_errmsg=NULL;
		return -1;
	}
	if(nColumn == 0)
	{
		pbuf->info.flag = NOPERSON;
		return -1;
	}

	printf("\n**this id database_login %d \n",pbuf->info.num);///////////////////
	if (sqlite3_exec(db, sqlstr, callback_login, (void*)pbuf, &p_errmsg) == SQLITE_OK)
	{
		printf("数据库记录查询成功\n");
		return 0;
	}
	printf("数据库记录查询失败 : %s\n",p_errmsg);
	sqlite3_free(p_errmsg);
	return -1;
}

int database_update_information(sqlite3 *db,MSG *pbuf)
{
	char sqlstr[512] = "";
	char *p_errmsg;
	
	switch(pbuf->info.func2)
	{
	case MODPWD:
		sprintf(sqlstr, "UPDATE COMPANY set PASSWORD = '%s' where NUM = '%d'", pbuf->user.password, pbuf->user.num);
	break;
	
	case MODLEVEL:
		sprintf(sqlstr, "UPDATE COMPANY set LEVEL = '%d' where NUM = '%d'", pbuf->user.level, pbuf->user.num);
	break;
		
	case MODADDR:
		sprintf(sqlstr, "UPDATE COMPANY set ADDRESS = '%s' where NUM = '%d'", pbuf->user.addr, pbuf->user.num);
	break;
		
	case MODAGE:
		sprintf(sqlstr, "UPDATE COMPANY set AGE = '%d' where NUM = '%d'", pbuf->user.age, pbuf->user.num);	
	break;
	
	case MODPHONE:
		sprintf(sqlstr, "UPDATE COMPANY set PHONE = '%s' where NUM = '%d'", pbuf->user.phone, pbuf->user.num);
	break;
	
	case MODSALARY:
		sprintf(sqlstr, "UPDATE COMPANY set SALARY = '%lf' where NUM = '%d'", pbuf->user.salary, pbuf->user.num);
	break;
	
	default:break;
	}
	if (sqlite3_exec(db, sqlstr, NULL, NULL, &p_errmsg) == SQLITE_OK)
	{
		printf("数据库记录更新成功\n");
		return 0;
	}
	printf("数据库记录更新失败 : %s\n",p_errmsg);
	sqlite3_free(p_errmsg);
	return -1;
}

