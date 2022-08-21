#ifndef _http_header_
#define _http_header_
#include <iostream>
//#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

class http_header
{
public:
    http_header(char* c_str, int tmp_client_header_size);
    http_header();
    ~http_header();

    char* get_http_request_path_file();
    long get_http_request_path_file_size();
    const char* get_http_server_header();
    int get_http_server_header_size();

private:
    int client_header_size;

    string http_request;
    int http_request_size;

    string http_request_path;
    int http_request_path_size;

    string http_request_path_file_type;
    int http_request_path_file_type_size;

    char* http_request_path_file;
    long http_request_path_file_size;

private:
    string  HTTP_version_and_status;
    string  HTTP_server;
    string  HTTP_CL;
    string  HTTP_request_path_type;

    string  server_header;
    int server_header_size;

private:
    const char* http_request_filter(char* c_str);
    const char* http_request_path_filter(char* c_str);
    const char* http_request_path_file_type_filter(char* c_str);
    const char* http_request_path_file_new();
    const char* http_server_header_new(); 
};

http_header::http_header(char* c_str, int tmp_client_header_size)
{
    client_header_size = tmp_client_header_size;
    HTTP_version_and_status = "HTTP/1.1 200";
    HTTP_server = "server:epoll web server";
    /*
    cout << endl << endl << endl;
    cout << http_request_filter(c_str) << endl
        << http_request_path_filter(c_str) << endl
        << http_request_path_file_type_filter(c_str) << endl
        << http_request_path_file_new() << endl
        << http_server_header_new() << endl;
    cout << endl << endl << endl;
    */
    http_request_filter(c_str);
    http_request_path_filter(c_str);
    http_request_path_file_type_filter(c_str);
    http_request_path_file_new();
    http_server_header_new();
    
}

http_header::http_header()
{
    exit(0);
}

http_header::~http_header()
{
    delete[](http_request_path_file);
}

const char* http_header::http_request_filter(char* c_str)
{
    if (c_str[0] == 'G' || c_str[0] == 'g')
    {
        this->http_request = {"GET"};
        this->http_request_size = http_request.size();
    }
    else if(c_str[0] == 'P' || c_str[0] == 'p')
    {
        this->http_request = {"POST"};
        this->http_request_size = http_request.size();
    }

    return http_request.c_str();
}

const char* http_header::http_request_path_filter(char* c_str)
 {
    http_request_path.push_back('.');
    for (int i = http_request_size + 1; c_str[i] != '\n' && c_str[i] != '\r' && c_str[i] != ' '; ++i)
        http_request_path.push_back(c_str[i]);

    

    if (http_request_path == "./")
        http_request_path += "index.html";

    http_request_path_size = http_request_path.size();
    return http_request_path.c_str();
 }

const char* http_header::http_request_path_file_type_filter(char* c_str)
{
    for (int i = 0; i < client_header_size; ++i)
    {
        if ((c_str[i] == 'A' || c_str[i] == 'a') && c_str[i+1] == 'c')
            if (c_str[i+2] == 'c' && c_str[i+3] == 'e' && c_str[i+4] == 'p' && c_str[i+5] == 't' && c_str[i+6] == ':')
            {
                i+=7;
                for(; c_str[i] != ','; ++i, http_request_path_file_type.push_back(c_str[i]));
            }
    }
    http_request_path_file_type.pop_back();
    http_request_path_file_type_size = http_request_path_file_type.size();
    //HTTP_request_path_type = http_request_path_file_type;

    return http_request_path_file_type.c_str();
}

const char* http_header::http_request_path_file_new()
{
    FILE* file = nullptr;

    if (HTTP_request_path_type == "text/html" || HTTP_request_path_type == "text/css" || HTTP_request_path_type == "application/x-javascript")
        file = fopen(http_request_path.c_str(), "r");
    else
        file = fopen(http_request_path.c_str(), "rb");


    if (file != nullptr)
    {
        fseek(file, 0, 2);
        http_request_path_file_size = ftell(file);
        http_request_path_file = new char[http_request_path_file_size+1];
        fseek(file, -(http_request_path_file_size), 2);

        fread(http_request_path_file, http_request_path_file_size, 1, file);
    
        fclose(file);
    }
    else
    {
        http_request_path_file = new char[4]{'4', '0', '4', '\0'};
    }
        
      

    std::stringstream tmp_int_sring;
    tmp_int_sring << http_request_path_file_size;
    HTTP_CL = tmp_int_sring.str();
    tmp_int_sring.clear();

    return http_request_path_file;
}

char* http_header::get_http_request_path_file()
{
    return http_request_path_file;
}

long http_header::get_http_request_path_file_size()
{
    return http_request_path_file_size;
}



const char* http_header::http_server_header_new()
{
    server_header = HTTP_version_and_status;
    server_header += '\n' +HTTP_server;
    server_header += '\n';
    server_header += "Conten-length:";
    server_header += HTTP_CL;
    server_header += '\n';
    server_header += "Content-Type:";
    server_header += http_request_path_file_type;
    server_header += "\n\n";
    server_header_size = server_header.size();

    return server_header.c_str();
}

const char* http_header::get_http_server_header()
{
    return server_header.c_str();
}

int http_header::get_http_server_header_size()
{
    return server_header_size;
}


#endif