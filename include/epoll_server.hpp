#ifndef _epoll_server_
#define _epoll_server_
#include <iostream>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "http_header.hpp"
//#include "pthread_pool.hpp"

class epoll_server
{
public:
    epoll_server();
    epoll_server(char*, uint16_t);
    ~epoll_server();

    bool fd_fcntl(int fd);

    //void start_epoll_ET();
    //void start_epoll_LT();

    bool epoll_EPOLLIN_ADD_events_ET(int server_socket);
    bool epoll_EPOLLERR_EPOLLHUP_DEL_events(int client_socket);
    bool epoll_EPOLLIN_read_client(int client_socket);
    bool epoll_EPOLLIN_read_write_client(int client_socket);
    void epoll_events_if(epoll_event &ep_ev);

    void start_epoll_ET();
    

private:
    int server_socket;
    sockaddr_in server_sockaddr;
    in_addr_t IP;
    uint16_t PROT;
    socklen_t server_sockaddr_len;
    int server_epoll;
    epoll_event server_epoll_ev[1024];
};

epoll_server::epoll_server()
{
    epoll_server("0.0.0.0", 80);
}

epoll_server::epoll_server(char* ip, uint16_t prot)
{
    server_socket = {0};
    server_sockaddr = {0};
    IP = inet_addr(ip);
    PROT= htons(prot);
    server_sockaddr_len = {0};
    server_epoll = epoll_create(1024);
    server_epoll_ev[1024] = {0};

    //设置socket的属性
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    server_sockaddr.sin_family = AF_INET;
    server_sockaddr.sin_addr.s_addr = IP;
    server_sockaddr.sin_port = PROT;
    server_sockaddr_len = sizeof(server_sockaddr);

    printf("http_server\n%s:%d\n\n", inet_ntoa(server_sockaddr.sin_addr) , ntohs(server_sockaddr.sin_port));

    //将socket属性和socket绑定并让socket进入被动连接状态
    bind(server_socket, (sockaddr*)&server_sockaddr, server_sockaddr_len);
    listen(server_socket, 1024);

    epoll_event tmp_server_epoll_ev = {0};
    tmp_server_epoll_ev.events = EPOLLIN;
    tmp_server_epoll_ev.data.fd = server_socket;

    //设置epoll触发事件
    epoll_ctl(server_epoll, EPOLL_CTL_ADD, server_socket, &tmp_server_epoll_ev);
}

epoll_server::~epoll_server()
{
    close(server_socket);
    //close(server_epoll);
}

bool epoll_server::fd_fcntl(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags < 0) 
    {
        std::cout << "set no block error, fd:" << server_socket << std::endl;
        return false;
    }
    if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) < 0) 
    {
        std::cout << "set no block error, fd:" << server_socket << std::endl;
        return false;
    }

    return true;
}

/*void epoll_server::start_epoll_ET()
{
    for(;;)
    {
        int client_quantity = epoll_wait(server_epoll, server_epoll_ev, 1024, -1);

        for(int i = 0; i < client_quantity; ++i)
        {
            std::cout << server_epoll_ev[i].data.fd << "   " << server_epoll_ev[i].events << std::endl;
            if (server_epoll_ev[i].data.fd == server_socket)
            {
                if (server_epoll_ev[i].events & EPOLLIN)
                {
                    int tmp_client_socket = {};
                    sockaddr_in tmp_client_sockaddr = {};
                    socklen_t tmp_client_sockaddr_len = sizeof(tmp_client_sockaddr);

                    tmp_client_socket = accept(server_socket, (sockaddr*)&tmp_client_sockaddr, &tmp_client_sockaddr_len);

                    epoll_event tmp_server_epoll_ev ={};
                    tmp_server_epoll_ev.events = EPOLLIN|EPOLLET;
                    tmp_server_epoll_ev.data.fd = tmp_client_socket;

                    int flags = fcntl(tmp_client_socket, F_GETFL, 0);
                    if (flags < 0) 
                    {
                        std::cout << "set no block error, fd:" << server_socket << std::endl;
                        continue;
                    }
                    if (fcntl(server_socket, F_SETFL, flags | O_NONBLOCK) < 0) 
                    {
                        std::cout << "set no block error, fd:" << server_socket << std::endl;
                        continue;
                    }

                    epoll_ctl(server_epoll, EPOLL_CTL_ADD, tmp_client_socket, &tmp_server_epoll_ev);
                    printf("客戶端标识\t%d\nIP: %s:%d\t链接上服务器\n\n", tmp_client_socket, inet_ntoa(tmp_client_sockaddr.sin_addr) , ntohs(tmp_client_sockaddr.sin_port));
                }
            }
            else
            {
                if ((server_epoll_ev[i].events & EPOLLERR) || (server_epoll_ev[i].events & EPOLLHUP))
                {
                    epoll_ctl(server_epoll, EPOLL_CTL_DEL, server_epoll_ev[i].data.fd, nullptr);
                    close(server_epoll_ev[i].data.fd);
                    printf("客戶端标识\t%d\t和服务器断开连接\n\n", server_epoll_ev[i].data.fd);
                }
                else if (server_epoll_ev[i].events & EPOLLIN)
                {
                    char tmp_ctr[1024] = {0};
                    size_t tmp_ctr_size = read(server_epoll_ev[i].data.fd, tmp_ctr, 1024);

                    if (tmp_ctr_size == -1)
                    {
                        epoll_ctl(server_epoll, EPOLL_CTL_DEL, server_epoll_ev[i].data.fd, nullptr);
                        close(server_epoll_ev[i].data.fd);
                        printf("客戶端标识\t%d\t和服务器断开连接\n\n", server_epoll_ev[i].data.fd);
                    }
                    else if (tmp_ctr_size != 0)
                    {
                        printf("客戶端标识\t%d\n发送数据: %s", server_epoll_ev[i].data.fd, tmp_ctr);
                        while(tmp_ctr_size == 1024)
                        {
                            tmp_ctr_size = read(server_epoll_ev[i].data.fd, tmp_ctr, 1024);
                            printf(tmp_ctr);
                        }
                        printf("\n\n");
                    }
                    else
                    {
                        write(server_epoll_ev[i].data.fd, "exit", sizeof("exit"));
                    }
                }
            }
        }
    }
}*/

