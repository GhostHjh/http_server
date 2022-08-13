#include <iostream>
#include <string>
#include "epoll_server.hpp"


int main(int argc, char** argv)
{
    epoll_server tmp_epoll("127.0.0.1", 6061);
    tmp_epoll.start_epoll_ET();

    /*
    while (true) 
    {
        //这个函数会阻塞,直到超时或者有响应事件发生
        int eNum = epoll_wait(server_epoll, server_epoll_ev, 1024, -1);

        if (eNum == -1) 
        {
            std::cout << "epoll_wait" << std::endl;
            return -1;
        }

        //遍历所有的事件
        for (int i = 0; i < eNum; i++) 
        {
            std::cout << server_epoll_ev[i].data.fd << "   " << server_epoll_ev[i].events << std::endl;
            //判断这次是不是socket可读(是不是有新的连接)
            if (server_epoll_ev[i].data.fd == server_socket) 
            {
                if (server_epoll_ev[i].events & EPOLLIN) 
                {
                    sockaddr_in cli_addr{};
                    socklen_t length = sizeof(cli_addr);
                    //接受来自socket连接
                    int tmp_cilent_socket = accept(server_socket, (sockaddr *) &cli_addr, &length);
                    if (tmp_cilent_socket > 0) {
                        //设置响应事件,设置可读和边缘(ET)模式
                        //很多人会把可写事件(EPOLLOUT)也注册了,后面会解释
                        tmp_server_epoll_ev.events = EPOLLIN | EPOLLET;
                        tmp_server_epoll_ev.data.fd = tmp_cilent_socket;
                        //设置连接为非阻塞模式
                        int flags = fcntl(tmp_cilent_socket, F_GETFL, 0);
                        if (flags < 0) 
                        {
                            std::cout << "set no block error, fd:" << tmp_cilent_socket << std::endl;
                            continue;
                        }
                        if (fcntl(tmp_cilent_socket, F_SETFL, flags | O_NONBLOCK) < 0) 
                        {
                            std::cout << "set no block error, fd:" << tmp_cilent_socket << std::endl;
                            continue;
                        }
                        //将新的连接添加到epoll中
                        epoll_ctl(server_epoll, EPOLL_CTL_ADD, tmp_cilent_socket, &tmp_server_epoll_ev);
                        std::cout << "client on line fd:" << tmp_cilent_socket << std::endl;
                    }
                }
            } 
            else 
            {//不是socket的响应事件
                //判断是不是断开和连接出错
                //因为连接断开和出错时,也会响应`EPOLLIN`事件
                if (server_epoll_ev[i].events & EPOLLERR || server_epoll_ev[i].events & EPOLLHUP) 
                {
                    //出错时,从epoll中删除对应的连接
                    //第一个是要操作的epoll的描述符
                    //因为是删除,所有event参数天null就可以了
                    epoll_ctl(server_epoll, EPOLL_CTL_DEL, server_epoll_ev[i].data.fd, nullptr);
                    std::cout << "client out fdxxx:" << server_epoll_ev[i].data.fd << std::endl;
                    close(server_epoll_ev[i].data.fd);
                } 
                else if (server_epoll_ev[i].events & EPOLLIN) 
                {//如果是可读事件
                    
                    char buff[1024] = {0};
                    //如果在windows中,读socket中的数据要用recv()函数
                    int len = read(server_epoll_ev[i].data.fd, buff, 1024);
                    //如果读取数据出错,关闭并从epoll中删除连接
                    if (len == -1) {
                        epoll_ctl(server_epoll, EPOLL_CTL_DEL, server_epoll_ev[i].data.fd, nullptr);
                        std::cout << "client out fd:" << server_epoll_ev[i].data.fd << std::endl;
                        close(server_epoll_ev[i].data.fd);
                    } 
                    else 
                    {
                        //正常读取,打印读到的数据
                        std::cout << buff << std::endl;
                        
                        //向客户端发数据
                        //char a[] = "123456";
                        //如果在windows中,向socket中写数据要用send()函数
                        //write(server_epoll_ev[i].data.fd, a, sizeof(a));
                    }
                }
            }
        }
    }
    */

    return 0;
}