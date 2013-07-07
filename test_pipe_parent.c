// test_pipe_parent.c

#include "celib.h"

int
main(int argc, char **argv)
{
  char buf[512];
  FILE *f;

  if((f = popen("/test_pipe_child", "r")) == NULL)
    {
      perror("popen");
    }
  else
    {
      while(fgets(buf, sizeof(buf), f))
	{
	  printf("%s", buf);
	}
      if(fclose(f) != 0)
	{
	  perror("fclose");
	}
    }

 done:
  // prevent console from closing...
  printf("Hit RETURN");
  fflush(stdout);
  fgets(buf, sizeof(buf), stdin);

  return 0;
}

#ifdef NO_CONSOLE
int
wWinMain(HINSTANCE hInst, HINSTANCE hPrevInst, wchar_t *lpCommand, int nShow)
{
  xceinit(lpCommand);

  xceexit(main(__xceargc, __xceargv));

  return 0;
}
#endif
