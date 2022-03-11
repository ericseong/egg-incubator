The code is cloned from https://gist.github.com/laobubu/d6d0e9beb934b60b2e552c2d03e1409e.
W.r.t. the original code, some changes made are:
* correction on duplicated variable declaration
* add free() to buf malloc
* clean-up on the slots / fds
* Corrected. CR/LF were not properly counted when httpd.cpp parses http get/post header/body. 

See the commits for the details.

