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
  
    sleep(2);

    return 0;
}