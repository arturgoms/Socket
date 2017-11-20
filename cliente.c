#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#include <unistd.h>
#include <sys/types.h> 
#include<sys/socket.h>
#include<arpa/inet.h>
#include <stdbool.h>
#include <wiringPi.h>

void erroAplicacao(const char *msg)
{
	perror(msg);
    	exit(1);
}


int main(void)
{
	int socketCliente;
	struct sockaddr_in servidorEndereco;
	char mensagem[256];
	char servidorMensagem[256];
	
	socketCliente = socket(AF_INET , SOCK_STREAM , 0);
	if (socketCliente < 0){
	    erroAplicacao("Erro na abertura do socket");
	}
	
	servidorEndereco.sin_addr.s_addr = inet_addr("192.168.0.5");
	servidorEndereco.sin_family = AF_INET;
	servidorEndereco.sin_port = htons( 1234 );

	if (connect(socketCliente , (struct sockaddr *)&servidorEndereco , sizeof(servidorEndereco)) < 0)
	{
		erroAplicacao("Erro na abertura do socket");
	}
	
	printf("Conectado com o Servidor\n");
	
	while(true)
	{
		bzero(mensagem,256);
		bzero(servidorMensagem,256);
		printf("Entre com a mensagem: ");
		scanf("%s" , mensagem);
		
		if(write(socketCliente , mensagem , sizeof(mensagem)) < 0)
		{
			erroAplicacao("Falha no envio da mensagem");
		}
		
		if( read(socketCliente , servidorMensagem , 255) < 0)
		{
			erroAplicacao("Falha no recebimento");
		}
		puts("Resposta servidor:");
		puts(servidorMensagem);
	}
	
	close(socketCliente);
	return 0;
}