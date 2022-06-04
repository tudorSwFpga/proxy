#include <stdexcept>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <string.h>
#include <stdexcept>
#include <iostream>


int main(){

	struct sockaddr_in addr;
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
        throw std::runtime_error(std::strerror(errno));
    }

	memset(&addr,0,sizeof(sockaddr_in)); //init address to 0
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(50000);

    connect(sockfd, (const struct sockaddr *)&addr, sizeof(sockaddr_in));

    char data[100] = "Tudor";
    while (true) {
        std::cin >> data;
        send(sockfd, &data, sizeof(data),0);
    }
    /* Send data to the host car */

    return 1;
}