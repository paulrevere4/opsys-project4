#include <sys/types.h>
#include <sys/socket.h>

#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

/**
store
  -Stores the file described by the arguments
args
  -char* filename - string for the name of the file we will store in
  -char* filesize - string for the size (in bytes) of the file
  -char* filedata - string for the data contained in the file
returns
  -int - 1 if sucess, 0 if false
*/
// STORE <filename> <bytes>\n<file-contents>
// -- add file <filename> to the storage server
// -- if the file already exists, return an "ERROR: FILE EXISTS\n" error
// -- in general, return "ERROR: <error-description>\n" if unsuccessful
// -- return "ACK\n" if successful
int store(char* filename,
    char* filesize,
    char* filedata)
{

  return 0;
}

/**
read
  -Reads <length> bytes from the file <filename> starting at the index <offset>
  of the file. Puts them in the <destination> pointer given
args
  -char* filename - the name of the file to be read from
  -char* byteoffset - index to start reading from
  -char* length - number of bytes to read
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
int read(char* filename,
    char* byteoffset,
    char* length,
    char* destination)
{
  return 0;
}

/**
delete
  -Deletes the file with the name <filename>
args
  -char* filename - name of the file to delete
returns
  -int - 1 if sucess, 0 if false
*/
// DELETE <filename>\n
// -- delete file <filename> from the storage server
// -- if the file does not exist, return an "ERROR: NO SUCH FILE\n" error
// -- in general, return "ERROR: <error-description>\n" if unsuccessful
// -- return "ACK\n" if successful
int delete(char* filename)
{
  return 0;
}

/**
dir
  -Makes a string that shows the files in the directory
args
  -char* destination - destination for the string to be written to
returns
  -int - the size of the string <destination>
*/
// DIR\n
// -- server returns the list of files currently stored on the server
// -- the list must be in alphabetical order
// -- the format of the message containing the list of files is as follows:
// <number-of-files>\n<filename1>\n<filename2>\n...\n
// -- therefore, if no files are stored, "0\n" is returned
int dir(char* destination)
{
  return 0;
}


int main( )
{

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

    int n = read(newsock, buffer, 1024);

    if (n < 0)
    {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }

    printf("MESSAGE: %s", buffer);

  }

}
