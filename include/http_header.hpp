#ifndef _http_header_
#define _http_header_
#include <iostream>
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
    http_header(char* header_cstr, int tmp_header_size);
    http_header(char* header_cstr, int tmp_header_size, const char* www_path);
    http_header();
    ~http_header();

//client header
private:
    int header_size;                //接收的header的大小
    string path_file_name;          //设置的网页文件路径
    int i_ptr;                      //第一行的大小

    string c_header_request;        //请求方式
    string c_header_path;           //请求文件路径
    string c_header_HTTP_version;   //HTTP版本
    string c_header_Host;           //Host路径
    string c_header_User_Agent;     //User-Agent
    string c_header_Accept;         //文件类型

    char* c_header_file;            //被请求文件的内容
    long c_header_file_size;        //被请求文件内容的大小

private:
    const char* set_c_header_request(char*);
    const char* set_c_header_path(char*);
    const char* set_c_header_HTTP_version(char*);
    const char* set_c_header_Host(char*);
    const char* set_c_header_User_Agent(char*);
    const char* set_c_header_Accept(char*);
    const char* set_c_header_file(const char*);

public:
    //const char* get_c_header_request() { return c_header_request.c_str(); }
    //const char* get_c_header_path() { return c_header_path.c_str(); }
    //const char* get_c_header_HTTP_version() { return c_header_HTTP_version.c_str(); }
    //const char* get_c_header_Host() { return c_header_Host.c_str(); }
    //const char* get_c_header_User_Agent() { return c_header_User_Agent.c_str(); }
    //const char* get_c_header_Accept() { return c_header_Accept.c_str(); }
    const char* get_c_header_file() { return c_header_file; }
    const long get_c_header_file_size() { return c_header_file_size; }


//server header
private:   
    string  s_header;

private:
    const char* set_s_header_main();
    const char* set_s_header_control_type_charset();
    const char* set_s_header_date();

public:
    const char* get_s_header() { return s_header.c_str(); }
    const int get_s_header_size() { return s_header.size(); }
};

http_header::http_header(char* header_cstr, int tmp_header_size) : header_size(tmp_header_size)
{
    set_c_header_request(header_cstr);
    set_c_header_path(header_cstr);
    set_c_header_HTTP_version(header_cstr);
    set_c_header_Host(header_cstr);
    set_c_header_Accept(header_cstr);
    set_c_header_User_Agent(header_cstr);
    set_c_header_file(c_header_path.c_str());

    set_s_header_main();
    set_s_header_control_type_charset();
}

http_header::http_header(char* header_cstr, int tmp_header_size, const char* www_path) : header_size(tmp_header_size)
{
    path_file_name = www_path;
    if(path_file_name.size() >= 1 && path_file_name[path_file_name.size() -1] == '/')
        path_file_name.pop_back();


    /*
    cout << set_c_header_request(header_cstr) << endl
        << set_c_header_path(header_cstr) << endl
        << set_c_header_HTTP_version(header_cstr) << endl
        << set_c_header_Host(header_cstr) << endl
        << set_c_header_Accept(header_cstr) << endl
        << set_c_header_User_Agent(header_cstr) << endl
        << set_c_header_file(c_header_path.c_str()) << endl
        << set_s_header_main() << endl
        << set_s_header_control_type_charset() << endl;
    */
    set_c_header_request(header_cstr);
    set_c_header_path(header_cstr);
    set_c_header_HTTP_version(header_cstr);
    set_c_header_Host(header_cstr);
    set_c_header_Accept(header_cstr);
    set_c_header_User_Agent(header_cstr);
    set_c_header_file(c_header_path.c_str());
    set_s_header_main();
    set_s_header_control_type_charset();;
}

http_header::http_header()
{
    exit(0);
}

http_header::~http_header()
{
    delete[](c_header_file);
}


//提取请求方式
const char* http_header::set_c_header_request(char* header)
{
    if (header[0] == 'G')
        c_header_request += "GET";
    else if (header[0] == 'P')
        c_header_request += "POST";

    i_ptr = c_header_request.size() +1;

    return c_header_request.c_str();
}

