// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <algorithm>

#include <stdlib.h>

#include "disk.h"

//constructor
Disk::Disk()
{
  letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  allocation = std::vector<char>(128, '.');
  blocksLeft = n_blocks;
  letterIndex = 0;
  system("rm -rf .storage");
  system("mkdir .storage");

  std::cout << "Block size is " << blocksize << std::endl;
  std::cout << "Number of blocks is " << n_blocks << std::endl;

}

//destructor
Disk::~Disk()
{}

//print
void Disk::printCluster()
{
  std::cout << "================================" << std::endl;
  for ( int i = 0; i < 128; i++ )
  {
    if ( i % 32 == 0 && i != 0 )
    {
      std::cout << std::endl;
    }
    std::cout << allocation[i];
  }
  std::cout << std::endl;
  std::cout << "================================" << std::endl;
}

void Disk::incrementLetterIndex()
{
  letterIndex++;
  if ( letterIndex == letters.length() )
  {
    letterIndex = 0;
  }
}

char Disk::allocateBlocks(std::string filename, int blocksNeeded)
{
  blocksLeft -= blocksNeeded;
  char letter = letters[letterIndex];
  incrementLetterIndex();
  filesToBlocks[filename] = std::vector<int>();
  for ( int i = 0; i < allocation.size() && blocksNeeded > 0; i++ )
  {
    if ( allocation[i] == '.' )
    {
      allocation[i] = letter;
      filesToBlocks[filename].push_back(i);
      blocksNeeded--;
    }
  }
  return letter;
}

int Disk::getNumClusters(std::string filename)
{
  std::vector<int> blocks = filesToBlocks[filename];
  if ( blocks.size() == 0)
  {
    return 0;
  }
  else
  {
    int numClusters = 1;
    for ( int i = 1; i < blocks.size(); i++ )
    {
      if ( blocks[i] != (blocks[i-1]+1) )
      {
        numClusters++;
      }
    }
    return numClusters;
  }
}

//store a file
//return the message the server should send back
std::string Disk::storeFile(std::string filename,
    int size,
    const char* contents)
{
  if ( filesToLetters.find(filename) != filesToLetters.end() )
  {
    return "ERROR: FILE EXISTS\n";
  }
  else
  {
    int blocksNeeded = size/blocksize;
    if ( (blocksNeeded * blocksize) < size )
    {
      blocksNeeded++;
    }
    if ( blocksNeeded > blocksLeft )
    {
      return "ERROR: INSUFFICIENT DISK SPACE\n";
    }
    else
    {
      char letter = allocateBlocks(filename, blocksNeeded);
      std::ofstream outFile((".storage/" + filename).c_str());
      std::string outString = "";
      for ( int i = 0; i < size; i++ )
      {
        outString += contents[i];
      }
      outFile << outString;
      filesToLetters[filename] = letter;
      int numClusters = getNumClusters(filename);
      printf("[Thread %lu] Stored file '%c' (%d bytes; %d blocks; %d clusters)\n",
          pthread_self(),
          letter,
          size,
          blocksNeeded,
          numClusters);
      return "ACK\n";
    }
  }
}

int Disk::getNumBlocksReadFrom(int offset, int length)
{
  if ( length + offset < blocksize && length != 0 )
  {
    return 1;
  }
  else
  {
    int blocks = 0;
    if ( offset != 0 && offset % blocksize != 0 )
    {
      blocks++;
    }
    blocks += (length / blocksize);
    if ( length > blocksize && length % blocksize != 0 )
    {
      blocks++;
    }
    return blocks;
  }
}

//read contents from a file, read length bytes starting at the offset
//return the message the server should send back
std::string Disk::readFile(std::string filename,
    int offset,
    int length)
{
  if ( filesToLetters.find(filename) == filesToLetters.end() )
  {
    return "ERROR: NO SUCH FILE\n";
  }
  else
  {
    std::ifstream inFile((".storage/" + filename).c_str());
    std::stringstream ss;
    ss << inFile.rdbuf();
    std::string fileContents = ss.str();
    if ( fileContents.length() < offset + length )
    {
      return "ERROR: INVALID BYTE RANGE\n";
    }
    else
    {
      char intBuff[11];
      sprintf(intBuff, "%d", length);
      std::string returnString = "ACK " + std::string(intBuff) + "\n";
      for ( int i = offset; i < offset + length; i++ )
      {
        returnString += fileContents[i];
      }
      int numBlocksReadFrom = getNumBlocksReadFrom(offset, length);
      printf("[Thread %lu] Sent %d bytes (from %d '%c' blocks) from offset %d\n",
          pthread_self(),
          length,
          numBlocksReadFrom,
          filesToLetters[filename],
          offset);
      return returnString;
    }
  }
}

void Disk::deallocateBlocks(std::string filename)
{
  std::vector<int> blocksToDeallocate = filesToBlocks[filename];
  for ( int i = 0; i < blocksToDeallocate.size(); i++ )
  {
    allocation[blocksToDeallocate[i]] = '.';
  }
  blocksLeft += blocksToDeallocate.size();
}

//delete a file
std::string Disk::deleteFile(std::string filename)
{
  if ( filesToLetters.find(filename) == filesToLetters.end() )
  {
    return "ERROR: NO SUCH FILE\n";
  }
  else
  {
    printf("[Thread %lu] Deleted %s file '%c' (deallocated %lu blocks)\n", pthread_self(), filename.c_str(), filesToLetters[filename], filesToBlocks[filename].size());
    std::string command = "rm .storage/" + filename;
    system(command.c_str());
    deallocateBlocks(filename);
    filesToBlocks.erase(filename);
    filesToLetters.erase(filename);
    return "ACK\n";
  }
}

//show the files in the directory
std::string Disk::dir()
{
  std::vector<std::string> fileNames;
  std::map<std::string, char>::iterator itr;
  for ( itr = filesToLetters.begin(); itr != filesToLetters.end(); itr++ )
  {
    fileNames.push_back(itr->first);
  }
  std::sort(fileNames.begin(), fileNames.end());
  char intBuff[11];
  sprintf(intBuff, "%d", (int)fileNames.size());
  std::string returnString = std::string(intBuff) + "\n";
  for ( int i = 0; i < fileNames.size(); i++ )
  {
    returnString += fileNames[i] + "\n";
  }
  return returnString;
}
