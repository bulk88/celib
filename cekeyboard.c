// cekeyboard.c
//
// Time-stamp: <10/09/01 04:53:18 keuchel@netwave.de>

#include "celib.h"
#include "ceutil.h"

typedef struct _keyentry
{
  int scancode;
  int vkey;
  int normal_char;
  int shift_char;
  int alt_char;
} KEYENTRY;

// -1 in virtual keycode means "not known yet"

KEYENTRY german_keyboard[] = {
  {0x01, 0x0, 0x0, 0x0}, /* VK_ESCAPE - no translation! */
  {0x02, '1', '1', '!'},
  {0x03, '2', '2', '"'},
  {0x04, '3', '3', '§'},
  {0x05, '4', '4', '$'},
  {0x06, '5', '5', '%'},
  {0x07, '6', '6', '&'},
  {0x08, '7', '7', '/', '{'},
  {0x09, '8', '8', '(', '['},
  {0x0a, '9', '9', ')', ']'},
  {0x0b, '0', '0', '=', '}'},
  {0x0c, 0xdb, 'ß', '?', '\\'},
  {0x0d, 0xdd, '´'}, // ´ (diacitic)
  {0x0e, 0x08, 0x08, 0x08, 0x08}, // Backspace
  {0x0f, 0x09, 0x09, 0x09, 0x09}, // Tab
  {0x10, 'Q', 'q', 'Q', '@'},
  {0x11, 'W', 'w', 'W'},
  {0x12, 'E', 'e', 'E'},
  {0x13, 'R', 'r', 'R'},
  {0x14, 'T', 't', 'T'},
  {0x15, 'Z', 'z', 'Z'},
  {0x16, 'U', 'u', 'U'},
  {0x17, 'I', 'i', 'I'},
  {0x18, 'O', 'o', 'O'},
  {0x19, 'P', 'p', 'P'},
  {0x1a, 0xba, 'ü', 'Ü'},
  {0x1b, 0x6b, '+', '*', '~'},
  {0x1c, 0xd, '\r', '\r', '\r'}, /* VK_RETURN */
  {0x1d, 0x11},			/* VK_CONTROL */
  {0x1e, 'A', 'a', 'A'},
  {0x1f, 'S', 's', 'S'},
  {0x20, 'D', 'd', 'D'},
  {0x21, 'F', 'f', 'F'},
  {0x22, 'G', 'g', 'G'},
  {0x23, 'H', 'h', 'H'},
  {0x24, 'J', 'j', 'J'},
  {0x25, 'K', 'k', 'K'},
  {0x26, 'L', 'l', 'L'},
  {0x27, 0xc0, 'ö', 'Ö'},
  {0x28, 0xde, 'ä', 'Ä'},
  {0x29, 0xdc, '^'},		/* ^ (diacritic) */
  {0x2a, 0x10},			/* VK_SHIFT */
  {0x2b, 0xbf, '#', '\''},
  {0x2c, 'Y', 'y', 'Y'},
  {0x2d, 'X', 'x', 'X'},
  {0x2e, 'C', 'c', 'C'},
  {0x2f, 'V', 'v', 'V'},
  {0x30, 'B', 'b', 'B'},
  {0x31, 'N', 'n', 'N'},
  {0x32, 'M', 'm', 'M'},
  {0x33, 0xbc, ',', ';'},
  {0x34, 0xbe, '.', ':'},
  {0x35, 0xbd, '-', '_'},
  {0x36, 0x1a},			/* ??? */
  {0x37, 0x6a},			/* ??? */
  {0x38, 0x12},			/* VK_MENU */
  {0x39, 0x20},			/* VK_SPACE */
  {0x4a, 0x6d},			/* ??? */
  {0x4b, 0x25},			/* ??? */
  {0x4c, 0x0c},			/* ??? */
  {0x4d, 0x27},			/* ??? */
  {0x4e, 0x6b},			/* + */
  {0x4f, 0x23},			/* ??? */
  {0x50, 0x28},			/* ??? */
  {0x51, 0x22},			/* ??? */
  {0x52, 0x2d},			/* ??? */
  {0x53, 0x6e, ',', ','},	/* This ok? */

  {0x56, 0xe2, '<', '>', '|'},

  {-1}
};

XCE_EXPORT int XCEAPI
XCEToAscii(UINT uVirtKey, UINT uScanCode, PBYTE lpbKeyState,
	   LPWORD lpwTransKey, UINT fuState)
{
  int i;
  KEYENTRY *keytab;

  keytab = german_keyboard;

  for(i = 0; keytab[i].vkey != -1; i++)
    {
      if(uVirtKey == keytab[i].vkey)
	{
	  if(lpbKeyState[VK_SHIFT])
	    *lpwTransKey = keytab[i].shift_char;
	  else if(lpbKeyState[VK_MENU])
	    *lpwTransKey = keytab[i].alt_char;
	  else
	    *lpwTransKey = keytab[i].normal_char;

	  if(*lpwTransKey == 0)
	    return 0;

	  return 1;
	}
    }

  return 0;
}

XCE_EXPORT SHORT XCEAPI
XCEVkKeyScan(unsigned char c)
{
  int i;
  KEYENTRY *keytab;
  SHORT res;

  keytab = german_keyboard;

  for(i = 0; keytab[i].vkey != -1; i++)
    {
      if(c == keytab[i].normal_char)
	{
	  res = keytab[i].vkey;
	  return res;
	}
      else if(c == keytab[i].shift_char)
	{
	  res = keytab[i].vkey | (1 << 8+0);
	  return res;
	}
      else if(c == keytab[i].alt_char)
	{
	  res = keytab[i].vkey | (1 << 8+3);
	  return res;
	}
    }

  return MAKEWORD(-1, -1);
}

XCE_EXPORT unsigned int
XCEUtil_VkToScancode(unsigned int vkey)
{
  int i;
  unsigned int res;
  KEYENTRY *keytab;

  // this should be set by a function!
  keytab = german_keyboard;

#if 0
  // only map type 2 is supported, maptype 0 returns 0!
  return MapVirtualKeyW(vkey, 0);
#endif

  for(i = 0; keytab[i].vkey != -1; i++)
    {
      if(vkey == keytab[i].vkey)
	{
	  res = keytab[i].scancode;
	  return res;
	}
    }
  return 0;
}
