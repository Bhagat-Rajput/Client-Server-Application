#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
using namespace std;
int main(int argc,char* argv[])
{
	/* ---------- INITIALIZING VARIABLES ---------- */
	int fdClient; /*!< File descriptor to store the values returned by the socket system call and the accept system call. */
	int portNum = atoi(argv[1]); /*!< NOTE that the port number is same for both client and server. */
	bool isExit = false; /*!< isExit is bool variable which will be used to end the loop. */
	int bufSize =atoi(argv[2]); /*!< client reads characters from the socket connection into a dynamic variable (msgBuffer). */
	char msgBuffer[bufSize];
	struct sockaddr_in server_addr;
	//! Socket function
	/*!
	 * AF_INET: address domain of the socket.
	 * SOCK_STREAM: Type of socket. a stream socket in which characters are read in a continuous stream (TCP).
	 * Third is a protocol argument: should always be 0. The OS will choose the most appropriate protocol.
	 * This will return a small integer and is used for all references to this socket. If the socket call fails,
          it returns -1.
	 */
	fdClient = socket(AF_INET, SOCK_STREAM, 0); /*!< Socket function creates a new socket.It takes three arguments. */
	/* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
	/* --------------- socket() function ------------------*/
	if (fdClient < 0)
	{
		cout << "\nError establishing socket..." << endl;
		exit(1);
	}
	else
	{
		cout << "\n=> Socket client has been created..." << endl;
	}
	/*!
	 * The variable serv_addr is a structure of sockaddr_in. sin_family contains a code for the address family.
	 * It should always be set to AF_INET.
	 * htons() converts the port number from host byte order to a port number in network byte order.
	 */
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(portNum); /*! htons() converts the port number from host byte order to a port number in network byte order. */
	/*!
	 * this function returns returns 1 if the IP is valid and 0 if invalid.
	 * inet_pton converts IP to packets.
	 * inet_ntoa converts back packets to IP.
	 * inet_pton(AF_INET, ip, &server_addr.sin_addr);
	 */
	/* ---------- CONNECTING THE SOCKET ---------- */
	/* ---------------- connect() ---------------- */
	if (connect(fdClient,(struct sockaddr *)&server_addr, sizeof(server_addr)) == 0)
		cout << "=> Connection to the server port number: " << portNum << endl;
	cout << "=> Awaiting confirmation from the server..." << endl;
	cout << "=> Connection confirmed, you are good to go..."<<msgBuffer<<"\n";
	cout << "\n\n=> Enter .bye to end the connection\n" << endl;
	/*-----Once it reaches here,the client can send a message first-----*/
	do {
		cout << "(C)Client: ";
		cin.getline(msgBuffer,256);
		write(fdClient, msgBuffer,bufSize);
		if (strcmp(msgBuffer,".bye")==0) {
			write(fdClient, msgBuffer, bufSize);
			isExit = true;
		}
	} while (!isExit);
	/* ----------------- close() --------------- */
	/*!
	 * Once the server presses ".bye" to end the connection,the loop will break and it will close the server
          socket connection and the client connection.
	 */
	cout << "\n=> Connection terminated.\nGoodbye...\n";
	close(fdClient);
	return 0;
}
