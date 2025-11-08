#include "html_req_res.h"

HttpRequest::HttpRequest() {
    path = "";
    method = "";
    body = "";
}

void HttpRequest::print() {
    output("Printing the http request\n");
    output("Path: ", path, "\n");
    output("Method: ", method, "\n", "Body: ", body, "\n");

    for (auto &i : header) {
        cout << i.first << " : " << i.second << endl;
    }
    for(auto& i: query){
        cout << i.first << " " << i.second << endl;
    }
}

HttpResponse::HttpResponse() {
    body = "";
    status_code = 200;
    status_msg = "OK";
    header["Content-Type"] = "text/html";
    header["Server"] = "My-CPP-Server";
}

void HttpResponse::setBody(string content) {
    body = content;
    header["Content-Length"] = to_string(body.size());
}

string HttpResponse::toString() {
    string result = "";
    result += string("HTTP/1.1 ") + to_string(status_code) + " " + status_msg + "\r\n";
    for (auto &i : header) {
        result += i.first + ": " + i.second + "\r\n";
    }
    result += "\r\n" + body;
    return result;
}

void HttpResponse::print() {
    output("Status: ", status_code, " Status Message: ", status_msg, "\n", " Body ", body, " \nHeaders\n");
    for (auto &i : header) cout << i.first << ": " << i.second << endl;
}
