#include <bits/stdc++.h>
using namespace std;

//#define output_nospace(x) cout << x << " "
// #define output(x) cout << x << endl

template<typename... Args>
void output(const Args&... args) {
    (cout << ... << args);  // left fold with << operator
}

/*

    Here ill define two things in this file one will be my HTTP request and response struct which will how my req and response should look
    like actually their format

*/

struct HttpRequest{

    // what all willit contains 
    string path;
    string method;
    map<string,string> header;
    string body;                        // this is if i have req as post 
    
    HttpRequest(){
        path = "", method = "", body = "";

    }

    void print(){
        output("Printing the http request\n");
        output("Path: ", path, "\n" );
        output("Method: ", method, "\n", "Body: ", body, "\n");

        for(auto i: header){
            cout << i.first << " : " << i.second;
            cout << endl;
        }
    }
};

struct HttpResponse{
    string body;
    int status_code;
    string status_msg;
    map<string, string> header;

    HttpResponse(){
        body = "";
        status_code = 200;
        status_msg = "OK";
        
        header["Content-Type"] = "text/html";
        header["Server"] = "My-CPP-Server";
    }

    // now functin to set body for the response 
    void setBody(string content){
        body = content;
        header["Content-Length"] = to_string(body.size());
    }

    // now this response our browser have to parse right so it should be invalid format 
    // for this u can google and chek the format for http request
    // so it like it accepts in first http keyword and status cdoe and status message 
    // on second line some headers would be there and then comes our body part
    string toString(){
        string result  = "";
        result += string("HTTP/1.1 ") + to_string(status_code) + " " + status_msg + "\r\n";

        for(auto i: header){
            result += i.first + ":" + i.second + "\r\n";
        }

        result += "\r\n" + body;
        return result;
    }

    void print(){
        // removed `header` from output(...) because map can't be streamed directly
        output("Status: ", status_code, " Status Message: ", status_msg, "\n", " Body", body, " \nHeaders\n");
        for(auto i: header) cout << i.first << ": " << i.second << endl;
    }

};

int main() {
    cout << "=== Testing Stage 1: Basic HTTP Classes ===\n" << endl;
    
    // Create a request 
    HttpRequest req;
    req.method = "GET";
    req.path = "/home";
    req.header["User-Agent"] = "Mozilla/5.0";
    req.header["Accept"] = "text/html";
    
    cout << "Sample HTTP Request:" << endl;
    req.print();
    cout << endl;
    
    // Create a response 
    HttpResponse res;
    res.setBody("<html><body><h1>Hello World!</h1></body></html>");
    
    cout << "Sample HTTP Response:" << endl;
    res.print();
    cout << endl;
    
    cout << "Raw HTTP Response (what browser receives):" << endl;
    cout << res.toString() << endl;
    
    return 0;
}
