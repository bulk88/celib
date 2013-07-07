	.verstamp	3 10
	.option	pic0
	.file	43 "testfloatio.c"
	.data	
	.align	3
	.align	0
$$2:
	.ascii	"Mod: %x\X0A\X00"
	.data	
	.align	3
	.align	0
$$3:
	.ascii	"Cannot get version info\X00"
	.data	
	.align	3
	.align	0
$$4:
	.ascii	"%d.%d 0x%x\X0D\X0A%s\X00"
	.data	
	.align	3
	.align	0
$$5:
	.ascii	"%d %d %d %lf %lf %lf\X00"
	.data	
	.align	3
	.align	0
$$6:
	.ascii	"1 -2 3 1.23 4.66 -10.99\X00"
	.data	
	.align	3
	.align	0
$$7:
	.ascii	"%d: %d %d %d %f %f %f\X00"
	.data	.drectve
	.align	2
	.align	0
__drectve:
	.ascii	"-defaultlib:MSVCRT -defaultlib:coredll.lib "
	.text	
	.align	2
	.file	43 "testfloatio.c"
	.globl	wWinMain
	.loc	43 13
#line 13 "testfloatio.c"	{
	.ent	wWinMain 2
wWinMain:
	.option	O0
	subu	$29, 376
	sw	$31, 52($29)
	.mask	0x80000000, -324
	.frame	$29, 376, $31
	.prologue	0
	.bgnb	0
	.bgnb	252
	.loc	43 14
#line 14 "testfloatio.c"	  char buf[126];
	.loc	43 15
#line 15 "testfloatio.c"	  double x, y, z;
	.loc	43 16
#line 16 "testfloatio.c"	  int n;
	.loc	43 17
#line 17 "testfloatio.c"	  int i, j, k;
	.loc	43 18
#line 18 "testfloatio.c"	  HMODULE hMod;
	.loc	43 20
#line 19 "testfloatio.c"	
#line 20 "testfloatio.c"	  OSVERSIONINFO osinfo;
	.loc	43 22
#line 21 "testfloatio.c"	
#line 22 "testfloatio.c"	  hMod = XCEGetModuleHandleA(NULL);
	move	$4, $0
	.livereg	0x0800000E,0x00000000
	jal	XCEGetModuleHandleA
	sw	$2, 372($29)
	.loc	43 23
#line 23 "testfloatio.c"	  XCEShowMessageA("Mod: %x\n", hMod);
	la	$4, $$2
	lw	$5, 372($29)
	.livereg	0x0C00000E,0x00000000
	jal	XCEShowMessageA
	.loc	43 24
#line 24 "testfloatio.c"	  return 0;
	move	$2, $0
	b	$34
	.loc	43 26
#line 25 "testfloatio.c"	
#line 26 "testfloatio.c"	  if(!XCEGetVersionExA(&osinfo))
	addu	$4, $29, 96
	.livereg	0x0800000E,0x00000000
	jal	XCEGetVersionExA
	bne	$2, 0, $32
	.loc	43 27
#line 27 "testfloatio.c"	    XCEShowMessageA("Cannot get version info");
	la	$4, $$3
	.livereg	0x0800000E,0x00000000
	jal	XCEShowMessageA
	.loc	43 28
#line 28 "testfloatio.c"	  else
	b	$33
$32:
	.loc	43 29
#line 29 "testfloatio.c"	    XCEShowMessageA("%d.%d 0x%x\r\n%s",
	.loc	43 30
#line 30 "testfloatio.c"			    osinfo.dwMajorVersion, // 2
	.loc	43 31
#line 31 "testfloatio.c"			    osinfo.dwMinorVersion, // 11
	.loc	43 32
#line 32 "testfloatio.c"			    osinfo.dwPlatformId,   // 3 = VER_PLATFORM_WIN32_CE
	.loc	43 33
#line 33 "testfloatio.c"			    osinfo.szCSDVersion);
	la	$4, $$4
	lw	$5, 100($29)
	lw	$6, 104($29)
	lw	$7, 112($29)
	addu	$14, $29, 96
	addu	$15, $14, 20
	sw	$15, 16($29)
	.livereg	0x0F00000E,0x00000000
	jal	XCEShowMessageA
$33:
	.loc	43 35
#line 34 "testfloatio.c"	
#line 35 "testfloatio.c"	  x = 10.0;
	li	$24, 0
	li	$25, 1076101120
	sw	$24, 88($29)
	sw	$25, 92($29)
	.loc	43 36
#line 36 "testfloatio.c"	  y = 3.0;
	li	$8, 0
	li	$9, 1074266112
	sw	$8, 80($29)
	sw	$9, 84($29)
	.loc	43 37
#line 37 "testfloatio.c"	  z = x / y;
	lw	$4, 88($29)
	lw	$5, 92($29)
	lw	$6, 80($29)
	lw	$7, 84($29)
	.livereg	0x0F00000E,0x00000000
	jal	__dpdiv
	sw	$2, 72($29)
	sw	$3, 76($29)
	.loc	43 46
#line 46 "testfloatio.c"	  n = sscanf("1 -2 3 1.23 4.66 -10.99", "%d %d %d %lf %lf %lf",
	.loc	43 47
#line 47 "testfloatio.c"		     &i, &j, &k, &x, &y, &z);
	la	$4, $$6
	la	$5, $$5
	addu	$6, $29, 56
	addu	$7, $29, 60
	addu	$10, $29, 64
	sw	$10, 16($29)
	addu	$11, $29, 88
	sw	$11, 20($29)
	addu	$12, $29, 80
	sw	$12, 24($29)
	addu	$13, $29, 72
	sw	$13, 28($29)
	.livereg	0x0F00000E,0x00000000
	jal	palm_sscanf
	sw	$2, 68($29)
	.loc	43 49
#line 48 "testfloatio.c"	
#line 49 "testfloatio.c"	  XCEShowMessageA("%d: %d %d %d %f %f %f", n, i, j, k, x, y, z);
	la	$4, $$7
	lw	$5, 68($29)
	lw	$6, 56($29)
	lw	$7, 60($29)
	lw	$14, 64($29)
	sw	$14, 16($29)
	lw	$24, 88($29)
	lw	$25, 92($29)
	sw	$24, 24($29)
	sw	$25, 28($29)
	lw	$8, 80($29)
	lw	$9, 84($29)
	sw	$8, 32($29)
	sw	$9, 36($29)
	lw	$10, 72($29)
	lw	$11, 76($29)
	sw	$10, 40($29)
	sw	$11, 44($29)
	.livereg	0x0F00000E,0x00000000
	jal	XCEShowMessageA
	.loc	43 51
#line 50 "testfloatio.c"	
#line 51 "testfloatio.c"	  return 0;
	move	$2, $0
	b	$34
	.loc	43 52
#line 52 "testfloatio.c"	}
	.endb	0
$34:
	.endb	0
	b	$35
$35:
	.livereg	0x2000FF0E,0x00000FFF
	lw	$31, 52($29)
	.set	 noreorder
	j	$31
	addu	$29, 376
	.set	 reorder
	.end	
