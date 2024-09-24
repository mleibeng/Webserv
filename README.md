
**1. Application Layer Protocols**
The application layer is where communication between web clients (e.g., browsers) and web servers happens, using the following protocols:
- **HTTP (Hypertext Transfer Protocol)**:
  - A protocol for transferring web pages and resources in an unencrypted manner.
  - Default port: 80.
- **HTTPS (Hypertext Transfer Protocol Secure)**:
  - The secure version of HTTP, using SSL/TLS encryption to protect data in transit.
  - Default port: 443.

**2. Transport Layer Protocols**
The transport layer is responsible for ensuring reliable or unreliable data delivery between systems.
- **TCP (Transmission Control Protocol)**:
  - Provides reliable, ordered, and error-checked delivery of data between applications. Most web traffic (HTTP/HTTPS) uses TCP.
- **UDP (User Datagram Protocol)**:
  - A connectionless protocol that allows for fast but unreliable transmission. It's used in situations where speed is more critical than reliability (e.g., live video streaming).

**3. HTTP (Hypertext Transfer Protocol) Basics**
HTTP is the protocol used for communication between web browsers and servers. It follows a client-server model and consists of requests and responses.

**HTTP Request**
- A client sends an HTTP request to the server, typically to retrieve or modify resources.
Example HTTP Request (GET):
GET /index.html HTTP/1.1
Host: www.example.com

Parts of the HTTP Request:
1. Method: Specifies the action to be performed. Common methods include:
   - GET: Request a resource.
   - POST: Submit data to the server.
   - DELETE: Delete a resource.
2. Path: The specific location of the resource on the server (e.g., `/index.html`).
3. Protocol/Version: Indicates the HTTP version being used (e.g., HTTP/1.1).
4. Headers: Additional information about the request, sent as key-value pairs (e.g., `Host`, `User-Agent`, `Content-Type`).

**HTTP Response**
- The server responds with an HTTP response message after processing the client's request.
Example HTTP Response:
HTTP/1.1 200 OK
Content-Type: text/html
Content-Length: 1234
<html>
  <body>...</body>
</html>

Parts of the HTTP Response:
1. Status Line: Indicates the HTTP version, status code, and status message.
   - Example: `HTTP/1.1 200 OK`
   - Common status codes:
     - 200: OK (successful request).
     - 404: Not Found (resource not found).
     - 500: Internal Server Error (server-side issue).
2. Headers: Metadata about the response (e.g., `Content-Type`, `Content-Length`).
3. Body: The actual content of the response (HTML, JSON, etc.).

**4. URL Structure and Port Usage**
A URL (Uniform Resource Locator) has different parts that determine the protocol, server, and port being accessed.
Example: `http://127.0.0.1:80`
- Part 1: Protocol (e.g., HTTP, HTTPS): Specifies the protocol used to communicate with the server.
- Part 2: IP Address (e.g., 127.0.0.1): The server's address, which can be an IP or domain name.
- Part 3: Port (e.g., 80, 443): Defines the port the server is listening on. Standard ports:
  - HTTP: Port 80 (unencrypted).
  - HTTPS: Port 443 (encrypted).

**5. Telnet (For Testing Purposes)**
Telnet is a simple command-line tool for testing basic network services. Though largely deprecated, it allows testing by connecting directly to servers via TCP.
- Usage:
  telnet <hostname> <port>
  Example:
  telnet 127.0.0.1 80
  - Telnet can simulate raw HTTP requests to a server for testing purposes.
- Alternatives: Tools like curl or netcat are more commonly used today for testing specific network services.

**6. Additional Notes**
- HTML (Hypertext Markup Language):
  - Not a protocol, but the language used to structure web content that is sent by servers to browsers. Web servers often deliver HTML files as part of the HTTP response.

- Ports and Encryption:
  - http://127.0.0.1:80: Unencrypted HTTP communication over port 80.
  - https://127.0.0.1:443: Encrypted HTTPS communication over port 443.

---

https://www.youtube.com/watch?v=9J1nJOivdyw
