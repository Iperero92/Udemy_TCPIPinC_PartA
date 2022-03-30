#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){
	int error_code = 0;
    int socket_desc;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    socklen_t server_struct_length = sizeof(server_addr);
    

	char continuar = 'n';
   
	do{
		// Create socket:
    	socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    	
    	if(socket_desc < 0){
    	    printf("Error while creating socket\n");
    	    error_code = -1;
			break;
    	}
    	printf("Socket created successfully\n");
    	
    	// Set port and IP:
    	server_addr.sin_family = AF_INET;
    	server_addr.sin_port = htons(2000);
    	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    	
		do{
			// Clean buffers:
	    	memset(server_message, '\0', sizeof(server_message));
	    	memset(client_message, '\0', sizeof(client_message));
			// Get input from the user:
			printf("Enter message: ");
			fgets(client_message,sizeof(client_message),stdin);
			
			// Send the message to server:
    		if(sendto(socket_desc, client_message, strlen(client_message), 0,
    		     (struct sockaddr*)&server_addr, server_struct_length) < 0){
    		    printf("Unable to send message\n");
    		    error_code = -1;
				break;
    		}
    		
    		// Receive the server's response:
    		if(recvfrom(socket_desc, server_message, sizeof(server_message), 0,
    		     (struct sockaddr*)&server_addr, &server_struct_length) < 0){
    		    printf("Error while receiving server's msg\n");
    		    error_code = -1;
				break;
    		}
    		
    		printf("Server's response: %s\n", server_message);
			printf("Desea continuar? (s/n)\n");
			continuar = getchar();
			fflush(stdin);
		} while (continuar == 's');
	}while(0);
    // Close the socket:
    close(socket_desc);
    
    return error_code;
}
