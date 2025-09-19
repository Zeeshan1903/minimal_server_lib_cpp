#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

using namespace std;

#include "html_builder.h"
#include "html_req_res.h"

class SimpleServer {
public:
    int port;
    int server_socket;
    bool is_running;
    map<string, function<void(HttpRequest&, HttpResponse&)>> routes;

    SimpleServer(int port) {
        this->port = port;
        is_running = false;
    }

    ~SimpleServer() {
        stop();
    }

    void stop() {
        if (is_running) {
            is_running = false;
            close(server_socket);
            cout << "Server stopped." << endl;
        }
    }

    //this is actually intertesitng befcause as we know how would i tell this so i use golang approach which do what is add a handler or register handler 
    //for like kind of callbackn= and call this whenever a req is made to that route    
    void addRoute(string path, function<void(HttpRequest&, HttpResponse&)> handler) {
        routes[path] = handler;
        cout << "Path added " << path << endl;
    }

    bool start() {
        cout << "Starting server boom....\n";

        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        // now here we have to made a struct format!!! of how socket programming works in cpp/c 
        struct sockaddr_in server_info;
        server_info.sin_addr.s_addr = INADDR_ANY;
        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(port);

        bind(server_socket, (struct sockaddr*)&server_info, sizeof(server_info)) ;
            // perror("Bind failed");
            // return false;
        // }

        if (listen(server_socket, 10) < 0) {
            perror("Listen failed");
            return false;
        }

        cout << "Server started mouja mouja\n";
        is_running = true;
        return true;
    }

    void run() {
        if (!is_running) {
            cout << "Server not started!" << endl;
            return;
        }

        while (is_running) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);

            int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            if (client_socket < 0) {
                if (is_running) {
                    cout << "Error accepting connection" << endl;
                }
                continue;
            }

            handleClient(client_socket);
            close(client_socket);
        }
    }

        //just here handling allt eh clinet 
    void handleClient(int client) {
        char buffer[5000];
        int bytes = recv(client, buffer, sizeof(buffer) - 1, 0);

        if (bytes <= 0) return;

        buffer[bytes] = '\0';

        string raw_request(buffer);
        HttpRequest request = parseRequest(raw_request);
        HttpResponse response;

        if (routes.find(request.path) == routes.end()) {
            response.status_code = 404;
            response.status_msg = "Not Found";
            response.setBody("<html><body><h1>404 - Page Not Found</h1></body></html>");
        } else {
            routes[request.path](request, response);
        }

        string response_str = response.toString();
        send(client, response_str.c_str(), response_str.length(), 0);
    }




    void extract_post_body(HttpRequest& request, istringstream& iss) {
        string line;

        // here our goal for now is only extractign the content type so in future we can use this concept 
        // and can implement how to parse body as well which we have implemented 
        while (getline(iss, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty()) break; 

            size_t pos = line.find(':');
            if (pos != string::npos) {
                string key = line.substr(0, pos);
                string val = line.substr(pos + 1);
                if (!val.empty() && val[0] == ' ') val.erase(0, 1); 
                if (key == "Content-Type") {
                    request.header["Content-Type"] = val;
                }
            }
        }

        string body;
        getline(iss, body, '\0');
        request.body = body;
    }

    // This was the extraction of first line of the query if the request if get and if we want to extract all the query parameter

    void extract_query(string query, HttpRequest& request){
        //my query will look like ==> /<path>?<query> 
        istringstream iss(query);

        vector<string> result;
        string token;
        while(getline(iss,token, '?')){
            result.push_back(token);
        }

        if(result.empty()){
            cerr << "Invalid request while parsing\n";
            return;
        }
        // now first line of result will contain path
        request.path = result.front();
        result.pop_back();

        //now other parameter would be my what query praameter
        token = result.front();
        result.clear();
        istringstream iss_token(token);
        token = "";
        while(getline(iss_token, token, '&')){
            //here each token would be of type a=b and now we have to add this entry to map of request 
            string a,b;
            vector<string> temp;
            istringstream isss(token);
            while(getline(isss,a,'=')){
                temp.push_back(a);
            }
            a = temp.front(), b = temp.back();
            request.query[a] = b;                       // this is i have added the query to my map
        }

        return;

    }





    HttpRequest parseRequest(const string& raw_request) {
        // HttpRequest request;
        // istringstream stream(raw_request);
        // string line;

        // if (getline(stream, line)) {
        //     istringstream first_line(line);
        //     string version;
        //     first_line >> request.method >> request.path >> version;
        //     // cout << first_line << "\n";
        
        // }
        // cout << "Here in parse request function and i don't even know wtf this function do\n";
        // return request;


        /*
        POST /submit HTTP/1.1\r\n
                Host: localhost:8080\r\n
                Content-Type: application/x-www-form-urlencoded\r\n
                Content-Length: 18\r\n
                \r\n
                name=zeeshan&age=21

        GET /index.html?name=zeeshan&age=21 HTTP/1.1\r\n
                Host: localhost:8080\r\n
                User-Agent: curl/7.68.0\r\n
                Accept: */
                // \r\n
        




     

        HttpRequest request;
        istringstream iss(raw_request);                //this would kind of contain the whole string 

        // now extract first_line
        string first_line;
        //now i got first line and this will contain my what query parameters and my path and tyupe
        getline(iss, first_line, '\n');

        istringstream extract_first_iss(first_line);
        string query_line;
        extract_first_iss >> request.method >> query_line;

        //now ill write a fn to extract path and query_parameter
        if(request.method == "GET")extract_query(query_line, request);
        else if(request.method == "POST"){
            //well do something here 
            request.path = query_line;          //here this line will now only contains our path

            //get body
            extract_post_body(request, iss);                //used for extracting the body of the poist request 

        }

        cout << "Request type --> " << request.method << "\nRequest path --> " << request.path << "\nrequest boyd --> " << request.body << endl;
        return request;
    }
};

int main() {
    cout << "=== Testing Stage 3: HTTP Server ===\n" << endl;

    SimpleServer server(8080);

    server.addRoute("/", [](HttpRequest& req, HttpResponse& res) {
        HtmlBuild page;
        string html = page
            .start()
            .title("C++ Web Server")
            .startBody()
            .heading("Hello from C++!")
            .para("This page is served by our custom C++ web server.")
            .link("/about", "About Page")
            .endDocument()
            .build();
        res.setBody(html);
    });

    server.addRoute("/about", [](HttpRequest& req, HttpResponse& res) {
        HtmlBuild page;
        string html = page
            .start()
            .title("About - C++ Web Server")
            .startBody()
            .heading("About This Server")
            .para("This server is built from scratch in C++!")
            .link("/", "Back to Home")
            .endDocument()
            .build();
        res.setBody(html);
    });

    if (server.start()) {
        server.run();
    }

    return 0;
}
