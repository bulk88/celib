# This is a makefile sample for exe files
#
# wget makefile for windows ce
#
# Time-stamp: <20/08/01 09:44:50 keuchel@w2k>

# NOTE: Change to path of celib sources

#CELIBDIR  = h:/src/wince/celib-palm
CELIBDIR   = h:/src/wince/celib-palm-3.0

# NOTE: Change to path of console sources

CONSOLEDIR = h:/src/wince/w32console

DEBUG = 0

# NOTE: Uncomment the machine you are compiling for

!if "$(MACHINE)" == ""
MACHINE=wince-arm-hpc-wce300
#MACHINE=wince-sh3-hpc-wce211
#MACHINE=wince-mips-hpc-wce211
#MACHINE=wince-arm-pocket-wce300
#MACHINE=wince-mips-pocket-wce300
#MACHINE=wince-sh3-pocket-wce300
#MACHINE=wince-x86em-pocket-wce300
#MACHINE=wince-mips-palm-wce211
#MACHINE=wince-sh3-palm-wce211
#MACHINE=wince-x86em-palm-wce211
!endif

SRCDIR     = .
TARGETDIR  = $(MACHINE)

# NOTE: Set to the path of you SDK

WCEROOT    = N:\Windows CE Tools

######################################################################
# machines

!if "$(MACHINE)" == "wince-arm-hpc-wce300"
CC         = clarm.exe
ARCH       = ARM
CPU        = ARM
TARGETCPU  = ARM
CEVersion  = 300
OSVERSION  = WCE300
PLATFORM   = HPC2000
MCFLAGS    = -D _MT -D _DLL -D ARM -D arm -D _arm_ -D _ARM_ \
             -DPROCESSOR_ARM -DPALM_SIZE 
SUBSYS	   = -subsystem:windowsce,3.00
!endif

!if "$(MACHINE)" == "wince-sh3-hpc-wce211"
CC         = shcl.exe
ARCH       = SH3
CPU        = SH3
TARGETCPU  = SH3
CEVersion  = 211
OSVERSION  = WCE211
PLATFORM   = MS HPC Pro
MCFLAGS    = -D _DLL -D _MT -DSH3 -D_SH3_ -DSHx -DPROCESSOR_SH3 -DPALM_SIZE
SUBSYS	   = -subsystem:windowsce,2.11
!endif

!if "$(MACHINE)" == "wince-mips-hpc-wce211"
CC         = clmips.exe
ARCH       = MIPS
CPU        = MIPS
TARGETCPU  = MIPS
CEVersion  = 211
OSVERSION  = WCE211
PLATFORM   = MS HPC Pro
MCFLAGS    = -D _MT -D _DLL \
             -D MIPS -D mips -D _MIPS_ -D _mips_ -DPROCESSOR_MIPS \
             -D PALM_SIZE
SUBSYS	   = -subsystem:windowsce,2.11
!endif

!if "$(MACHINE)" == "wince-mips-palm-wce211"
CC         = clmips.exe
ARCH       = MIPS
CPU        = MIPS
TARGETCPU  = MIPS
CEVersion  = 211
OSVERSION  = WCE211
PLATFORM   = ms palm size pc
MCFLAGS    = -DMIPS -D_MIPS_ -DPROCESSOR_MIPS -D PALM_SIZE -D _DLL -D _MT
SUBSYS	   = -subsystem:windowsce,2.11
!endif

!if "$(MACHINE)" == "wince-sh3-palm-wce211"
CC         = shcl.exe
ARCH       = SH3
CPU        = SH3
TARGETCPU  = SH3
CEVersion  = 211
OSVERSION  = WCE211
PLATFORM   = ms palm size pc
MCFLAGS    = -D _MT -D _DLL -DSH3 -D_SH3_ -DSHx -DPROCESSOR_SH3 -DPALM_SIZE
SUBSYS	   = -subsystem:windowsce,2.11
!endif

!if "$(MACHINE)" == "wince-x86em-palm-wce211"
CC         = cl.exe
ARCH       = X86EM
CPU        = X86
TARGETCPU  = X86
CEVersion  = 211
OSVERSION  = WCE211
PLATFORM   = ms palm size pc
MCFLAGS    = -MDd -DX86 -D_X86_ -DPROCESSOR_X86 \
             -D_WIN32_WCE_EMULATION -DPALM_SIZE
MACH       = -machine:x86
SUBSYS     = -subsystem:windows
!endif

!if "$(MACHINE)" == "wince-x86em-pocket-wce300"
CC         = cl.exe
ARCH       = X86EM
CPU        = X86
TARGETCPU  = X86
CEVersion  = 300
OSVERSION  = WCE300
PLATFORM   = MS Pocket PC
MCFLAGS    = -DX86 -D_X86_ -DPROCESSOR_X86 -D _MT -D _DLL \
             -D_WIN32_WCE_EMULATION -DPALM_SIZE -DPOCKET_SIZE
MACH       = -machine:x86
SUBSYS     = -subsystem:windows
!endif

!if "$(MACHINE)" == "wince-mips-pocket-wce300"
CC         = clmips.exe
ARCH       = MIPS
CPU        = MIPS
TARGETCPU  = MIPS
CEVersion  = 300
OSVERSION  = WCE300
PLATFORM   = MS Pocket PC
MCFLAGS    = -D MIPS -D mips -D _MIPS_ -D _mips_ -DPROCESSOR_MIPS \
             -D _MT -D _DLL -DPALM_SIZE -DPOCKET_SIZE
MACH       = -machine:mips
SUBSYS     = -subsystem:windowsce,3.00
!endif

