#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <linux/if_ether.h>
#include <stddef.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[1024];
    int bytes_read;
    char recvline[1000];
    struct iphdr ip; // структура для хранения IP заголовка пакета
    struct tcphdr tcp; // TCP заголовок
    struct ethhdr eth; // заголовок Ethernet-кадра
    __u32 num = 0;

    listener = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    //listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0)
    {
        perror("socket");
        exit(1);
    }
    
    addr.sin_family = PF_PACKET;
    addr.sin_port = htons(ETH_P_ALL);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    printf("IP адрес - %d\n",addr.sin_family);
    printf("IP адрес - %d\n",addr.sin_port);
    printf("IP адрес - %d\n",addr.sin_addr.s_addr);
/*
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("bindn");
        exit(2);
    }
*/
    listen(listener, 1);
    
    

        while(1)
        {
            bytes_read=recvfrom(listener,buf,sizeof(buf),0,NULL,NULL);
            //recvline[bytes_read]=0;
            //printf("Received: %s\n",bytes_read);
            
            if(bytes_read <= 0) break;

    memcpy((void *)&eth, buf, ETH_HLEN);
    memcpy((void *)&ip, buf + ETH_HLEN, sizeof(struct iphdr));
    if((ip.version) != 4) continue;
    memcpy((void *)&tcp, buf + ETH_HLEN + ip.ihl * 4, sizeof(struct tcphdr));

/*
 * MAC-адреса отправителя и получателя
 */
    printf("\n%u\n", num++);
    printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\t->\t",
    eth.h_source[0],eth.h_source[1],eth.h_source[2],
    eth.h_source[3],eth.h_source[4],eth.h_source[5]);

    printf("%.2x:%.2x:%.2x:%.2x:%.2x:%.2x\n",
    eth.h_dest[0],eth.h_dest[1],eth.h_dest[2],
    eth.h_dest[3],eth.h_dest[4],eth.h_dest[5]);

    printf("Длина заголовка - %d, ", (ip.ihl * 4));
    printf("длина пакета - %d\n", ntohs(ip.tot_len));

/*
 * Если транспортный протокол - TCP, отобразим IP адреса и порты
 * получателя и отправителя
 */
    if(ip.protocol == IPPROTO_TCP) {
        printf("%d (%d)\t->\t",inet_ntoa(ip.saddr), ntohs(tcp.source));
        printf("%d (%d)\n",inet_ntoa(ip.daddr), ntohs(tcp.dest));
        printf("TCP пакет\n");
    }
        }
    
        close(sock);
    
    
    return 0;
}
