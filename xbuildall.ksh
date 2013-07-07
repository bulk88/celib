targets="
wince-arm-hpc-wce300
wince-x86-hpc-wce300
wince-arm-hpc-wce211
wince-sh3-hpc-wce211
wince-sh4-hpc-wce211
wince-mips-hpc-wce211
wince-mips-hpc-wce200
wince-sh3-hpc-wce200
wince-x86em-hpc-wce211
wince-arm-pocket-wce300
wince-sh3-pocket-wce300
wince-mips-pocket-wce300
wince-x86em-pocket-wce300
wince-sh3-palm-wce211
wince-mips-palm-wce211
wince-x86em-palm-wce211
wince-sh3-palm-wce201
wince-mips-palm-wce201
"

#clean=clean

for t in $targets; do
  nmake -f makefile.ce TARGET=$t $clean all || exit 1
done
