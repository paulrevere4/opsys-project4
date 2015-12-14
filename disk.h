// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)

#ifndef DISK_H
#define DISK_H

// File system for Opsys Project 4
#include <vector>
#include <string>
#include <set>

class Disk
{
private:

  std::vector<char> allocation;
  std::set<std::string> files;

public:

	Disk(); //constructor
	~Disk(); //destructor
	void printCluster(); //print
	std::string storeFile(std::string filename, //store a file
      int size,
      char* contents);
  std::string readFile(std::string filename, //read contents from a file,
      int offset,              // read length bytes starting at the offset
      int length);
  bool deleteFile(std::string filename); //delete a file
  std::string dir(); //show the files in the directory

};

#endif
