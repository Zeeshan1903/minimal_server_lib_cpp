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

    void addRoute(string path, function<void(HttpRequest&, HttpResponse&)> handler) {
        routes[path] = handler;
        cout << "Path added " << path << endl;
    }

    bool start() {
        cout << "Starting server boom....\n";

        server_socket = socket(AF_INET, SOCK_STREAM, 0);
        int opt = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(opt));

        struct sockaddr_in server_info;
        server_info.sin_addr.s_addr = INADDR_ANY;
        server_info.sin_family = AF_INET;
        server_info.sin_port = htons(port);

        if (bind(server_socket, (struct sockaddr*)&server_info, sizeof(server_info)) < 0) {
            perror("Bind failed");
            return false;
        }

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

    HttpRequest parseRequest(const string& raw_request) {
        HttpRequest request;
        istringstream stream(raw_request);
        string line;

        if (getline(stream, line)) {
            istringstream first_line(line);
            string version;
            first_line >> request.method >> request.path >> version;
        }
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
