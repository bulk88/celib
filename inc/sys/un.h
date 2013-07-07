// sys/un.h - dummy unix socket file

#ifndef SYS_UN_H
#define SYS_UN_H 1

struct sockaddr_un {
  int sun_family;
  char sun_path[126];
};

#endif
