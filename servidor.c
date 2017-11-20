#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <wiringPi.h>

#define	LED 4

void erroAplicacao(const char *msg)
{
	perror(msg);
    	exit(1);
}


int main(void)
{
		wiringPiSetupGpio () ;
	  	pinMode (LED, OUTPUT) ;

		int socketCliente;
		int socketAceite;
		int portaNumero = 1234;
		socklen_t socketClienteTamanho; 
     
     	char buffer[256];
     	struct sockaddr_in servidorEndereco; 
		struct sockaddr_in clienteEndereco;
	
		char mensagemLed[] = "Led Acesso";

     	int leiturasTamanho;

     	socketCliente = socket(AF_INET, SOCK_STREAM, 0);
     	if (socketCliente < 0) {
        	erroAplicacao("Erro na abertura do socket");
     	}

     	bzero((char *) &servidorEndereco, sizeof(servidorEndereco));

     	servidorEndereco.sin_family = AF_INET;
     	servidorEndereco.sin_addr.s_addr = INADDR_ANY;
     	servidorEndereco.sin_port = htons(portaNumero);

     	if (bind(socketCliente, (struct sockaddr *) &servidorEndereco,sizeof(servidorEndereco)) < 0){
			erroAplicacao("Erro na abertura do socket");
		} 
             
     	listen(socketCliente,2);
     	socketClienteTamanho = sizeof(clienteEndereco);

     	socketAceite = accept(socketCliente, (struct sockaddr *) &clienteEndereco, &socketClienteTamanho);
     	if (socketAceite < 0) {
        	erroAplicacao("Erro na aceite do socket");
     	}
     	printf("Conectado com o cliente\n");

     	while(true){
			bzero(buffer,256);
	     		leiturasTamanho = read(socketAceite,buffer,255);
	     		if (leiturasTamanho < 0) {
				erroAplicacao("Erro na leitura do socket");
			}
			//printf("Mensagem %n\n",buffer);

			if(strcmp(buffer,"led") ==0){
				leiturasTamanho = write(socketAceite,mensagemLed,sizeof(mensagemLed));
				digitalWrite (LED, HIGH) ;	// On
	    	   		delay (500) ;		// mS
	    	   		digitalWrite (LED, LOW) ;	// Off
	           		delay (500) ;
			}
		
     		if (leiturasTamanho < 0){
		 		erroAplicacao("Erro na escrita do socket");
			}
     	}

     	close(socketAceite);
     	close(socketCliente);
     	return 0; 
}