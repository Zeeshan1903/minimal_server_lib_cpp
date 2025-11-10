# Web C++ Minimal HTTP Server (Educational)

This repository implements a small, educational HTTP server library in C++ for learning TCP sockets, basic HTTP request/response handling, simple routing, and a tiny chainable HTML builder.


---

## Project layout (important files under src/)

- src/server.cpp — main server implementation, routing and request handling
- src/html_req_res.h / src/html_req_res.cpp — simple HttpRequest / HttpResponse structs and helpers
- src/html_builder.h — chainable HTML builder used to compose response HTML
- src/Makefile — convenience build/run (if present)
- src/testing.cpp — scratch/testing file (may be incomplete)


---

## What this project does

- Listens on a TCP port (default 8080) using POSIX sockets.
- Reads raw HTTP requests (minimal parsing of request line, headers, and body).
- Provides a routes map (path -> handler) where handlers are functions of signature:
  void handler(HttpRequest& req, HttpResponse& res)
- Uses a small chainable HtmlBuild API to construct simple HTML pages.
- Sends back an HttpResponse (headers + body) as a raw HTTP response buffer.

This is intentionally minimal and educational — it demonstrates the end-to-end pipeline: accept TCP -> parse HTTP -> route -> build response -> send.

---

## How it works (flow)

1. Start server (SimpleServer::start) — create socket, bind, listen.
2. Accept loop (SimpleServer::run) — accept connections and call handleClient.
3. parseRequest — parse request line, headers, extract GET query string or POST body (basic).
4. Route dispatch — call registered handler or return 404.
5. Handlers build content (often using HtmlBuild) and set body on HttpResponse.
6. Server sends HttpResponse::toString() over the socket.

---

## Example route registration (from main in src/server.cpp)

```cpp
server.addRoute("/", [](HttpRequest& req, HttpResponse& res) {
    HtmlBuild page;
    std::string html = page
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
```

Add routes similarly for POST endpoints or parameterized handlers.

---

## Build & run

From repository root:

Option A — Makefile 
```sh
cd src
make run
```

Option B — manual compile
```sh
cd src
g++ -std=c++17 -Wall -O2 server.cpp html_req_res.cpp -o cpp_server
./cpp_server
```
Default listen port is 8080 (see main in src/server.cpp). Adjust compilation units if your build requires additional .cpp files.

---

## Try it with curl

- GET root:
```sh
curl -v http://localhost:8080/
```

- GET with query:
```sh
curl -v "http://localhost:8080/index?name=zeeshan&age=21"
```

- POST (raw body):
```sh
curl -v -X POST http://localhost:8080/submit -d "name=zeeshan&age=21" -H "Content-Type: application/x-www-form-urlencoded"
```
(Add a `/submit` handler in main to test POST parsing.)

---

## Design notes & limitations

- Request parsing is intentionally minimal:
  - Basic request-line parsing and header extraction.
  - Query parsing is not URL-decoded.
  - POST body extraction is basic; does not support chunked encoding or multi-part.
- HtmlBuild is a tiny helper for demos — not a full HTML templating engine.
- No thread pool or async IO — server is single-threaded and blocks per connection.
- Header parsing currently extracts minimal fields (e.g., Content-Type) only.
- Testing and edge-case handling are limited; see TODOs below.

---

## Suggested improvements / TODOs

- Implement a proper HTTP parser (robust header parsing, version, and method handling).
- Add URL-decoding and robust query-string parsing.
- Support serving static files with proper Content-Type detection.
- Add concurrency (threads or async IO) to handle multiple clients.
- Improve HtmlBuild (valid heading tags, attributes, escaping).
- Add unit tests for parsing and response formatting.
- Clean up Makefile to compile only .cpp files (avoid compiling headers directly).
- Remove or fix src/testing.cpp if it contains invalid or incomplete code.

---

