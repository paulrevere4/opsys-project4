// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <set>
#include <fstream>
#include <algorithm>

#include <stdlib.h>

#include "disk.h"

//constructor
Disk::Disk()
{
  allocation = std::vector<char>(128, '.');
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

//store a file
//return the message the server should send back
std::string Disk::storeFile(std::string filename,
    int size,
    char* contents)
{
  if ( files.find(filename) == files.end() )
  {
    return "ERROR: FILE EXISTS";
  }
  else
  {
    std::ofstream outFile(filename);
    std::string outString = "";
    for ( int i = 0; i < size; i++ )
    {
      outString += contents[i];
    }
    outFile << outString;
    files.insert(filename);
    return "ACK";
  }
}

//read contents from a file, read length bytes starting at the offset
//return the message the server should send back
std::string Disk::readFile(std::string filename,
    int offset,
    int length)
{
  if ( files.find(filename) != files.end() )
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

//delete a file
std::string Disk::deleteFile(std::string filename)
{
  if ( files.find(filename) != files.end() )
  {
    return "ERROR: NO SUCH FILE";
  }
  else
  {
    std::string command = "rm .storage/" + filename;
    system(command.c_str());
    return "ACK";
  }
}

//show the files in the directory
std::string Disk::dir()
{
  std::vector<std::string> fileNames;
  std::set<std::string>::iterator itr;
  for ( itr = files.begin(); itr != files.end(); itr++ )
  {
    fileNames.push_back(*itr);
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
