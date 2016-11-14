/*!
 * Simple chat program (server side).cpp - http://github.com/hassanyf
 * Version - 2.0.1
 *
 * Copyright (c) 2016 Hassan M. Yousuf
 */

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main()
{
    /* ---------- INITIALIZING VARIABLES ---------- */

    int client, server;//client/server are two file descriptors.
    int portNum = 1500;//portNum is for storing port number on which the accepts connections.
    bool isExit = false;//isExit is bool variable which will be used to end the loop.
    int bufsize = 256;
    char buffer[bufsize];//server reads characters from the socket connection into a dynamic variable (buffer).

    struct sockaddr_in server_addr;//serv_addr will contain the address of the server.
    socklen_t size;//socklen_t  is an intr type of width of at least 32 bits.

    /* ---------- ESTABLISHING SOCKET CONNECTION ----------*/
    /* --------------- socket() function ------------------*/

    client = socket(AF_INET, SOCK_STREAM, 0);//socket() function creates a new socket.

    if (client < 0) 
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


    if ((bind(client, (struct sockaddr*)&server_addr,sizeof(server_addr))) < 0) //The bind() system call binds a socket to an address.
    {
        cout << "=> Error binding connection, the socket has already been established..." << endl;
        return -1;
    }

    size = sizeof(server_addr);
    cout << "=> Looking for clients..." << endl;

    /* ------------- LISTENING CALL ------------- */
    /* ---------------- listen() ---------------- */

    listen(client, 1);

    /* 
        The listen system call allows the process to listen 
        on the socket for connections. 

        The program will be stay idle here if there are no 
        incomming connections.

        The first argument is the socket file descriptor, 
        and the second is the size for the number of clients 
        i.e the number of connections that the server can 
        handle while the process is handling a particular 
        connection. The maximum size permitted by most 
        systems is 5.

    */

    /* ------------- ACCEPTING CLIENTS  ------------- */
    /* ----------------- listen() ------------------- */

    /* 
        The accept() system call causes the process to block 
        until a client connects to the server. Thus, it wakes 
        up the process when a connection from a client has been 
        successfully established. It returns a new file descriptor, 
        and all communication on this connection should be done 
        using the new file descriptor. The second argument is a 
        reference pointer to the address of the client on the other 
        end of the connection, and the third argument is the size 
        of this structure.
    */

    int clientCount = 1;
    server = accept(client,(struct sockaddr *)&server_addr,&size);

    // first check if it is valid or not
    if (server < 0) 
        cout << "=> Error on accepting..." << endl;

    while (server > 0) 
    {
        strcpy(buffer, "=> Server connected...\n");
        send(server, buffer, bufsize, 0);
        cout << "=> Connected with the client #" << clientCount << ", you are good to go..." << endl;
        cout << "\n=> Enter # to end the connection\n" << endl;

        /* 
            Note that we would only get to this point after a 
            client has successfully connected to our server. 
            This reads from the socket. Note that the read() 
            will block until there is something for it to read 
            in the socket, i.e. after the client has executed a 
            the send().

            It will read either the total number of characters 
            in the socket or 1024
        */
	cout<<"Message from client\n";
        cout << "Client: ";
        do {
		cout<<"Server 1st client:"<<buffer<<"\n";
            recv(server, buffer, bufsize, 0);
            cout << buffer << " ";
           if (*buffer == '#') {
                *buffer = '*';
                isExit = true;
            }
        } while (*buffer != '*');

        do {
            cout << "\nServer: ";
            do {
                cin >> buffer;
		cout<<"Server 1st server:"<<buffer<<"\n";
                send(server, buffer, bufsize, 0);
                if (*buffer == '#') {
                    send(server, buffer, bufsize, 0);
                    *buffer = '*';
                    isExit = true;
                }
            } while (*buffer != '*');

            cout << "Client: ";
            do {
		cout<<"Server 2nd client:"<<buffer<<"\n";
                recv(server, buffer, bufsize, 0);
                cout << buffer << " ";
               if (*buffer == '#') {
                    *buffer == '*';
                    isExit = true;
                }
            } while (*buffer != '*');
        } while (!isExit);

    

        /* ---------------- CLOSE CALL ------------- */
        /* ----------------- close() --------------- */

        /* 
            Once the server presses # to end the connection,
            the loop will break and it will close the server 
            socket connection and the client connection.
        */

        // inet_ntoa converts packet data to IP, which was taken from client
        cout << "\n\n=> Connection terminated with IP " << inet_ntoa(server_addr.sin_addr);
        close(server);
        cout << "\nGoodbye..." << endl;
        isExit = false;
        exit(1);
    }

    close(client);
    return 0;
}