!if "$(MACHINE)" == "wince-sh3-pocket-wce300"
CC         = shcl.exe
ARCH       = SH3
CPU        = SH3
TARGETCPU  = SH3
CEVersion  = 300
OSVERSION  = WCE300
PLATFORM   = MS Pocket PC
MCFLAGS    = -D _MT -D _DLL -DSH3 -D_SH3_ -DSHx -DPROCESSOR_SH3 \
             -DPALM_SIZE -DPOCKET_SIZE
MACH       = -machine:mips
SUBSYS     = -subsystem:windowsce,3.00
!endif

!if "$(MACHINE)" == "wince-arm-pocket-wce300"
CC         = clarm.exe
ARCH       = ARM
CPU        = ARM
TARGETCPU  = ARM
CEVersion  = 300
OSVERSION  = WCE300
PLATFORM   = MS Pocket PC
MCFLAGS    = -D ARM -D arm -D _ARM_ -D _arm_ -DPROCESSOR_ARM \
             -D _MT -D _DLL -DPALM_SIZE -DPOCKET_SIZE
MACH       = -machine:arm
SUBSYS     = -subsystem:windowsce,3.00
!endif

######################################################################
# common section

CELIBPATH  = $(CELIBDIR)/$(MACHINE)-release
#CELIBPATH  = $(CELIBDIR)/$(MACHINE)-debug

LDLIBPATH  = -libpath:$(CELIBPATH)

DEFS    = -D WINDOWS -D_WINDOWS \
          -D _WIN32_WCE=$(CEVersion) -DUNDER_CE=$(CEVersion)
CFLAGS  = -nologo -I . -I $(CELIBDIR)/inc $(DEFS) $(MCFLAGS) -D HAVE_CONFIG_H
LDFLAGS = -nologo -debug:full -pdb:none $(SUBSYS) \
          -libpath:$(CELIBPATH) -machine:$(CPU)

!if "$(DEBUG)" == "1"
CFLAGS  = $(CFLAGS)  -Od -Zi 
LDFLAGS = $(LDFLAGS) -debug:full -pdb:none
!endif

LIBS    = -nodefaultlib:oldnames.lib -nodefaultlib:libc.lib \
          corelibc.lib coredll.lib winsock.lib celib.lib

RCDEFS  = -DUNDER_CE=$(CEVersion) -DUNICODE -D_UNICODE 

# NOTE: The console objects, which automatically call main()

STARTOBJS  = $(CONSOLEDIR)/$(MACHINE)/wmain.obj \
             $(CONSOLEDIR)/$(MACHINE)/w32console.obj

# old sdk
#PATH=$(WCEROOT)\bin;$(WCEROOT)\$(OSVERSION)\bin;$(PATH)
# embedded tools sdk
PATH=N:\Programme\Microsoft eMbedded Tools\EVC\$(OSVERSION)\BIN;$(PATH)

INCLUDE=$(WCEROOT)\$(OSVERSION)\$(PLATFORM)\include
LIB=$(WCEROOT)\$(OSVERSION)\$(PLATFORM)\lib\$(TARGETCPU)

!message *****************************************************************
!message Building for $(TARGETCPU) $(PLATFORM) $(OSVERSION)
!message INCLUDE = $(INCLUDE)
!message LIB     = $(LIB)
!message *****************************************************************
!message

######################################################################

# NOTE: Put your objects here

OBJS = \
$(TARGETDIR)\cmpt.obj \
$(TARGETDIR)\connect.obj \
$(TARGETDIR)\fnmatch.obj \
$(TARGETDIR)\ftp-basic.obj \
$(TARGETDIR)\ftp-ls.obj \
$(TARGETDIR)\ftp-opie.obj \
$(TARGETDIR)\ftp.obj \
$(TARGETDIR)\getopt.obj \
$(TARGETDIR)\headers.obj \
$(TARGETDIR)\host.obj \
$(TARGETDIR)\html.obj \
$(TARGETDIR)\http.obj \
$(TARGETDIR)\init.obj \
$(TARGETDIR)\log.obj \
$(TARGETDIR)\main.obj \
$(TARGETDIR)\md5.obj \
$(TARGETDIR)\mswindows.obj \
$(TARGETDIR)\netrc.obj \
$(TARGETDIR)\rbuf.obj \
$(TARGETDIR)\recur.obj \
$(TARGETDIR)\retr.obj \
$(TARGETDIR)\url.obj \
$(TARGETDIR)\utils.obj \
$(TARGETDIR)\version.obj \

# NOTE: put the name of your EXE file here

EXE = $(MACHINE)\wget.exe

all: $(TARGETDIR) $(EXE)

$(EXE): $(OBJS)
	link $(LDFLAGS) -out:$(EXE) $(STARTOBJS) $(OBJS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)

# NOTE: Change paths if you want to copy automatically to device

!if "$(MACHINE)" == "wince-arm-hpc-wce300"
install: all
	cecopy pc:$(EXE) ce:/speicherkarte/bin
!endif

!if "$(MACHINE)" == "wince-sh3-hpc-wce211"
install: all
	cecopy pc:$(EXE) ce:/speicherkarte2/bin
!endif

!if "$(MACHINE)" == "wince-mips-hpc-wce211"
install: all
	cecopy pc:$(EXE) ce:/bin
!endif

makedist:
	rm -f wget-$(MACHINE).tar.gz
	sh -c "tar -c -f wget-$(MACHINE).tar $(MACHINE)/*.exe -C .. COPYING"
	gzip -9 wget-$(MACHINE).tar
	mv wget-$(MACHINE).tar.gz h:/freenet/new

$(TARGETDIR):
	mkdir $(TARGETDIR)

{$(SRCDIR)}.c{$(TARGETDIR)}.obj:
    $(CC) -c $(CFLAGS) -Fo$(TARGETDIR)\ $<

$(OBJS): config.h
