#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;
int main(int argc,char* argv[])
{
	/* ---------- INITIALIZING VARIABLES ---------- */
	int fdClient, fdServer; /*!< client and server are two file descriptors. */
	int portNum =atoi(argv[1]); /*!< portNum is for storing port number on which the accepts connections. */
	bool isExit = false; /*!< isExit is bool variable which will be used to end the loop. */
	int bufSize =atoi(argv[2]);   /*!< 256(max.) characters will receive from client side. */
	char msgBuffer[bufSize]; /*!< server reads characters from the socket connection into a dynamic variable (msgBuffer). */
	struct sockaddr_in server_addr; /*!< serv_addr will contain the address of the server. */
	socklen_t size; /*!< socklen_t  is an intr type of width of at least 32 bits. */
	int clientCount = 0;
	//! Socket function
	/*!
	 * AF_INET: address domain of the socket.
	 * SOCK_STREAM: Type of socket. a stream socket in which characters are read in a continuous stream (TCP).
	 * Third is a protocol argument: should always be 0. The OS will choose the most appropiate protocol.
	 * This will return a small integer and is used for all references to this socket. If the socket call fails,
          it returns -1.
	 */
	fdClient = socket(AF_INET, SOCK_STREAM, 0);//socket() function creates a new socket.

	/* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
	/* --------------- socket() function ------------------*/
	if (fdClient < 0)
	{
		cout << "\nError establishing socket..." << endl;
		exit(1);
	}
	cout << "\n=> Socket server has been created..." << endl;
	server_addr.sin_family = AF_INET;//The variable serv_addr is a structure of sockaddr_in.It should always be set to AF_INET.
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);// INADDR_ANY contains the IP address of the host.
	server_addr.sin_port = htons(portNum);//htons() converts the port number from host byte order to a port number in network byte order.
	/* ---------- BINDING THE SOCKET ---------- */
	/* ---------------- bind() ---------------- */
	if ((bind(fdClient, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) /*!< The bind() system call binds a socket to an address. */
	{
		cout << "=> Error binding connection, the socket has already been established..." << endl;
		return -1;
	}
	size = sizeof(server_addr);
	// bool busy=false;
	cout << "\n=> Looking for clients...." << endl;
	start: /*!< goto call will start from here */
	/* ------------- LISTENING CALL ------------- */
	/* ---------------- listen() ---------------- */
	//! listen() system call
	/*!
        \ The listen system call allows the process to listen on the socket for connections. 
        \ The program will be stay idle here if there are no incomming connections.
        \ The first argument is the socket file descriptor,and the second is the size for the number of clients 
          i.e the number of connections that the server can handle while the process is handling a particular connection.
        \ The maximum size permitted by most systems is 5.
	 */
	listen(fdClient,1);
	/* ------------- ACCEPTING CLIENTS  ------------- */
	//! accept() system call
	/*!
        \ The accept() system call causes the process to block until a client connects to the server. Thus, it wakes 
          up the process when a connection from a client has been successfully established. 
        \ It returns a new file descriptor,and all communication on this connection should be done using the new file descriptor.
        \ The second argument is a reference pointer to the address of the client on the other end of the connection.
        \ The third argument is the size of this structure.
	 */
	fdServer = accept(fdClient,(struct sockaddr *)&server_addr,&size);
	clientCount++;
	// first check if it is valid or not
	if (fdServer < 0)
		cout << "=> Error on accepting..." << endl;

	while (fdServer > 0)
	{
		cout << "\n=>Connected with the client #" << clientCount << ", you are good to go..." << endl;
		cout << "\n=> Enter .bye to end the connection\n" << endl;
		/*!
            \ Note that we would only get to this point after a client has successfully connected to our server. 
            \ This reads from the socket. Note that the read() will block until there is something for it to read 
              in the socket, i.e. after the client has executed a the send().
            \ It will read either the total number of characters in the socket or size of buffer.
		 */
		int pid=fork();
		if(pid==0)
		{
			do {
				cout << "\n(S)Client "<<clientCount<<":";
				read(fdServer,msgBuffer,bufSize);
				cout << msgBuffer << " ";
				if (strcmp(msgBuffer,".bye")==0) {
					cout<<"\nClient "<<clientCount<<" is terminated his connection.\n";
					//clientCount++;
					isExit = true;
				}
			} while (!isExit);
		}
		isExit=false;
		//busy=false;
		goto start;
		/* ---------------- CLOSE CALL ------------- */
		/* ----------------- close() --------------- */
		/*!
            Once the server presses ".bye" to end the connection,the loop will break and it will close the server 
            socket connection and the client connection.
		 */
		/*!
		inet_ntoa converts packet data to IP, which was taken from client
		 */
		cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
		close(fdServer);
		cout << "\nGoodbye..." << endl;
		isExit = false;
		exit(1);
	}
	close(fdClient);
	return 0;
}
