	.verstamp	3 10
	.option	pic0
	.file	44 "testfloat.c"
	.data	
	.align	3
	.align	0
$$2:
	.ascii	"w\X00i\X00n\X00s\X00o\X00c\X00k\X00.\X00d\X00l\X00l\X00\X00\X00"
	.data	
	.align	3
	.align	0
$$3:
	.ascii	"\\X00b\X00i\X00n\X00\\X00x\X001\X00.\X00t\X00x\X00t\X00\X00\X00"
	.data	
	.align	3
	.align	0
$$4:
	.ascii	"O\X00p\X00e\X00n\X00\X00\X00"
	.data	
	.align	3
	.align	0
$$5:
	.ascii	"O\X00p\X00e\X00n\X00\X00\X00"
	.data	
	.align	3
	.align	0
$$6:
	.ascii	"Z\X00=\X00=\X000\X00\X00\X00"
	.data	
	.align	3
	.align	0
$$7:
	.ascii	"Z\X00=\X00=\X000\X00\X00\X00"
	.data	
	.align	2
	.align	0
$$8:
	.ascii	"%\X00f\X00\X00\X00"
	.data	.drectve
	.align	2
	.align	0
__drectve:
	.ascii	"-defaultlib:MSVCRT -defaultlib:coredll.lib "
	.extern	__imp_CreateFileW 4
	.extern	__imp_swprintf 4
	.extern	__imp_GetProcAddressW 4
	.extern	__imp_ReadFile 4
	.extern	__imp_GetFileSize 4
	.extern	__imp_malloc 4
	.extern	__imp_CloseHandle 4
	.text	
	.align	2
	.file	44 "testfloat.c"
	.globl	testexports
	.loc	44 12
#line 12 "testfloat.c"	{
	.ent	testexports 2
testexports:
	.option	O0
	subu	$29, 464
	sw	$31, 36($29)
	.mask	0x80000000, -428
	.frame	$29, 464, $31
	.prologue	0
	.bgnb	0
	.loc	44 13
#line 13 "testfloat.c"	  HANDLE hFile;
	.loc	44 14
#line 14 "testfloat.c"	  HANDLE hMod;
	.loc	44 15
#line 15 "testfloat.c"	  FARPROC proc;
	.loc	44 16
#line 16 "testfloat.c"	  char *buf;
	.loc	44 17
#line 17 "testfloat.c"	  char *p;
	.loc	44 18
#line 18 "testfloat.c"	  DWORD size;
	.loc	44 19
#line 19 "testfloat.c"	  DWORD dwRead;
	.loc	44 20
#line 20 "testfloat.c"	  char name[126];
	.loc	44 21
#line 21 "testfloat.c"	  WCHAR wname[126];
	.loc	44 22
#line 22 "testfloat.c"	  char *s, *d;
	.loc	44 25
#line 23 "testfloat.c"	
#line 24 "testfloat.c"	  //hMod = LoadLibraryW(L"coredll.dll");
#line 25 "testfloat.c"	  hMod = LoadLibraryW(L"winsock.dll");
	la	$4, $$2
	.livereg	0x0800000E,0x00000000
	jal	LoadLibraryW
	sw	$2, 460($29)
	.loc	44 27
#line 26 "testfloat.c"	
#line 27 "testfloat.c"	  if((hFile = CreateFileW(L"\\bin\\x1.txt", GENERIC_READ, 0, NULL,
	.loc	44 28
#line 28 "testfloat.c"				  OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL)) == NULL)
	la	$4, $$3
	li	$5, -2147483648
	move	$6, $0
	move	$7, $0
	li	$14, 3
	sw	$14, 16($29)
	li	$15, 128
	sw	$15, 20($29)
	sw	$0, 24($29)
	.livereg	0x0F00000E,0x00000000
	lw	$24, __imp_CreateFileW
	jal	$24
	sw	$2, 456($29)
	lw	$25, 456($29)
	bne	$25, 0, $32
	.loc	44 29
#line 29 "testfloat.c"	    {
	.loc	44 30
#line 30 "testfloat.c"	      MessageBoxW(NULL, L"Open", L"Open", MB_OK);
	move	$4, $0
	la	$5, $$5
	la	$6, $$4
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	MessageBoxW
	.loc	44 31
#line 31 "testfloat.c"	      return FALSE;
	move	$2, $0
	b	$41
	.loc	44 32
#line 32 "testfloat.c"	    }
	.loc	44 34
#line 33 "testfloat.c"	
#line 34 "testfloat.c"	  size = GetFileSize(hFile, NULL);
$32:
	lw	$4, 456($29)
	move	$5, $0
	.livereg	0x0C00000E,0x00000000
	lw	$8, __imp_GetFileSize
	jal	$8
	sw	$2, 452($29)
	.loc	44 35
