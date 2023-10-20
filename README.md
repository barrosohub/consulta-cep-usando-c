# Address Finder

This project allows you to enter a Brazilian zip code and retrieves the corresponding address information by interacting with the `viacep.com.br` API. It is written in C and demonstrates basic socket programming and HTTP request handling.

## Compilation

To compile the program, use the following command:

```bash
gcc -o AddressFinder main.c
```

## Usage
Run the compiled binary, then enter a Brazilian zip code when prompted:
```bash
./AddressFinder
```

## Code Overview
Here is a detailed flowchart of the program's logic, illustrated using Mermaid syntax:
```mermaid
graph TD;
    A[Start: Program Execution] --> B[main: Prompt User for Zip Code]
    B --> C[get_address_from_zip_code: Pass Zip Code as Argument]
    C --> D["Initialize addrinfo hints structure\nmemset(&hints, 0, sizeof(hints))"]
    D --> E["Set hints for IPv4 or IPv6 use\nhints.ai_family = AF_UNSPEC"]
    E --> F["Set hints for TCP protocol\nhints.ai_socktype = SOCK_STREAM"]
    F --> G["Call getaddrinfo\ngetaddrinfo('viacep.com.br', '80', &hints, &res)"]
    G -->|Success| H[create_socket_and_connect: Create Socket and Connect to Server]
    G -->|Failure| I["Error: getaddrinfo\nperror('getaddrinfo'); exit(EXIT_FAILURE)"]
    H --> J["send_request: Compose and Send HTTP Request\nsprintf(request, 'GET /ws/%s/json/ HTTP/1.1\r\nHost: viacep.com.br\r\nConnection: close\r\n\r\n', zip); send(sockfd, request, strlen(request), 0)"]
    J --> K["initialize_string: Initialize String Structure\ns->len = 0; s->ptr = malloc(s->len + 1)"]
    K --> L["receive_response: Receive Data\nwhile ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0)"]
    L -->|Data Received| M["append_to_string: Append Data to String\nmemcpy(s->ptr + s->len, buffer, size); s->ptr[new_len] = '\0'; s->len = new_len"]
    L -->|Failure| N["Error: recv\nperror('recv'); exit(EXIT_FAILURE)"]
    M --> O["Display Response\nprintf('%s\n', s.ptr)"]
    O --> P["Free Memory\nfree(s.ptr)"]
    P --> Q["Close Socket\nclose(sockfd)"]
    Q --> R["Free addrinfo Structure\nfreeaddrinfo(res)"]
    R --> S[End]
    I --> S
    N --> S
```

### Sequence Diagram
```mermaid
sequenceDiagram
    participant User
    participant Main
    participant get_address_from_zip_code
    participant create_socket_and_connect
    participant send_request
    participant initialize_string
    participant receive_response
    participant append_to_string

    User->>Main: Enter Zip Code
    Main->>get_address_from_zip_code: Pass Zip Code
    Note over get_address_from_zip_code: Initialize addrinfo hints structure
    Note over get_address_from_zip_code: Set hints for IPv4 or IPv6 use
    Note over get_address_from_zip_code: Set hints for TCP protocol
    Note over get_address_from_zip_code: Call getaddrinfo
    get_address_from_zip_code->>create_socket_and_connect: Create Socket and Connect to Server
    create_socket_and_connect->>get_address_from_zip_code: Return Socket Descriptor
    get_address_from_zip_code->>send_request: Compose and Send HTTP Request
    send_request->>get_address_from_zip_code: Request Sent
    get_address_from_zip_code->>initialize_string: Initialize String Structure
    initialize_string->>get_address_from_zip_code: String Initialized
    get_address_from_zip_code->>receive_response: Receive Data
    receive_response->>append_to_string: Append Data to String
    append_to_string->>receive_response: Data Appended
    receive_response->>get_address_from_zip_code: All Data Received
    get_address_from_zip_code->>Main: Return Response
    Main->>User: Display Response
```

