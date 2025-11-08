#ifndef HTTP_HPP
#define HTTP_HPP

#include <bits/stdc++.h>
using namespace std;

template<typename... Args>
void output(const Args&... args) {
    (cout << ... << args); 
}

/*
    HTTP Request and Response Structures
*/

struct HttpRequest {
    string path;
    string method;
    map<string,string> header;
    map<string,string> query;
    string body;   

    HttpRequest();

    void print();
};

struct HttpResponse {
    string body;
    int status_code;
    string status_msg;
    map<string, string> header;

    HttpResponse();

    void setBody(string content);
    string toString();
    void print();
};

#endif // HTTP_HPP