#line 35 "testfloat.c"	  buf = malloc(size + 1);
	lw	$4, 452($29)
	addu	$4, $4, 1
	.livereg	0x0800000E,0x00000000
	lw	$9, __imp_malloc
	jal	$9
	sw	$2, 448($29)
	.loc	44 36
#line 36 "testfloat.c"	  ReadFile(hFile, buf, size, &dwRead, NULL);
	lw	$4, 456($29)
	lw	$5, 448($29)
	lw	$6, 452($29)
	addu	$7, $29, 444
	sw	$0, 16($29)
	.livereg	0x0F00000E,0x00000000
	lw	$10, __imp_ReadFile
	jal	$10
	.loc	44 37
#line 37 "testfloat.c"	  CloseHandle(hFile);
	lw	$4, 456($29)
	.livereg	0x0800000E,0x00000000
	lw	$11, __imp_CloseHandle
	jal	$11
	.loc	44 38
#line 38 "testfloat.c"	  buf[size] = 0;
	lw	$12, 448($29)
	lw	$13, 452($29)
	addu	$14, $12, $13
	sb	$0, 0($14)
	.loc	44 40
#line 39 "testfloat.c"	
#line 40 "testfloat.c"	  for(s = buf, d = name; *s;)
	lw	$15, 448($29)
	sw	$15, 440($29)
	addu	$24, $29, 304
	sw	$24, 436($29)
	lb	$25, 0($15)
	beq	$25, 0, $40
$33:
$34:
	.loc	44 41
#line 41 "testfloat.c"	    {
	.loc	44 42
#line 42 "testfloat.c"	      if(*s == '\n')
	lw	$8, 440($29)
	lb	$9, 0($8)
	bne	$9, 10, $37
	.loc	44 43
#line 43 "testfloat.c"		{
	.loc	44 44
#line 44 "testfloat.c"		  if(d[-1] == '\r')
	lw	$10, 436($29)
	lb	$11, -1($10)
	bne	$11, 13, $35
	.loc	44 45
#line 45 "testfloat.c"		    d[-1] = 0;
	lw	$12, 436($29)
	sb	$0, -1($12)
	.loc	44 46
#line 46 "testfloat.c"		  *d = 0;
$35:
	lw	$13, 436($29)
	sb	$0, 0($13)
	.loc	44 48
#line 47 "testfloat.c"	
#line 48 "testfloat.c"		  MultiByteToWideChar(CP_ACP, 0, name, -1, wname, sizeof(wname)/2);
	move	$4, $0
	move	$5, $0
	addu	$6, $29, 304
	li	$7, -1
	addu	$14, $29, 48
	sw	$14, 16($29)
	li	$24, 126
	sw	$24, 20($29)
	.livereg	0x0F00000E,0x00000000
	jal	MultiByteToWideChar
	.loc	44 49
#line 49 "testfloat.c"		  proc = GetProcAddressW(hMod, wname);
	lw	$4, 460($29)
	addu	$5, $29, 48
	.livereg	0x0C00000E,0x00000000
	lw	$15, __imp_GetProcAddressW
	jal	$15
	sw	$2, 44($29)
	.loc	44 50
#line 50 "testfloat.c"		  if(proc == NULL)
	lw	$25, 44($29)
	bne	$25, 0, $36
	.loc	44 51
#line 51 "testfloat.c"		    {
	.loc	44 52
#line 52 "testfloat.c"		      MessageBoxW(NULL, wname, wname, MB_OK);
	move	$4, $0
	addu	$8, $29, 48
	move	$5, $8
	move	$6, $8
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	MessageBoxW
	.loc	44 53
#line 53 "testfloat.c"		      return FALSE;
	move	$2, $0
	b	$41
	.loc	44 54
#line 54 "testfloat.c"		    }
	.loc	44 55
#line 55 "testfloat.c"		  d = name;
$36:
	addu	$9, $29, 304
	sw	$9, 436($29)
	.loc	44 56
#line 56 "testfloat.c"		  s++;
	lw	$10, 440($29)
	addu	$11, $10, 1
	sw	$11, 440($29)
	.loc	44 57
#line 57 "testfloat.c"		}
	.loc	44 58
#line 58 "testfloat.c"	      else
	b	$38
$37:
	.loc	44 59
#line 59 "testfloat.c"		{
	.loc	44 60
#line 60 "testfloat.c"		  *d++ = *s++;
	lw	$12, 440($29)
	lb	$13, 0($12)
	lw	$14, 436($29)
	sb	$13, 0($14)
	lw	$24, 436($29)
	addu	$15, $24, 1
	sw	$15, 436($29)
	lw	$25, 440($29)
	addu	$8, $25, 1
	sw	$8, 440($29)
	.loc	44 61
