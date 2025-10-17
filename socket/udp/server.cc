#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <iostream>
#include <string.h>
#include <string>


int main() {
    /* * * * * * * * * * * Socket步骤开始* * * * * * * * * */

    /**
     * 1. 创建socket
     * 
     * 函数签名：
     *   int socket(int domain, int type, int protocol);
     * 参数：
     *   domain : 通信类型， 一般就写AF_INET
     *   type : socket类型， TCP为 SOCK_STREAM, UDP为 SOCK_DGRAM
     *   protocol : 前面指定了，所以这里一般为 0
     * 返回值：
     *   创建socket成功返回一个文件描述符
     *   创建socket失败返回-1，并设置全局的错误码，表明错误的原因
     */
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) std::cerr << "create socket failed!" << std::endl;

    /**
     * 2. 绑定地址和端口
     * 
     * 函数签名：
     *   int bind(int sockfd, (struct sockaddr*)&local, sizeof(local));
     */
    struct sockaddr_in local;
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons();
    local.sin_addr.s_addr = ;

    // https://blog.csdn.net/D5486789_/article/details/142148987 
    // 写到这里了
}



