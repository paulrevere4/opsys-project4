// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <string>

#include "disk.h"

/**
store
  -Stores the file described by the arguments in the file <filename> that is the
  length <bytes> with the contents <file-contents>
args
  -char* query - the query sent to the server
returns
  -int - 1 if sucess, 0 if false
*/
// STORE <filename> <bytes>\n<file-contents>
// -- add file <filename> to the storage server
// -- if the file already exists, return an "ERROR: FILE EXISTS\n" error
// -- in general, return "ERROR: <error-description>\n" if unsuccessful
// -- return "ACK\n" if successful
bool command_store(char* query)
{
  return 0;
}

/**
read
  -Reads <length> bytes from the file <filename> starting at the index <offset>
  of the file. Puts them in the <destination> pointer given
args
  -char* query - the query sent to the server
  -char* destination - destination for the bytes to be read to
returns
  -int - the number of bytes read, the size of the string <destination>
*/
// READ <filename> <byte-offset> <length>\n
// -- server returns <length> bytes of the contents of file <filename>,
// starting at <byte-offset>
// -- note that this does NOT remove the file on the server
// -- if the file does not exist, return an "ERROR: NO SUCH FILE\n" error
// -- if the file byte range is invalid, return an "ERROR: INVALID BYTE RANGE\n" error
// -- in general, return "ERROR: <error-description>\n" if unsuccessful
// -- return "ACK" if successful, following it with the length and data, as follows:
//    ACK <bytes>\n<file-excerpt>
bool command_read(char* query, char* destination)
{
  return 0;
}

/**
delete
  -Deletes the file with the name <filename>
args
  -char* query - the query sent to the server
returns
  -int - 1 if sucess, 0 if false
*/
// DELETE <filename>\n
// -- delete file <filename> from the storage server
// -- if the file does not exist, return an "ERROR: NO SUCH FILE\n" error
// -- in general, return "ERROR: <error-description>\n" if unsuccessful
// -- return "ACK\n" if successful
bool command_delete(char* query)
{
  return 0;
}

/**
dir
  -Makes a string that shows the files in the directory
args
  -char* query - the query sent to the server
returns
  -int - the size of the string <destination>
*/
// DIR\n
// -- server returns the list of files currently stored on the server
// -- the list must be in alphabetical order
// -- the format of the message containing the list of files is as follows:
// <number-of-files>\n<filename1>\n<filename2>\n...\n
// -- therefore, if no files are stored, "0\n" is returned
bool command_dir(char* query)
{
  return 0;
}

/**
getQueryType
  -gets the type of query sent so that the correct function can be called
args
  -char* query - the query to parse
returns
  -int:
    0 for read error
    1 for STORE
    2 for READ
    3 for DELETE
    4 for DIR
*/
std::string getQueryType(char* query)
{
  unsigned int len = strlen(query);
  unsigned int i = 0;
  int result = 0;
  while( i < len && query[i] == ' ' )
  {
    i++;
  }
  std::string command;
  while( i < len && query[i] != ' ' && query[i] != '\n')
  {
    command += query[i];
    i++;
  }
  return command;
}

/**
readQuery
  -reads the query sent to the server and prepares the response
args
  -char* query - the query from the client
  -char* destination - destination for the response to be written to
returns
  -int - size of the string <destination>
*/
int readQuery(char* query, char* destination)
{
  std::string queryType = getQueryType(query);
  if ( queryType == "STORE" )
  {
    //STORE
    printf("STORE\n");
  }
  else if ( queryType == "READ" )
  {
    //READ
    printf("READ\n");
  }
  else if ( queryType == "DELETE" )
  {
    //DELETE
    printf("DELETE\n");
  }
  else if ( queryType == "DIR" )
  {
    //DIR
    printf("DIR\n");
  }
  else
  {
    //error
    printf("ERROR: unrecognized query\n");
  }

  return 0;
}

int main( )
{
  // Disk disk;
  // disk.printCluster();

  int sock = socket(PF_INET, SOCK_STREAM, 0);

  if (sock < 0)
  {
    perror("socket() failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in server;
  server.sin_family = PF_INET;
  server.sin_addr.s_addr = INADDR_ANY;

  unsigned short listener_port = 8765;

  server.sin_port = htons(listener_port);
  int len = sizeof(server);

  if (bind(sock, (struct sockaddr*)&server, len) < 0)
  {
      perror("bind() failed");
      exit(EXIT_FAILURE);
  }

  listen(sock, 5);
  printf("PARENT: Listener bound to port %d\n", listener_port);

  struct sockaddr_in client;
  int fromlen = sizeof(client);

  int pid;
  char buffer[1024];

  while(1)
  {
    printf("PARENT: Blocked on accept()\n");

    int newsock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&fromlen);

    printf("PARENT: Accepted client connection\n");

    printf("THREAD: Blocked on read()\n");

    bzero(buffer, 1024);
    int n = read(newsock, buffer, 1024);

    if (n < 0)
    {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }

    printf("MESSAGE: '%s'\n", buffer);
    char* resMsg = (char*)calloc(1, sizeof(char));
    int resStat = readQuery(buffer, resMsg);

  }

}
