@echo off
set VCCRTSRC=N:\Programme\Microsoft Visual Studio\VC98\CRT\SRC
copy "%VCCRTSRC%\localtim.c" "localtim.c"
patch localtim.c localtim.c.diff
copy "%VCCRTSRC%\asctime.c" "asctime.c"
patch asctime.c asctime.c.diff
copy "%VCCRTSRC%\ctime.c" "ctime.c"
patch ctime.c ctime.c.diff
copy "%VCCRTSRC%\gmtime.c" "gmtime.c"
patch gmtime.c gmtime.c.diff
copy "%VCCRTSRC%\ftime.c" "ftime.c"
patch ftime.c ftime.c.diff
copy "%VCCRTSRC%\time.c" "time.c"
patch time.c time.c.diff
copy "%VCCRTSRC%\dtoxtime.c" "dtoxtime.c"
patch dtoxtime.c dtoxtime.c.diff
copy "%VCCRTSRC%\mktime.c" "mktime.c"
patch mktime.c mktime.c.diff
copy "%VCCRTSRC%\tzset.c" "tzset.c"
patch tzset.c tzset.c.diff
copy "%VCCRTSRC%\utime.c" "utime.c"
patch utime.c utime.c.diff
