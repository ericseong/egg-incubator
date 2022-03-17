#ifndef __LOCKED_FILE_ACCESS_H__
#define __LOCKED_FILE_ACCESS_H__
#include <string>

#define CUR_SESSION_STAT_FILE_NAME "cur_session_stat.txt"
#define READ_BUF_SIZE 1024

class LockedFileAccess {
	std::string fileName;
public:
	LockedFileAccess( std::string fileName ) : fileName(fileName) {}
	virtual ~LockedFileAccess() {}
	int readFile( std::string& line ) const;
	int writeFile( std::string& line ) const; 
};

#endif

// eof

