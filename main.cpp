#include <iostream>
#include <string>
//#include "include/sql_pool.hpp"
#include <ctime>
#include "include/epoll_server.hpp"
//#include "http_header.hpp"
#include "unistd.h"


int main(int argc, char** argv)
{
    epoll_server tmp_epoll("0.0.0.0", 6061  );
    tmp_epoll.start_epoll_ET();
    /*
    string client_ip = "128.0.0.9";
    time_t tmp_time = time(0);
    string server_time = ctime(&tmp_time);
    sql_pool cs("./cs.db", 1);
    string SQL_void = ("INSERT INTO http_log (event, time_date, client_IP) VALUES ('连接上服务器', '");
    SQL_void += server_time;
    SQL_void += "', '";
    SQL_void += client_ip;
    SQL_void += "');";
    cout << SQL_void << endl << endl << endl;

    cs.sql_run_SQL(SQL_void);
    */
    //for(int i = 0; i < 500; ++i)
        //cs.run_sql_add("INSERT INTO table_1 (name, old) VALUES ('ys', 553);");
 
    //getchar();




    /*
    sql_pool* cscs = new sql_pool("http_log.db", 10);
    cscs->run_sql_add("INSERT INTO http_log (event, time_date, client_IP) VALUES ('连接上服务器', 'cscs', 'cscs');");
    cscs->run_sql_add("INSERT INTO http_log (event, time_date, client_IP) VALUES ('连接上服务器', 'cscs', 'cscs');");
    cscs->run_sql_add("INSERT INTO http_log (event, time_date, client_IP) VALUES ('连接上服务器', 'cscs', 'cscs');");
    cscs->run_sql_add("INSERT INTO http_log (event, time_date, client_IP) VALUES ('连接上服务器', 'cscs', 'cscs');");
    */
    
    
    sleep(2);

    return 0;
}