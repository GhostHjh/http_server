#include <iostream>
#include <string>
#include "epoll_server.hpp"
#include "http_header.hpp"


int main(int argc, char** argv)
{
    epoll_server tmp_epoll("127.0.0.1", 6060);
    tmp_epoll.start_epoll_ET();
    
    //char* tmp_str = new char[644];
    //stpcpy(tmp_str, "GET /xx.txt HTTP/1.1\nHost: 127.0.0.1:6061\nConnection: keep-alive\nsec-ch-ua: \".Not/A)Brand\";v=\"99\", \"Google Chrome\";v=\"103\", \"Chromium\";v=\"103\"\nsec-ch-ua-mobile: ?0\nsec-ch-ua-platform: \"Linux\"\nDNT: 1\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9\nSec-Fetch-Site: none\nSec-Fetch-Mode: navigate\nSec-Fetch-User: ?1\nSec-Fetch-Dest: document\nAccept-Encoding: gzip, deflate, br\nAccept-Language: zh-CN,zh;q=0.9");
    //http_header* cs = new http_header(tmp_str);
    //cout << cs->get_http_request_path_file();
    
    return 0;
}