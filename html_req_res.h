#ifndef HTTP_HPP
#define HTTP_HPP

#include <bits/stdc++.h>
using namespace std;

// Generic variadic print function
template<typename... Args>
void output(const Args&... args) {
    (cout << ... << args);  // left fold with << operator
}

/*
    HTTP Request and Response Structures
*/

struct HttpRequest {
    string path;
    string method;
    map<string,string> header;
    string body;   // for POST/PUT requests

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
