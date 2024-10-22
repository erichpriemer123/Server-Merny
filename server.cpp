#include <iostream>
#include <cstring>

#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>




int main(){

    // Create a socket
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);

    // check if there is error opening socket
    if (server_socket < 0) {
        std::cerr << "Error creating socket\n";
        return -1;
    }
    else{
        std::cout << "success. Socket Created\n";
    }


    // Describes an IPv4 Internet domain socket address.
    // members stored as network byte order
    struct sockaddr_in server_addr;

    const int PORT = 8080;
    const char* IP_addr = "127.0.0.1";

    //zero out struct 
    memset((char *)&server_addr, 0, sizeof(server_addr));

    // AF_INET = IPV4
    server_addr.sin_family = AF_INET;
    
    //port number
    //hton unsigned int host byte order to network byte order
    server_addr.sin_port = htons(PORT); // Port number

    //ip address to bind too
    //sin addr is a struct of in_addr. in_addr has member s_addr
    server_addr.sin_addr.s_addr = inet_addr(IP_addr);

    // Bind the socket to the IP address and port
    // if the size of server addr is less than zero, error
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket\n";
        return -1;
    }


    // turns passive socket into active socket that can recieve connections with accept()
    if (listen(server_socket, 10) < 0)
    {
        std::cerr << "failed listen\n";
        return -1;
    }

    // infinite loop to accept connections
    while(1)
    {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

        int addr_len = sizeof(server_addr);

        // accept the connection
        int new_socket = accept(server_socket, (struct sockaddr *)&server_addr, (socklen_t*)&addr_len);

        //if socket connection fails
        if (new_socket < 0)
        {
            std::cerr << "connection accept failed\n";
            return -1;
        }
        
        //create buffer 30,000 char
        //char 1 bytes * 30,000
        char buffer[30000] = {0};

        //read in 30,000 bytes from accept socket and store in buffer
        long valread = read(new_socket , buffer, 30000);

        // print message from the client
        std::cout << "\n+++++++ Message from client++++++++\n\n";
        std::cout << buffer << '\n';

            
        // http hello msg
        char* hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

        // write to accept socket
        write(new_socket , hello , strlen(hello));

        printf("------------------Hello message sent-------------------");
        close(new_socket);
    }


    std::cout << "Merny\n";

    return 0;
}