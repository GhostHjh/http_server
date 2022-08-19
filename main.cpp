#include <iostream>
#include <string>
#include "epoll_server.hpp"
//#include "http_header.hpp"
#include "include/sql_pool.hpp"
#include "unistd.h"


int main(int argc, char** argv)
{
    epoll_server tmp_epoll("192.168.37.128", 8091);
    tmp_epoll.start_epoll_ET();
    //sql_pool cs("./cs.db", 1);
    //string* SQL_void = new string("INSERT INTO table_1 (name, old) VALUES ('ys', 550);");
    //cs.sql_run_SQL((void*)SQL_void);
    //for(int i = 0; i < 500; ++i)
    //    cs.run_sql_add("INSERT INTO table_1 (name, old) VALUES ('ys', 553);");
 
    //getchar();
    //sleep(2);

    return 0;
}