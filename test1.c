// test1.c

#include "celib.h"
#include "ceshared.h"

void
test_pipes()
{
  HANDLE hIn, hOut;
  char buf[512];

  strcpy(buf, "Hello pipe!");

  if(XXCreatePipe(&hIn, &hOut, NULL, 1024) == FALSE)
    {
      fprintf(stderr, "CreatePipe\n");
      exit(1);
    }

  if(pipe_write(hOut, buf, strlen(buf)) != strlen(buf))
    {
      fprintf(stderr, "pipe_write");
      exit(1);
    }
  buf[0] = 0;
  if(pipe_read(hIn, buf, sizeof(buf)) < 0)
    {
      fprintf(stderr, "pipe_read");
      exit(1);
    }
  printf("%s\n", buf);
}

int
main(int argc, char **argv)
{
  char buf[512];

  // Wow, using stdio opens a window! Did not know that!
  fprintf(stderr, "*** CELib Tests ***\n\n");

  test_pipes();

 done:
  // prevent console from closing...
  printf("Hit RETURN");
  fflush(stdout);
  fgets(buf, sizeof(buf), stdin);

  return 0;
}

int
wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t *lpCommand, int nShow)
{
  xceinit(lpCommand);

  return main(__xceargc, __xceargv);
}