#line 61 "testfloat.c"		}
$38:
	.loc	44 62
#line 62 "testfloat.c"	    }
	lw	$9, 440($29)
	lb	$10, 0($9)
	bne	$10, 0, $34
$39:
$40:
	.loc	44 64
#line 63 "testfloat.c"	
#line 64 "testfloat.c"	  return TRUE;
	li	$2, 1
	b	$41
	.loc	44 65
#line 65 "testfloat.c"	}
$41:
	.endb	0
	b	$42
$42:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 36($29)
	.set	 noreorder
	j	$31
	addu	$29, 464
	.set	 reorder
	.end	
	.text	
	.align	2
	.file	44 "testfloat.c"
	.globl	wWinMain
	.loc	44 69
#line 66 "testfloat.c"	
#line 67 "testfloat.c"	int
#line 68 "testfloat.c"	wWinMain()
#line 69 "testfloat.c"	{
	.ent	wWinMain 2
wWinMain:
	.option	O0
	subu	$29, 312
	sw	$31, 20($29)
	.mask	0x80000000, -292
	.frame	$29, 312, $31
	.prologue	0
	.bgnb	0
	.loc	44 70
#line 70 "testfloat.c"	  double x, y, z;
	.loc	44 71
#line 71 "testfloat.c"	  WCHAR buf[126];
	.loc	44 72
#line 72 "testfloat.c"	  HMODULE hMod;
	.loc	44 74
#line 73 "testfloat.c"	
#line 74 "testfloat.c"	  testexports();
	.livereg	0x0000000E,0x00000000
	jal	testexports
	.loc	44 75
#line 75 "testfloat.c"	  return 0;
	move	$2, $0
	b	$44
	.loc	44 82
#line 82 "testfloat.c"	  x = 10.0;
	li	$14, 0
	li	$15, 1076101120
	sw	$14, 304($29)
	sw	$15, 308($29)
	.loc	44 83
#line 83 "testfloat.c"	  y = 3.0;
	li	$24, 0
	li	$25, 1074266112
	sw	$24, 296($29)
	sw	$25, 300($29)
	.loc	44 86
#line 84 "testfloat.c"	
#line 85 "testfloat.c"	  // mul works with FPE
#line 86 "testfloat.c"	  z = x * y;
	lw	$4, 304($29)
	lw	$5, 308($29)
	lw	$6, 296($29)
	lw	$7, 300($29)
	.livereg	0x0F00000E,0x00000000
	jal	__dpmul
	sw	$2, 288($29)
	sw	$3, 292($29)
	.loc	44 89
#line 87 "testfloat.c"	
#line 88 "testfloat.c"	  // div works with FPE...
#line 89 "testfloat.c"	  z = x / y;
	lw	$4, 304($29)
	lw	$5, 308($29)
	lw	$6, 296($29)
	lw	$7, 300($29)
	.livereg	0x0F00000E,0x00000000
	jal	__dpdiv
	sw	$2, 288($29)
	sw	$3, 292($29)
	.loc	44 96
#line 96 "testfloat.c"	  if(z == 0.0)
	lw	$4, 288($29)
	lw	$5, 292($29)
	li	$6, 0
	li	$7, 0
	.livereg	0x0F00000E,0x00000000
	jal	__dpcmp
	sw	$2, 28($29)
	lw	$8, 28($29)
	bne	$8, 0, $43
	.loc	44 97
#line 97 "testfloat.c"	    MessageBoxW(NULL, L"Z==0", L"Z==0", MB_OK);
	move	$4, $0
	la	$5, $$7
	la	$6, $$6
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	MessageBoxW
	.loc	44 118
#line 118 "testfloat.c"	  swprintf(buf, L"%f", z);
$43:
	addu	$4, $29, 32
	la	$5, $$8
	lw	$6, 288($29)
	lw	$7, 292($29)
	.livereg	0x0F00000E,0x00000000
	lw	$9, __imp_swprintf
	jal	$9
	.loc	44 120
#line 119 "testfloat.c"	
#line 120 "testfloat.c"	  MessageBoxW(NULL, buf, buf, MB_OK);
	move	$4, $0
	addu	$10, $29, 32
	move	$5, $10
	move	$6, $10
	move	$7, $0
	.livereg	0x0F00000E,0x00000000
	jal	MessageBoxW
	.loc	44 122
#line 121 "testfloat.c"	
#line 122 "testfloat.c"	  return 0;
	move	$2, $0
	b	$44
	.loc	44 123
#line 123 "testfloat.c"	}
$44:
	.endb	0
	b	$45
$45:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 20($29)
	.set	 noreorder
	j	$31
	addu	$29, 312
	.set	 reorder
	.end	
