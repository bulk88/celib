crtdir="N:/Programme/Microsoft Visual Studio/VC98/CRT/SRC"

crtfiles="localtim.c asctime.c ctime.c gmtime.c ftime.c 
time.c dtoxtime.c mktime.c tzset.c utime.c"

rm -f ./diffdir/*

echo "@echo off" >>./diffdir/xpatch.bat
echo "set VCCRTSRC=N:\\\\Programme\\\\Microsoft Visual Studio\\\\VC98\\\\CRT\\\\SRC" >>./diffdir/xpatch.bat

for f in $crtfiles; do
  cp "$crtdir/$f" "./$f.orig"
  diff -c  "./$f.orig" "./$f" >./diffdir/$f.diff
  rm "./$f.orig"

  echo "copy \"%VCCRTSRC%\\\\$f\" \"$f\"" >>./diffdir/xpatch.bat
  echo "patch $f $f.diff" >>./diffdir/xpatch.bat
done

cp diffdir/* .
 
