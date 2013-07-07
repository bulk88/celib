// test_pipe_child.c

#include "celib.h"

int
main(int argc, char **argv)
{
  //printf("StandardOutputHandle: %x\n", GetStdHandle(STD_OUTPUT_HANDLE));
  printf("Hello!\n");
  printf("I am a little pipe child.\n");
  printf("I am very stupid.\n");
  printf("Forgive my sins!\n");

  return 0;
}

#ifdef NO_CONSOLE
int
wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t *lpCommand, int nShow)
{
  xceinit(lpCommand);

  // client must call xceexit so the pipe is closed correctly!
  xceexit(main(__xceargc, __xceargv));

  return 0;
}
#endif
