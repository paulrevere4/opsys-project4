// This the submission for Paul Revere(reverp) and Joseph Hitchcock(hitchj)

// File system for Opsys Project 4
#include <vector>
#include <string>

class disk
{
public:

	//constructor
	disk()
	{}

	//destructor
	~disk();

	//print
	void printCluster()
	{}

	//store a file
	bool storeFile(std::string filename,
      int size,
      char* contents)
  {
    return false;
  }

  //read contents from a file, read length bytes starting at the offset
  std::string readFile(std::string filename,
      int offset,
      int length)
  {
    return "";
  }

  //delete a file
  bool deleteFile(std::string filename)
  {
    return false;
  }

  //show the files in the directory
  std::string dir()
  {
    return "";
  }

private:

};