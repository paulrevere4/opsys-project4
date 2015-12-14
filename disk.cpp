#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "disk.h"

//constructor
Disk::Disk()
{
  allocation = std::vector<char>(128, '.');
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
bool Disk::storeFile(std::string filename,
  int size,
  char* contents)
{
	return false;
}

//read contents from a file, read length bytes starting at the offset
std::string Disk::readFile(std::string filename,
  int offset,
  int length)
{
  return "";
}

//delete a file
bool Disk::deleteFile(std::string filename)
{
  return false;
}

//show the files in the directory
std::string Disk::dir()
{
  return "";
}