bool epoll_server::epoll_EPOLLIN_ADD_events_ET(int server_socket)
{
    int tmp_client_socket = {0};
    sockaddr_in tmp_client_sockaddr = {0};
    socklen_t tmp_client_sockaddr_len = sizeof(tmp_client_sockaddr);

    tmp_client_socket = accept(server_socket, (sockaddr*)&tmp_client_sockaddr, &tmp_client_sockaddr_len);

    epoll_event tmp_server_epoll_ev ={0};
    tmp_server_epoll_ev.events = EPOLLIN|EPOLLET;
    tmp_server_epoll_ev.data.fd = tmp_client_socket;

    if (fd_fcntl(tmp_client_socket))
    {
        epoll_ctl(server_epoll, EPOLL_CTL_ADD, tmp_client_socket, &tmp_server_epoll_ev);
        printf("客戶端标识\t%d\nIP: %s:%d\t链接上服务器\n\n", tmp_client_socket, inet_ntoa(tmp_client_sockaddr.sin_addr) , ntohs(tmp_client_sockaddr.sin_port));
        return true;
    }
    else
        return false;
}

bool epoll_server::epoll_EPOLLERR_EPOLLHUP_DEL_events(int client_socket)
{
    if (epoll_ctl(server_epoll, EPOLL_CTL_DEL, client_socket, nullptr) == 0)
    {
        close(client_socket);
        printf("客戶端标识\t%d\t和服务器断开连接\n\n", client_socket);
        return true;
    }
    return false;
}

bool epoll_server::epoll_EPOLLIN_read_client(int client_socket)
{
    char tmp_ctr[3072] = {0};
    size_t tmp_ctr_size = read(client_socket, tmp_ctr, 3072);

    if(tmp_ctr_size == -1 || tmp_ctr_size == 0)
    {
        return epoll_EPOLLERR_EPOLLHUP_DEL_events(client_socket);
    }
    else if (tmp_ctr_size != 0)
    {
        printf("客戶端标识\t%d\n发送数据:\n%s", client_socket, tmp_ctr);
        while(tmp_ctr_size == 3072)
        {
            tmp_ctr_size = read(client_socket, tmp_ctr, 3072);
            printf(tmp_ctr);
        }
        printf("\n\n");
        return true;
    }   
}

bool epoll_server::epoll_EPOLLIN_read_write_client(int client_socket)
{
    char tmp_ctr[3072] = {0};
    size_t tmp_ctr_size = read(client_socket, tmp_ctr, 3072);

    if(tmp_ctr_size == -1 || tmp_ctr_size == 0)
    {
        return epoll_EPOLLERR_EPOLLHUP_DEL_events(client_socket);
    }
    else if (tmp_ctr_size != 0)
    {
        //printf("客戶端标识\t%d\n发送数据:\n%s", client_socket, tmp_ctr);
        while(tmp_ctr_size == 3072)
        {
            tmp_ctr_size = read(client_socket, tmp_ctr, 3072);
            //printf(tmp_ctr);
        }

        //printf("\n\n");
        //char* tmp_write_1 = new char[81];
        //strcpy(tmp_write_1, "HTTP/1.1 200\nserver:simple web server\nConten-length:2048\nContent-type:text/html\n");
        //write(client_socket, tmp_write_1, 81);
        //cout << tmp_write_1 << endl;
        //char* tmp_write_2 = new char[122];
        //strcpy(tmp_write_2, "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>test</title>\n</head>\n<body>\n    Hello World.\n</body>\n</html>\n");
        //write(client_socket, tmp_write_2, 122);
        //cout << tmp_write_2 << endl;
        //delete[](tmp_write_1);
        //delete[](tmp_write_2);

        http_header* header = new http_header(tmp_ctr, tmp_ctr_size);
        write(client_socket, header->get_http_server_header(), header->get_http_server_header_size());
        write(client_socket, header->get_http_request_path_file(), header->get_http_request_path_file_size());
        delete(header);

        return epoll_EPOLLERR_EPOLLHUP_DEL_events(client_socket);
    }   
}

void epoll_server::epoll_events_if(epoll_event &ep_ev)
{
    //std::cout << ep_ev.data.fd << "   " << ep_ev.events << std::endl;
    if (ep_ev.data.fd == server_socket && ep_ev.events & EPOLLIN)
    {
        //int tmp_ =  server_epoll_ev[i].data.fd;
        if (epoll_EPOLLIN_ADD_events_ET(ep_ev.data.fd) == false)
            exit(0);
    }
    else
    {
        //int tmp_ =  server_epoll_ev[i].data.fd;
        if (epoll_EPOLLIN_read_write_client(ep_ev.data.fd) == false)
            exit(0);
    }
}

void epoll_server::start_epoll_ET()
{
    for(;;)
    {
        int client_quantity = epoll_wait(server_epoll, server_epoll_ev, 1024, -1);

        for(int i = 0; i < client_quantity; ++i)
        {
            epoll_events_if(server_epoll_ev[i]);
        }
    }
}


#endif _epoll_server_