//提取请求文件路径
const char* http_header::set_c_header_path(char* header)
{
    for (i_ptr; header[i_ptr] != ' ' && header[i_ptr] != '\n' && header[i_ptr] != '\r'; ++i_ptr)
    {
        c_header_path.push_back(header[i_ptr]);
    }

    ++i_ptr;

    return c_header_path.c_str();
}

//提前HTTP协议
const char* http_header::set_c_header_HTTP_version(char* header)
{
    if (header[i_ptr +8] == '\n' || header[i_ptr +8] == '\r')
    {
        for (int o = 0; o < 8; ++o)
            c_header_HTTP_version.push_back(header[i_ptr+o]);
        
        i_ptr += c_header_HTTP_version.size() +1;
    }
    else
    {
        c_header_HTTP_version = "HTTP/1.0";
        i_ptr += 9;
    }
        
    

    return c_header_HTTP_version.c_str();
}

//提取Host
const char* http_header::set_c_header_Host(char* header)
{
    for (int i = i_ptr; i < header_size; ++i)
    {
        if (header[i] == 'H' && header[i +1] == 'o' && header[i +2] == 's' && header[i +3] == 't')
        {
            i += 6;
            for (; header[i] != ' ' && header[i] != '\n' && header[i] != '\r'; ++i)
            {
                c_header_Host.push_back(header[i]);
            }

            i = header_size;
        }
    }

    return c_header_Host.c_str();
}

//提取User-Agent
const char* http_header::set_c_header_User_Agent(char* header)
{
    for (int i = i_ptr; i < header_size; ++i)
    {
        if (header[i] == 'U' && header[i +1] == 's' && header[i +2] == 'e' && header[i +3] == 'r' && header[i +4] == '-' && header[i +5] == 'A' && header[i +6] == 'g' && header[i +7] == 'e' && header[i +8] == 'n' && header[i +9] == 't')
        {
            i += 12;
            for (; header[i] != '\n' && header[i] != '\r'; ++i)
            {
                c_header_User_Agent.push_back(header[i]);
            }

            i = header_size;
        }
    }

    return c_header_User_Agent.c_str();
}

//提取文件格式
const char* http_header::set_c_header_Accept(char* header)
{
    for (int i = i_ptr; i < header_size; ++i)
    {
        if (header[i] == 'A' && header[i +1] == 'c' && header[i +2] == 'c' && header[i +3] == 'e' && header[i +4] == 'p' && header[i +5] == 't')
        {
            i += 8;
            for (; header[i] != ','; ++i)
            {
                c_header_Accept.push_back(header[i]);
            }

            i = header_size;
        }
    }

    return c_header_Accept.c_str();
}

//提取文件内容
const char* http_header::set_c_header_file(const char* file_name)
{
    if(c_header_path == "/")
        c_header_path += "index.html";

    if(path_file_name.size() <= 1)
        path_file_name = ".";

    path_file_name += c_header_path;


    FILE* file = nullptr;

    if (c_header_Accept == "text/html" || c_header_Accept == "text/css" || c_header_Accept == "application/x-javascript")
        file = fopen(path_file_name.c_str(), "r");
    else
        file = fopen(path_file_name.c_str(), "rb");


    if (file != nullptr)
    {
        fseek(file, 0, 2);
        c_header_file_size = ftell(file);
        c_header_file = new char[c_header_file_size+1];
        fseek(file, -(c_header_file_size), 2);

        fread(c_header_file, c_header_file_size, 1, file);
    
        fclose(file);
    }
    else
    {
        c_header_file_size = 4;
        c_header_file = new char[c_header_file_size]{'4', '0', '4', '\0'};
    }

    return c_header_file;
}

//设置返回的header的内容 (HTTP版本, 请求状态)
const char* http_header::set_s_header_main()
{
    s_header += c_header_HTTP_version + " 200\n";
    return s_header.c_str();
}

//设置返回的header的内容 (文件大小, 文件类型)
const char* http_header::set_s_header_control_type_charset()
{
    s_header += "Cache-control: " + std::to_string(c_header_file_size) + "\nContent-Type: " + c_header_Accept + "\n\n";
    return s_header.c_str();
}

#endif