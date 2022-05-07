// LockedFileAccess.cpp
// This file is to write/read a shared file between the processess. *nix support file locking mechanism for mutually exclusive read/write.
//

#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include "LockedFileAccess.h"
#include <iostream>

// returns zero for success, -1 otherwise
int LockedFileAccess::writeFile( std::string& line ) const {
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();	

	int fd;

	fd = open( (char*)fileName.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666 );
	if( fd < 0 ) {
		perror( "file open failed." );
		return -1;
	}

	if( fcntl( fd, F_SETLK, &lock ) < 0 ) {
		perror( "fcntl failed to get lock." );
		close( fd );
		return -1;
	}

	if( write( fd, line.c_str(), line.length() ) < 0 ){
		perror("write");
		return -1;
	}

	if( fsync( fd ) ) {
			perror("write");
			return -1;
	}

	lock.l_type = F_UNLCK;
	if( fcntl( fd, F_SETLK, &lock ) < 0 ) {
		perror( "Unlock failed." );
		if( close(fd) ) {
      perror("close");
			return -1;
		}
	}
	
	if( close( fd ) ) {
    perror("close");
		return -1;
	}

	return 0;
}

// returns zero for success, -1 otherwise
int LockedFileAccess::readFile( std::string& line ) const {
	struct flock lock;
	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();	

	int fd;

	fd = open( (char*)fileName.c_str(), O_RDONLY );
	if( fd < 0 ) {
		perror( "file open failed." );
		return -1;
	}

	fcntl( fd, F_GETLK, &lock );
	if( lock.l_type != F_UNLCK ) {
		perror( "File is still write locked." );
		close( fd );
		return -1;
	} 

	lock.l_type = F_RDLCK;
	if( fcntl( fd, F_SETLK, &lock ) < 0 ) {
		perror( "Unlock failed." );
		close( fd );
		return -1;
	}
	char str[READ_BUF_SIZE]; 
	memset(str, '\0', sizeof(str));
	if( read( fd, str, READ_BUF_SIZE ) ) {
		line = str;
	} else {
		std::cerr << "Unexpected! EOF found." << std::endl;
		close( fd );
		return -1;
	}

	lock.l_type = F_UNLCK;
	if( fcntl( fd, F_SETLK, &lock ) < 0 ) {
		perror( "Unlock failed." );
		close( fd );
		return -1;
	}
	
	close( fd );
	return 0;
}

// eof

