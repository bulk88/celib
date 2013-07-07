# make source archive

dir=celib-palm-3.0
tar=celib-3.10-dll-src.tar

cd h:/src/wince
xfind $dir -type f | \
 egrep -i -v '(\.obj|\.exe|\.pdb|\.ilk|\.exp|~)$' |
 egrep -i -v '(/.*-debug|.*-profile/)' | 
 cat >$dir/xfiles.lst

rm -f $tar $tar.gz
tar -c -f $tar -T $dir/xfiles.lst
gzip -9 $tar
ls -l $tar.gz
mv $tar.gz h:/freenet/new
