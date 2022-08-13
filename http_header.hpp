#ifndef _http_header_
#define _http_header_
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <fstream>
#include <unistd.h>

using std::cout;
using std::endl;
using std::string;

class http_header
{
public:
    http_header(char* c_str);
    http_header();
    ~http_header();

    char* get_http_request_path_file();
    long* get_http_request_path_file_size();
    string get_http_server_header();
    int* get_http_server_header_size();

private:
    char* client_addr;
    int* client_addr_size;

    char* http_request;
    int* http_request_size;

    char* http_request_path;
    int* http_request_path_size;

    char* http_request_path_file_type;
    int* http_request_path_file_type_size;

    char* http_request_path_file;
    long* http_request_path_file_size;

private:
    string  HTTP_version_and_status;
    string  HTTP_server;
    string  HTTP_CL;
    string  HTTP_request_path_type;

    string  server_header;
    int* server_header_size;

private:
    char* http_request_filter(char* c_str);
    char* http_request_path_filter(char* c_str);
    char* http_request_path_file_type_filter(char* c_str);
    char* http_request_path_file_new();
    string http_server_header_new(); 
};

http_header::http_header(char* c_str)
{
    HTTP_version_and_status = "HTTP/1.1 200";
    HTTP_server = "server:simple web server";
    /*
    cout << endl << endl << endl;
    cout << http_request_filter(c_str) << endl
        << http_request_path_filter(c_str) << endl
        << http_request_path_file_type_filter(c_str) << endl
        << http_request_path_file_new() << endl;
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
    delete[](client_addr);
    delete(client_addr_size);

    delete[](http_request);
    delete(http_request_size);

    delete[](http_request_path);
    delete(http_request_path_size);

    delete[](http_request_path_file_type);
    delete(http_request_path_file_type_size);

    delete[](http_request_path_file);
    delete(http_request_path_file_size);

    delete(server_header_size);
}

char* http_header::http_request_filter(char* c_str)
{
    if (c_str[0] == 'G' || c_str[0] == 'g')
    {
        http_request_size = new int(4);
        http_request = new char[*http_request_size];
        strcpy(http_request, "GET");
    }
    else if(c_str[0] == 'P' || c_str[0] == 'p')
    {
        http_request_size = new int(5);
        http_request = new char[*http_request_size];
        strcpy(http_request, "POST");
    }

    return http_request;
}

char* http_header::http_request_path_filter(char* c_str)
 {
    http_request_path_size = new int(2);

    for (int i = *http_request_size; c_str[i] != '\n' && c_str[i] != '\r' && c_str[i] != ' '; ++i, ++(*http_request_path_size))

    //cout << *http_request_path_size << endl;

    if(*http_request_path_size > 3)
    {
        http_request_path = new char[*http_request_path_size];
        http_request_path[0] = '.';
        http_request_path[*http_request_path_size - 1] = '\0'; 
        for (int i = 0; i < *http_request_path_size - 1; ++i)
            http_request_path[i + 1] = c_str[(*http_request_size) + i];
    }
    else
    {
        *http_request_path_size = 13;
        http_request_path = new char[*http_request_path_size];
        strcpy(http_request_path, "./index.html");
    }
    
    return http_request_path;
 }

char* http_header::http_request_path_file_type_filter(char* c_str)
{
    int c_str_accept;
    for (int i = 0; i < strlen(c_str); ++i)
    {
        if ((c_str[i] == 'A' || c_str[i] == 'a') && c_str[i+1] == 'c')
            if (c_str[i+2] == 'c' && c_str[i+3] == 'e' && c_str[i+4] == 'p' && c_str[i+5] == 't' && c_str[i+6] == ':')
            {
                i+=8;
                c_str_accept = i;
                for(http_request_path_file_type_size = new int(1); c_str[i] != ','; ++i,++(*http_request_path_file_type_size));
            }
    }

    http_request_path_file_type = new char[*http_request_path_file_type_size];
    http_request_path_file_type[(*http_request_path_file_type_size)] = '\0';
    for(int i = 0; i < *http_request_path_file_type_size - 1; ++i)
        http_request_path_file_type[i] = c_str[i+c_str_accept];
    
    HTTP_request_path_type = http_request_path_file_type;
    cout << "\n\n\n" << http_request_path_file_type;
    cout << "\n\n\n" << HTTP_request_path_type << "\n\n\n";
    return http_request_path_file_type;
}

char* http_header::http_request_path_file_new()
{
    FILE* file = nullptr;

    http_request_path_file_size = new long;

    if (HTTP_request_path_type == "text/html" || HTTP_request_path_type == "text/css" || HTTP_request_path_type == "application/x-javascript")
    {
        if (file = fopen(http_request_path, "r"))
        {
            fseek(file, 0, 2);
            *http_request_path_file_size = ftell(file);
            http_request_path_file = new char[*http_request_path_file_size+1];
            fseek(file, -(*http_request_path_file_size), 2);
            /*
            if (*http_request_path_file_size+1 != fread(http_request_path_file, (*http_request_path_file_size+1), 1, file))
            {
                delete[](http_request_path_file);
                http_request_path_file = new char[4];
                strcpy(http_request_path_file, "404");
            }*/
            fread(http_request_path_file, *http_request_path_file_size, 1, file);
        
            fclose(file);
        }
        else
        {
            http_request_path_file = new char[4];
            strcpy(http_request_path_file, "404");
        }
    }
    else
    {
        if (file = fopen(http_request_path, "rb"))
        {
            fseek(file, 0, 2);
            *http_request_path_file_size = ftell(file);
            http_request_path_file = new char[*http_request_path_file_size+1];
            fseek(file, -(*http_request_path_file_size), 2);
            /*
            if (*http_request_path_file_size+1 != fread(http_request_path_file, (*http_request_path_file_size+1), 1, file))
            {
                delete[](http_request_path_file);
                http_request_path_file = new char[4];
                strcpy(http_request_path_file, "404");
            }*/
            fread(http_request_path_file, *http_request_path_file_size, 1, file);
        
            fclose(file);
        }
        else
        {
            http_request_path_file = new char[4];
            strcpy(http_request_path_file, "404");
        }   
    }
      

    std::stringstream tmp_int_sring;
    tmp_int_sring << *http_request_path_file_size;
    HTTP_CL = tmp_int_sring.str();
    tmp_int_sring.clear();

    return http_request_path_file;
}

char* http_header::get_http_request_path_file()
{
    return http_request_path_file;
}

long* http_header::get_http_request_path_file_size()
{
    return http_request_path_file_size;
}

string http_header::http_server_header_new()
{
    server_header = HTTP_version_and_status + '\n' +HTTP_server + '\n' + "Conten-length:" + HTTP_CL + '\n' + "Content-type:" + HTTP_request_path_type + "\n\n";
    server_header_size = new int (strlen(server_header.c_str()));

    return server_header;
}

string http_header::get_http_server_header()
{
    return server_header;
}

int* http_header::get_http_server_header_size()
{
    return server_header_size;
}


#endif