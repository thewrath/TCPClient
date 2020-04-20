#include <iostream>
#include <string.h>
#include <thread>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

const int BUFSIZE = 15;
const int PORT = 3333;

struct SocketException : public std::exception
{
    std::string message;
    SocketException(std::string m) : message(m) {}
    ~SocketException() throw () {}
    const char* what() const throw() { return message.c_str(); }
};

void read(int description, char *buffer)
{
    ssize_t bytes_read = 0;
    memset(buffer, 0, BUFSIZE);
    //testing stuff, BUFSIZE = 4096
    printf("Received Socket # %d\nBuffer Size = %d\n", description, BUFSIZE);

    // buffer[bytes_read] = 0; // Null-terminate the buffer
    bytes_read = recv(description, buffer, BUFSIZE, 0);

    if (bytes_read == -1) {
        throw SocketException("Socket recv failed");
    }

    //Turn into a sting
    // std::string dataStr(buffer);

    std::cout << buffer << std::endl;

    // std::cout << "Fin de reception" << std::endl;
}

void write(int description, std::string data)
{
    std::cout << "Send response to server :" << data << std::endl;
    int n = send(description, data.c_str(), data.length(), 0);
    if (n < 0) {
        throw SocketException("ERROR writing to socket");
    }
}

int main() {

    char buffer[BUFSIZE];

    int description = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in serv_addr;

    if (description < 0) {
        throw SocketException("Erreur initialisation socket.");
    }

    // Structure contenant les informations "reseaux" de la socket
    serv_addr.sin_port = htons(PORT); // on precise le port d'ecoute (htons permet de convertir le int du c++ en int reseaux)
    serv_addr.sin_family = AF_INET; // on precise qu'il s'agit d'un socket reseau et non inter-processus (AF_UNIX)
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        throw SocketException("Invalide address");
    }

    if (connect(description, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        throw SocketException("Cannot connect to server");  
    }

     while(true){
        write(description, "test");
        try {
            read(description, buffer);
            // Pour plus tard 
            // Message::Duck duck;
            // duck.ParseFromString(std::string(buffer));
            
            // if(duck.IsInitialized()) {
                // std::cout << duck.DebugString();
                // break;
            // }   
            
        } catch(SocketException& e) {
            std::cout << e.what() << std::endl;
        }
    }

    close(description);
}