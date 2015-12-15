// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)

#ifndef DISK_H
#define DISK_H

// File system for Opsys Project 4
#include <vector>
#include <string>
#include <map>

class Disk
{
private:

  const static int n_blocks = 128;
  const static int blocksize = 4096;
  std::string letters;
  int blocksLeft;
  int letterIndex;
  std::vector<char> allocation;
  std::map<std::string, char> filesToLetters;
  std::map<std::string, std::vector<int> > filesToBlocks;

  void incrementLetterIndex();
  char allocateBlocks(std::string filename, int blocksNeeded);

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
  std::string deleteFile(std::string filename); //delete a file
  std::string dir(); //show the files in the directory

};

#endif
