# Webserv 🌐

**Webserv** is a lightweight HTTP server built in C++. The challenge of this project was to implement core HTTP functionalities and create a fully functional web server, adhering to the HTTP/1.1 specification.

![Enregistrement de l’écran 2024-12-18 à 22 29 25 (1)](https://github.com/user-attachments/assets/0c2f23c9-58b5-4f12-9500-8541e3dc2df6)


## 📋 Table of Contents

- [Introduction](#introduction)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [Testing](#testing)
- [Technical Details](#technical-details)
- [Author](#author)

---

## Introduction

This project was a deep dive into the inner workings of how data is shared and communicated over the internet we rely on every day. We developed a fully functional web server that handles HTTP requests, serves static files, executes CGI scripts, and efficiently manages multiple client connections simultaneously. Through this project, I gained valuable insights into the foundational principles of web servers and network communication.

This project emphasizes:
- **Network programming**.
- **Concurrency management**.
- **Protocol compliance**.

### Goals

1. Understand and implement the HTTP/1.1 protocol.
2. Handle multiple client connections using non-blocking I/O.
3. Manage server configurations dynamically through a configuration file.

---

## Features

- 🖥️ **Static File Serving**: Serve HTML, CSS, JS, or any static content.
- 🧩 **CGI Support**: Run dynamic scripts (e.g., PHP, Python).
- 📑 **Custom Configuration**: Define routes, error pages, and limits.
- 📡 **Persistent Connections**: Handle `keep-alive` connections.
- 🔄 **HTTP Methods**: Support for `GET`, `POST`, and `DELETE`.
- 🔥 **Error Handling**: Customizable error pages for HTTP status codes.

---

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/asut00/webserv.git
   cd webserv
   ```

2. **Compile the server**:
   ```bash
   make
   ```

   This will create the `webserv` executable.

---

## Usage

To start the server, provide a configuration file as an argument:

```bash
./webserv ./config_file/default.conf
```

### Logs:

The server logs incoming requests, responses, and errors to the console.

---

## Configuration

The server is configured through `.conf` files written in a custom format. Below is an example configuration:

```plaintext
server {
    listen 8080;
    server_name localhost;

    root /var/www/html;
    index index.html;

    error_page 404 /errors/404.html;

    location /cgi-bin {
        cgi_pass /usr/bin/python3;
    }

    client_max_body_size 10M;
}
```

### Key Directives:

- **`listen`**: Port number for the server to bind to.
- **`server_name`**: Hostname for virtual hosting.
- **`root`**: Root directory for serving files.
- **`index`**: Default file to serve in a directory.
- **`error_page`**: Custom error pages for specific HTTP status codes.
- **`cgi_pass`**: Path to the CGI interpreter for dynamic scripts.
- **`client_max_body_size`**: Maximum size for incoming requests.

---

## Testing

### Basic Tests:

1. Use `curl` to send HTTP requests:
   ```bash
   curl -X GET http://localhost:8081
   curl -X POST -d "data=example" http://localhost:8081
   ```

2. Test the server with its static website:
   - If you use the `./config_file/default.conf` config file, the `http://localhost:8081` URL will redirect you to a static website.
   - This website allows any user to test the server's different features.

3. Test error handling:
   - Request a non-existent file to trigger a `404` error.

### Load Testing:

Use tools like [siege](https://github.com/JoeDog/siege), [ApacheBench](https://httpd.apache.org/docs/2.4/programs/ab.html) or [wrk](https://github.com/wg/wrk) to stress test the server:
```bash
siege -b http://localhost:8081
```

```bash
ab -n 1000 -c 10 http://localhost:8081/
```

---

## Technical Details

### Core Components:

1. **Socket Programming**:
   - Utilizes `select()` for handling multiple clients.
   - Manages incoming and outgoing connections efficiently.

2. **Request Parsing**:
   - Parses HTTP headers, methods, and body.
   - Supports chunked encoding and multipart form data.

3. **Response Generation**:
   - Generates HTTP responses with appropriate headers and content.
   - Includes support for `Content-Length` and `Content-Type`.

4. **Concurrency**:
   - Handles multiple clients simultaneously.
   - Ensures data integrity and non-blocking operations.

5. **CGI Integration**:
   - Executes scripts in isolated processes.
   - Sends the request body and environment variables to the script.

---

## Team

- [@atwazar](https://github.com/atwazar)
- [@asut00](https://github.com/asut00)  

---
