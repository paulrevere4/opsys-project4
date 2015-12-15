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

//store a file
//return the message the server should send back
std::string Disk::storeFile(std::string filename,
    int size,
    char* contents)
{
  if ( filesToLetters.find(filename) == filesToLetters.end() )
  {
    return "ERROR: FILE EXISTS";
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
      return "ERROR: INSUFFICIENT DISK SPACE";
    }
    else
    {
      blocksLeft -= blocksNeeded;
      char letter = allocateBlocks(filename, blocksNeeded);
      std::ofstream outFile(filename);
      std::string outString = "";
      for ( int i = 0; i < size; i++ )
      {
        outString += contents[i];
      }
      outFile << outString;
      filesToLetters[filename] = letter;
      return "ACK";
    }
  }
}

//read contents from a file, read length bytes starting at the offset
//return the message the server should send back
std::string Disk::readFile(std::string filename,
    int offset,
    int length)
{
  if ( filesToLetters.find(filename) != filesToLetters.end() )
  {
    return "ERROR: NO SUCH FILE";
  }
  else
  {
    std::ifstream inFile(filename);
    std::stringstream ss;
    ss << inFile.rdbuf();
    std::string fileContents = ss.str();
    if ( fileContents.length() < offset + length )
    {
      return "ERROR: INVALID BYTE RANGE";
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
      return returnString;
    }
  }
}

//delete a file
std::string Disk::deleteFile(std::string filename)
{
  if ( filesToLetters.find(filename) != filesToLetters.end() )
  {
    return "ERROR: NO SUCH FILE";
  }
  else
  {
    std::string command = "rm .storage/" + filename;
    system(command.c_str());
    filesToLetters.erase(filename);
    return "ACK";
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
  sprintf(intBuff, "%d", fileNames.size());
  std::string returnString = std::string(intBuff) + "\n";
  for ( int i = 0; i < fileNames.size(); i++ )
  {
    returnString += fileNames[i] + "\n";
  }
  return returnString;
}
