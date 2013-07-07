// celocale.c
//
// Time-stamp: <13/05/01 11:42:42 keuchel@w2k>

#include "celib.h"
#include "locale.h"

char *
xcesetlocale(int category, const char *locale)
{
  if(locale == NULL)
    return "C";

  if(strcmp(locale, "C") != 0)
    return NULL;

  return "C";
}


