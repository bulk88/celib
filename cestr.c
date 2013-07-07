// cestr.c
// 
// Time-stamp: <21/12/01 23:28:00 keuchel@netwave.de>

#include "celib.h"

char *
xcestrrchr(const char * string, int ch)
{
  char *start = (char *)string;

  while (*string++)
    ;
  while (--string != start && *string != (char) ch)
    ;

  if (*string == (char) ch)
    return( (char *) string );

  return(NULL);
}

char *
xcestrdup (const char *string)
{
  char *memory;

  if (!string)
    return(NULL);

  if ((memory = malloc(strlen(string) + 1)))
    return(strcpy(memory,string));

  return(NULL);
}

char *
xcestrndup (const char *string, int len)
{
  char *memory;

  if(!string)
    return(NULL);

  if((memory = malloc(len + 1)) == NULL)
    return NULL;

  memory[len] = 0;
  strncpy(memory, string, len);

  return memory;
}

int 
xcestrnicmp (const char * first, const char * last, size_t count)
{
  int f,l;

  if ( count )
    {
      do {
	if ( ((f = (unsigned char)(*(first++))) >= 'A') &&
	     (f <= 'Z') )
	  f -= 'A' - 'a';

	if ( ((l = (unsigned char)(*(last++))) >= 'A') &&
	     (l <= 'Z') )
	  l -= 'A' - 'a';

      } while ( --count && f && (f == l) );

      return( f - l );
    }

  return( 0 );
}

int 
xcestrncasecmp (const char * first, const char * last, size_t count)
{
  return xcestrnicmp (first, last, count);
}

char *
xcestrupr(char *string)
{
  char *cp;

  for (cp=string; *cp; ++cp)
    {
      if ('a' <= *cp && *cp <= 'z')
	*cp += 'A' - 'a';
    }

  return(string);
}

char *
xcestrlwr(char *string)
{
  char *cp;

  for (cp=string; *cp; ++cp)
    {
      if ('A' <= *cp && *cp <= 'Z')
	*cp += 'a' - 'A';
    }

  return(string);
}

int 
xcestricmp (const char * dst, const char * src)
{
  return(xcestrcmpi(dst,src));
}

int 
xcestrcasecmp (const char * dst, const char * src)
{
  return(xcestrcmpi(dst,src));
}

int 
xcestrcmpi(const char * dst, const char * src)
{
  int f,l;

  do {
    if ( ((f = (unsigned char)(*(dst++))) >= 'A') && (f <= 'Z') )
      f -= ('A' - 'a');
    
    if ( ((l = (unsigned char)(*(src++))) >= 'A') && (l <= 'Z') )
      l -= ('A' - 'a');
  } while ( f && (f == l) );

  return(f - l);
}

double
xcestrtod(const char *s, char **errorptr)
{
  double d;
  wchar_t *ws;
  wchar_t *werrorptr = NULL;
  int len = strlen(s) + 1;

  ws = malloc(len * sizeof(wchar_t));

  MultiByteToWideChar(CP_ACP, 0, s, -1, ws, len);

  d = wcstod(ws, &werrorptr);

  if(errorptr)
    {
      if(werrorptr)
	{
	  *errorptr = (char *) s + (werrorptr - ws);
	}
      else
	{
	  *errorptr = NULL;
	}
    }

  free(ws);

  return d;
}

long
xcestrtol(const char *s, char **errorptr, int base)
{
  long l;
  wchar_t *ws;
  wchar_t *werrorptr = NULL;
  int len = strlen(s) + 1;

  ws = malloc(len * sizeof(wchar_t));

  MultiByteToWideChar(CP_ACP, 0, s, -1, ws, len);

  l = wcstol(ws, &werrorptr, base);

  if(errorptr)
    {
      if(werrorptr)
	{
	  *errorptr = (char *) s + (werrorptr - ws);
	}
      else
	{
	  *errorptr = NULL;
	}
    }

  free(ws);

  return l;
}

unsigned long
xcestrtoul(const char *s, char **errorptr, int base)
{
  unsigned long l;
  wchar_t *ws;
  wchar_t *werrorptr = NULL;
  int len = strlen(s) + 1;

  // was malloc
  ws = _alloca(len * sizeof(wchar_t));

  MultiByteToWideChar(CP_ACP, 0, s, -1, ws, len);

  l = wcstoul(ws, &werrorptr, base);

  if(errorptr)
    {
      if(werrorptr)
	{
	  *errorptr = (char *) s + (werrorptr - ws);
	}
      else
	{
	  *errorptr = NULL;
	}
    }

  return l;
}

int 
xcestrcoll(const char *string1, const char *string2)
{
  return strcmp(string1, string2);
}

double 
xceatof(const char *s)
{
  return xcestrtod(s, NULL);
}
