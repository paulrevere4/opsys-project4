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
#include <vector>
#include <sstream>
using namespace std;

#include "disk.h"

typedef struct input {
  Disk* disk;
  int*  socket;
} input;

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
bool command_store(char* query, int* socket, Disk* disk)
{
  
  bool inCommand = true;
  bool inFilename = false;
  bool inBytes = false;
  
  std::string filename;
  std::string file;
  
  std::string bytesString;
  int bytes;
  
  unsigned int i;
  for (i = 0; i < strlen(query); i++)
  {
    
    // cout << query[i] << endl;
    // cout << "  inCommand: " << inCommand << endl;
    // cout << "  inFilename: " << inFilename << endl;
    // cout << "  inBytes: " << inBytes << endl;
    
    if (query[i] == ' ')
    {
      if (inCommand)
      {
        inCommand = false;
        inFilename = true;
      }
      else if (inFilename)
      {
        inFilename = false;
        inBytes = true;
      }
    }
    
    else
    {
      if (query[i] == '\n')
      {
        // check if there's more after later on?
        i += 1;
        break;
      }
      
      if (inFilename)
      {
        filename += query[i];
      }
      
      if (inBytes)
      {
        bytesString += query[i];
      }
    }
    
    
  }
  
  bytes = atoi(bytesString.c_str());
  
  int bytesRead = 0;
  
  for (; i < strlen(query); i++)
  {
    file += query[i];
    bytesRead += 1;
  }
  
  while (bytesRead < bytes)
  {
    char chunk[1024];
    bzero(chunk, 1024);
    int n = read(*socket, chunk, 1024);
  
    if (n < 0)
    {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }
    
    if (n == 0)
    {
      printf("[Thread %lu] Client closed its socket....terminating\n", pthread_self());
      free(socket);
      return false;
    }
    
    for (i = 0; i < n; i++)
    {
      file += chunk[i];
      bytesRead += 1;
    }

  }
  
  // std::cout << "filename: " << filename << std::endl;
  // std::cout << "bytes: " << bytes << std::endl;
  // std::cout << "file: " << "|" << file << "|" << std::endl;
  
  std::string output = disk->storeFile(filename, bytes, file.c_str());
  
  printf("[Thread %lu] Simulated Clustered Disk Space Allocation:\n", pthread_self());
  disk->printCluster();
  
  write(*socket, output.c_str(), output.size());
  printf("[Thread %lu] Sent: %s", pthread_self(), output.c_str());
  
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
bool command_read(char* query, int* socket, Disk* disk)
{
  std::string input(query);
  input = input.substr(0, input.size()-1);
  
  std::vector<std::string> sections;
  std::stringstream sstream(input.c_str());
  std::string split;

  while(getline(sstream, split, ' '))
  {
    sections.push_back(split);
  }
  
  std::string filename = sections[1];
  int offset = atoi(sections[2].c_str());
  int length = atoi(sections[3].c_str());
  
  // std::cout << "filename: " << filename << std::endl;
  // std::cout << "offset: " << offset << std::endl;
  // std::cout << "length: " << length << std::endl;
  
  std::string output = disk->readFile(filename, offset, length);
  std::string serverOutput;
  for (int i = 0 ; i < output.size(); i++)
  {
    serverOutput += output[i];
    if (output[i] == '\n') 
    {
      break;
    }
  }
  
  write(*socket, output.c_str(), output.size());
  printf("[Thread %lu] Sent: %s", pthread_self(), serverOutput.c_str());
  
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
bool command_delete(char* query, int* socket, Disk* disk)
{
  
  std::string input(query);
  input = input.substr(0, input.size()-1);
  
  std::vector<std::string> sections;
  std::stringstream sstream(input.c_str());
  std::string split;

  while(getline(sstream, split, ' '))
  {
    sections.push_back(split);
  }
  
  std::string filename = sections[1];
  
  // std::cout << "filename: " << filename << std::endl;
  
  std::string output = disk->deleteFile(filename);
  
  printf("[Thread %lu] Simulated Clustered Disk Space Allocation:\n", pthread_self());
  disk->printCluster();
  
  write(*socket, output.c_str(), output.size());
  printf("[Thread %lu] Sent: %s", pthread_self(), output.c_str());
  
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
bool command_dir(char* query, int* socket, Disk* disk)
{
  
  std::string output = disk->dir();
  
  std::vector<std::string> sections;
  std::stringstream sstream(output.c_str());
  std::string split;

  while(getline(sstream, split, '\n'))
  {
    sections.push_back(split);
  }
  
  write(*socket, output.c_str(), output.size());
  printf("[Thread %lu] Sent directory listing for %s files\n", pthread_self(), sections[0].c_str());
  
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
int readQuery(char* query, char* destination, int* socket, Disk* disk)
{
  std::string queryType = getQueryType(query);
  if ( queryType == "STORE" )
  {
    //STORE
    command_store(query, socket, disk);
  }
  else if ( queryType == "READ" )
  {
    //READ
    command_read(query, socket, disk);
  }
  else if ( queryType == "DELETE" )
  {
    //DELETE
    command_delete(query, socket, disk);
  }
  else if ( queryType == "DIR" )
  {
    //DIR
    command_dir(query, socket, disk);
  }
  else
  {
    //error
    printf("ERROR: unrecognized query\n");
  }

  return 0;
}

/**
clientListen
  -threaded function to handle each individual client
args
  -void* argument - input struct containing arguments
returns
  -exits once read() call returns 0 (client exits)
*/
void* clientListen(void* arguments)
{
  input* args = (input*) arguments;
  int* socket = args->socket;
  Disk* disk = args->disk;
  
  while(1)
  {
  
    // printf("THREAD: Blocked on read()\n");
  
    char buffer[1024];
    bzero(buffer, 1024);
    int n = read(*socket, buffer, 1024);
  
    if (n < 0)
    {
        perror("read() failed");
        exit(EXIT_FAILURE);
    }
    
    if (n == 0)
    {
      printf("[Thread %lu]: Client closed its socket....terminating\n", pthread_self());
      free(socket);
      return NULL;
    }
    
    std::string command;
    for (int i = 0; i < strlen(buffer); i++)
    {
      command += buffer[i];
      if (buffer[i] == '\n')
      {
        break;
      }
    }
  
    printf("[Thread %lu]: Rcvd %s", pthread_self(), command.c_str());
    char* resMsg = (char*)calloc(1, sizeof(char));
    int resStat = readQuery(buffer, resMsg, socket, disk);
  }
  
}


int main()
{
  
  Disk disk;

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
  printf("Listening on port %d\n", listener_port);

  struct sockaddr_in client;
  int fromlen = sizeof(client);
  
  //vector<int*> sockets;

  while(1)
  {
    
    //printf("PARENT: Blocked on accept()\n");

    int newsock = accept(sock, (struct sockaddr*)&client, (socklen_t*)&fromlen);
    cout << client.sin_addr.s_addr << endl;
    printf("Received incoming connection from %s\n", inet_ntoa(client.sin_addr));
    //printf("Received incoming connection\n");
    
    int* socket = (int*)malloc(sizeof(int));
    *socket = newsock;
    
    input* in = (input*)malloc(sizeof(input));
    in->disk = &disk;
    in->socket = socket;
    
    
    pthread_t tid;
    
    int rc = pthread_create(&tid, NULL, clientListen, (void*)in);
    
    if (rc != 0)
    {
        perror("pthread_create() failed");
        exit(EXIT_FAILURE);
    }

  }

}
