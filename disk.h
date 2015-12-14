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
	bool storeFile(std::string filename, //store a file
      int size,
      char* contents);
  //read contents from a file, read length bytes starting at the offset
  std::string readFile(std::string filename,
      int offset,
      int length);
  bool deleteFile(std::string filename); //delete a file
  std::string dir(); //show the files in the directory

};

#endif
