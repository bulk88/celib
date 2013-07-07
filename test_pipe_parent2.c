// test_pipe_parent2.c

#include "celib.h"

int
main(int argc, char **argv)
{
  char buf[512];
  FILE *f;

  if((f = popen("sort", "w")) == NULL)
    {
      perror("popen");
    }
  else
    {
      fprintf(f, "CDEFG\n");
      fprintf(f, "ABCDE\n");

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
