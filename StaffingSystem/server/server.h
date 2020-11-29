#ifndef __SERVER_H__
#define __SERVER_H__

#include "protocal.h"

#define PRIVATE_PATH "private.txt"
#define USER_PATH "user.txt"
#define DATABASE_PATH "user.db"

void* start(void *arg);
int do_client(int datafd);

int add_user(int datafd,MSG *pbuf,sqlite3 *db);
int del_user(int datafd,MSG *pbuf,sqlite3 *db);
int mod_user(int datafd,MSG *pbuf,sqlite3 *db);
int find_user(int datafd,MSG *pbuf,sqlite3 *db);
int mod_pwd(int datafd,MSG *pbuf,sqlite3 *db);
int transmit_user(int datafd,MSG *pbuf,sqlite3 *db);

#endif
