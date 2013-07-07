# Sample makefile for static libraries
#
# Makefile for fileutil lib (wince)
#
# Time-stamp: <20/08/01 09:39:45 keuchel@w2k>

#CELIBDIR = H:/src/wince/celib-palm
CELIBDIR = H:/src/wince/celib-palm-3.0

DEFS = -DHAVE_CONFIG_H=1 -DWIN32=1 -D__WIN32__=1 -DCLOSEDIR_VOID=1 -I ..
YACC = bison -y

######################################################################

CFG = WINCE_RELEASESHARED
#CFG = WINCE_DEBUGSHARED

!if "$(TARGET)" == ""
TARGET=wince-arm-hpc-wce300
#TARGET=wince-sh3-hpc-wce211
#TARGET=wince-mips-hpc-ce211

#TARGET=wince-arm-pocket-wce300
#TARGET=wince-sh3-pocket-wce300
#TARGET=wince-mips-pocket-wce300
#TARGET=wince-x86em-pocket-wce300

#TARGET=wince-mips-palm-wce211
#TARGET=wince-x86em-palm-wce211
!endif

WCEROOT    = N:\Windows CE Tools
SDKROOT    = N:\Windows CE Tools

!if "$(TARGET)" == "wince-arm-hpc-wce300"
CC         = clarm.exe
PLATFORM   = HPC2000
TARGETCPU  = arm
MACHFLAGS  = -D ARM -D _ARM_ -D arm -D _arm_ -D _MT -D _DLL \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE300
CEVERSION  = 300
CESUBSYS   = windowsce,3.00 -machine:arm
!endif

!if "$(TARGET)" == "wince-x86em-pocket-wce300"
CC         = cl.exe
PLATFORM   = MS Pocket PC
TARGETCPU  = x86em
MACHFLAGS  = -D _X86_ -D_WIN32_WCE_EMULATION -D_MT -D_DLL \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE300
CEVERSION  = 300
CESUBSYS   = windows
!endif

!if "$(TARGET)" == "wince-arm-pocket-wce300"
CC         = clarm.exe
PLATFORM   = MS Pocket PC
TARGETCPU  = arm
MACHFLAGS  = -D _ARM_ -D ARM -D _DLL -D _MT \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE300
CEVERSION  = 300
CESUBSYS   = windowsce,3.00 -machine:arm
!endif

!if "$(TARGET)" == "wince-sh3-pocket-wce300"
CC         = shcl.exe
PLATFORM   = MS Pocket PC
TARGETCPU  = sh3
MACHFLAGS  = -D _SHx_ -D SHx -D _DLL -D _MT \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE300
CEVERSION  = 300
CESUBSYS   = windowsce,3.00 -machine:sh3
!endif

!if "$(TARGET)" == "wince-mips-pocket-wce300"
CC         = clmips.exe
PLATFORM   = MS Pocket PC
TARGETCPU  = mips
MACHFLAGS  = -D MIPS -D mips -D _MIPS_ -D _mips_ \
             -D _DLL -D _MT \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE300
CEVERSION  = 300
CESUBSYS   = windowsce,3.00 -machine:mips
!endif

!if "$(TARGET)" == "wince-mips-palm-wce211"
CC         = clmips.exe
PLATFORM   = MS Palm Size PC
TARGETCPU  = mips
MACHFLAGS  = -D _MT -D _DLL -D MIPS -D _MIPS_ -D mips -D _mips_ \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE211
CEVERSION  = 211
CESUBSYS   = windowsce,2.11
!endif

!if "$(TARGET)" == "wince-x86em-palm-wce211"
CC         = cl.exe
PLATFORM   = MS Palm Size PC
TARGETCPU  = x86em
MACHFLAGS  = -D _MT -D _DLL -D _X86_ -D_WIN32_WCE_EMULATION \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE211
CEVERSION  = 211
CESUBSYS   = windows
!endif

!if "$(TARGET)" == "wince-sh3-hpc-wce211"
CC         = shcl.exe
PLATFORM   = MS HPC Pro
TARGETCPU  = sh3
MACHFLAGS  = -D SHx -D SH3 -D_SH3_ -D _DLL -D _MT \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE211
CEVERSION  = 211
CESUBSYS   = windowsce,2.11
!endif

!if "$(TARGET)" == "wince-mips-hpc-wce211"
CC         = clmips.exe
PLATFORM   = MS HPC Pro
TARGETCPU  = mips
MACHFLAGS  = -D MIPS -D _mips_ -D _DLL -D _MT \
             -I $(CELIBDIR)/inc
OSVERSION  = WCE211
CEVERSION  = 211
CESUBSYS   = windowsce,2.11
!endif

