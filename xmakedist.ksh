# make dist archive

name=celib-3.11-dll-bin-all-platforms
xfind . -name *.dll|egrep -v 'debug|profile' >xfiles.lst

rm -f $name.tar.gz
tar -c -v -f $name.tar -T xfiles.lst
gzip -9 $name.tar
mv $name.tar.gz h:/freenet/new
