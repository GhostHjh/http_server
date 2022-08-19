#ifndef _sql_pool_
#define _sql_pool_
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <pthread.h>
#include "include/sqlite3.h"
//#include "../pthread_pool.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;

class sql_pool
{
public:
    sql_pool(const char* db, int sql_pool);
    ~sql_pool();

    void tmp_sqlite_run_sql(string SQL_str);
    void sql_run_SQL(string &SQL_str);
    static void* sql_pthread_run_SQL(void* args);

    void run_sql_add(const char* SQL_str);
    void run_sql_add(string &SQL_str);
    


private:
    sqlite3* sqlite_pool = nullptr;
    string sqlite_db;

    bool sql_pool_status;
    int sql_pool_size;
    vector<string> run_sql_s;
    vector<pthread_t*> sql_run_pool;

    pthread_mutex_t sql_run_str_mutex;
    pthread_cond_t sql_run_str_cond;

    pthread_mutex_t sql_run_mutex;
};

sql_pool::sql_pool(const char* db, int pool) : sqlite_db(db), sql_pool_size(pool)
{
    if (sqlite3_open_v2(sqlite_db.c_str(), &sqlite_pool, SQLITE_OPEN_FULLMUTEX | SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, nullptr)  != SQLITE_OK)
        exit(0);

    sqlite3_config(SQLITE_CONFIG_SERIALIZED);
    
    sql_pool_status = true;
    printf("\n%d\n", sqlite3_threadsafe());

    pthread_mutex_init(&sql_run_str_mutex, 0);
    pthread_cond_init(&sql_run_str_cond, 0);

    pthread_mutex_init(&sql_run_mutex, 0);

    for(int i = 0; i < sql_pool_size; ++i)
    {
        sql_run_pool.push_back(new pthread_t(0));
        pthread_create(sql_run_pool[i], nullptr, &sql_pthread_run_SQL, (void*)this);
    }

    sleep(1);

}

sql_pool::~sql_pool()
{
    sqlite3_close_v2(sqlite_pool);
    printf("\n开始准备结束线程\n");

    for(int i = 0; i < sql_pool_size; ++i)
        pthread_join(*sql_run_pool[i], nullptr);

    pthread_cond_broadcast(&sql_run_str_cond);
    printf("\n线程结束\n");
}

void sql_pool::tmp_sqlite_run_sql(string void_tmp)
{
    sqlite3_stmt* sqlite_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite_pool, void_tmp.c_str(), -1, &sqlite_stmt, nullptr) == SQLITE_OK && sqlite3_step(sqlite_stmt) == SQLITE_OK)
    {
        sqlite3_finalize(sqlite_stmt);
    }
    else
    {
        exit(0);
    }     
}

void sql_pool::sql_run_SQL(string &SQL_str)
{
    //pthread_mutex_lock(&sql_run_mutex);
    sqlite3_stmt* sqlite_stmt = nullptr;
    if (sqlite3_prepare_v2(sqlite_pool, SQL_str.c_str(), -1, &sqlite_stmt, nullptr) == SQLITE_OK && sqlite3_step(sqlite_stmt) == SQLITE_OK)
    {
        sqlite3_finalize(sqlite_stmt);
    }
    /*else
    {
        printf("sql数据库写入出错");
    }*/
    //pthread_mutex_unlock(&sql_run_mutex);

}

void* sql_pool::sql_pthread_run_SQL(void* args)
{
    sql_pool* this_sql_pool = (sql_pool*)args;
    while(1)
    {
        pthread_mutex_lock(&(this_sql_pool->sql_run_str_mutex));
        while (this_sql_pool->run_sql_s.size() == 0)
        {
            pthread_cond_wait(&(this_sql_pool->sql_run_str_cond), &(this_sql_pool->sql_run_str_mutex));
        }

        string tmp_run_sql = this_sql_pool->run_sql_s.back();
        this_sql_pool->run_sql_s.pop_back();
        pthread_mutex_unlock(&(this_sql_pool->sql_run_str_mutex));

        this_sql_pool->sql_run_SQL(tmp_run_sql);
    }
}

void sql_pool::run_sql_add(const char* SQL_str)
{
    pthread_mutex_lock(&sql_run_str_mutex);
    run_sql_s.push_back(SQL_str);
    pthread_mutex_unlock(&sql_run_str_mutex);
    pthread_cond_broadcast(&sql_run_str_cond);
}

void sql_pool::run_sql_add(string &SQL_str)
{
    run_sql_add(SQL_str.c_str());
}

#endif