# old sdk
#PATH=$(WCEROOT)\bin;$(WCEROOT)\$(OSVERSION)\bin;$(PATH)
# embedded tools sdk
PATH=N:\Programme\Microsoft eMbedded Tools\EVC\$(OSVERSION)\BIN;$(PATH)

INCLUDE=$(WCEROOT)/$(OSVERSION)/$(PLATFORM)/include
LIB=$(WCEROOT)/$(OSVERSION)/$(PLATFORM)/lib/$(TARGETCPU)

!message *****************************************************************
!message Building for $(TARGETCPU) $(PLATFORM) $(OSVERSION)
!message INCLUDE = $(INCLUDE)
!message LIB     = $(LIB)
!message *****************************************************************
!message

SRCDIR    = .
TARGETDIR = $(TARGET)

######################################################################

INC            = -I. -I.. 
COMMONCFLAGS   = -nologo $(INC) -DPOCKET_SIZE -DPALM_SIZE $(MACHFLAGS) \
	         $(DEFS)
COMMONCEDEFS   = -D_WINCE -DUNDER_CE=$(CEVERSION)
LIBFLAGS       = -nologo
LDFLAGS        = -nologo -incremental:no -debug:full -pdb:none -nodefaultlib \
                 -subsystem:$(CESUBSYS) $(LIBPATH)
CELIBS         = corelibc.lib coredll.lib winsock.lib

######################################################################

!if "$(CFG)" == "WINCE_RELEASESHARED"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -Od 
!endif

!if "$(CFG)" == "WINCE_DEBUGSHARED"
CFLAGS = $(COMMONCFLAGS) $(COMMONCEDEFS) -Od -Zi
!endif

######################################################################

OBJS = \
$(TARGETDIR)\getdate.obj \
$(TARGETDIR)\posixtm.obj \
$(TARGETDIR)\argmatch.obj \
$(TARGETDIR)\backupfile.obj \
$(TARGETDIR)\basename.obj \
$(TARGETDIR)\dirname.obj \
$(TARGETDIR)\error.obj \
$(TARGETDIR)\filemode.obj \
$(TARGETDIR)\full-write.obj \
$(TARGETDIR)\getopt.obj \
$(TARGETDIR)\getopt1.obj \
$(TARGETDIR)\getversion.obj \
$(TARGETDIR)\group-member.obj \
$(TARGETDIR)\idcache.obj \
$(TARGETDIR)\isdir.obj \
$(TARGETDIR)\long-options.obj \
$(TARGETDIR)\makepath.obj \
$(TARGETDIR)\modechange.obj \
$(TARGETDIR)\obstack.obj \
$(TARGETDIR)\safe-read.obj \
$(TARGETDIR)\savedir.obj \
$(TARGETDIR)\stripslash.obj \
$(TARGETDIR)\xgetcwd.obj \
$(TARGETDIR)\xmalloc.obj \
$(TARGETDIR)\xstrdup.obj \
$(TARGETDIR)\userspec.obj \
$(TARGETDIR)\yesno.obj \
$(TARGETDIR)\fileblocks.obj \
$(TARGETDIR)\euidaccess.obj \
$(TARGETDIR)\fnmatch.obj \
$(TARGETDIR)\fsusage.obj \
$(TARGETDIR)\mountlist.obj \
$(TARGETDIR)\stpcpy.obj \
$(TARGETDIR)\win32.obj

#$(TARGETDIR)\mktime.obj

######################################################################
LIBNAME = $(TARGETDIR)\libfileutil.lib

all: $(TARGETDIR) $(LIBNAME)

$(LIBNAME): $(OBJS)
	lib $(LIBFLAGS) -out:$(LIBNAME) $(OBJS)

install: all

clean:
	rm -f $(OBJS) $(LIBNAME)

# Since this directory contains two parsers, we have to be careful to avoid
# running two $(YACC)s during parallel makes.  See below.
getdate.c: getdate.y
	@echo expect 10 shift/reduce conflicts
	$(YACC) getdate.y
	mv y.tab.c getdate.c

# Make the rename atomic, in case sed is interrupted and later rerun.
# The artificial dependency on getdate.c keeps the two parsers from being
# built in parallel.  Enforcing this little bit of sequentiality lets
# everyone (even those without bison) still run mostly parallel builds.
posixtm.c: posixtm.y getdate.c
	$(YACC) posixtm.y
	mv y.tab.c posixtm.tab.c
	sh -c "sed -e 's/yy/zz/g' posixtm.tab.c > tposixtm.c"
	mv tposixtm.c posixtm.c
	rm -f posixtm.tab.c

$(TARGETDIR):
	mkdir $(TARGETDIR)

{$(SRCDIR)}.c{$(TARGETDIR)}.obj:
    $(CC) -c $(CFLAGS) -Fo$(TARGETDIR)\ $<
