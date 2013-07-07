# xprofile.pl - create profile report -*- cperl -*-
#
# Time-stamp: <03/11/01 17:15:28 keuchel@netwave.de>

# I have not tried loading symbols from pdb files. Using a map file
# seems to be easier.

$map   = "wince-arm-hpc-wce300-profile/celib.map";
$prof  = "profile.txt";

# enable fix of load addresses
$dll   = 1;
# semicolon seperated or report
$cvs   = 0;
# debugging
$debug = 0;

if($#ARGV == 1) {
  $map  = $ARGV[0];
  $prof = $ARGV[1];
}

open MAP, "<$map" or die "Cannot open mapfile $map";
while(<MAP>) {
    chomp;
    if(/Publics by Value/) {
      $header_seen = 1;
      next;
    }
    elsif(/Preferred load address is ([0-9a-f]+)/) {
      $image_loadaddr = hex($1);
    }

    next if(!$header_seen);

    ($dummy, $addr, $name, $rva, $flag, $file) = split(/[ ]+/);
    $addrtab{$rva} = $name;
    $filetab{$rva} = $file;
}
close MAP;

if ($dll) {
  # get the lowest address from profile and derive real load address
  $real_loadaddr = 0xffffffff;
  open PROF, "<$prof" or die "Cannot open profile $prof";
  while (<PROF>) {
    chomp;
    next if(/^\#/);
    ($addr, $count, $total, $avg) = split(/:/);
    $addr = hex($addr);
    $real_loadaddr = $addr if($addr < $real_loadaddr);
  }
  close PROF;
  
  $real_loadaddr &= 0xFFFF0000;
  $addrdiff = $image_loadaddr - $real_loadaddr;

  if($debug) {
    print "LoadAddr: ", (sprintf "%x", $image_loadaddr), "\n";
    print "MinAddr:  ", (sprintf "%x", $real_loadaddr), "\n";
    print "Diff:     ", (sprintf "%x", $addrdiff), "\n";
  }
}

format STDOUT_TOP =

**** PROFILE REPORT - TIME IS IN MS AND INCLUDES PROFILE AND CHILD TIME ****

RVA        COUNT      TIME   AVG FUNCTION               FILE
-----------------------------------------------------------------------------
.

format STDOUT =
@<<<<<<< @>>>>>> @>>>>>>>> @>>>> @<<<<<<<<<<<<<<<<<<<   @<<<<<<<<<<<<<
$addr,   $count, $total,   $avg, $func,                 $file
.

if ($cvs) {
  print "RVA;COUNT;TOTAL;AVG;FUNC;FILE\n";
}

open PROF, "<$prof" or die "Cannot open profile $prof";
while (<PROF>) {
  chomp;
  next if(/^\#/);
  ($addr, $count, $total, $avg) = split(/:/);
  # fix address
  $addr = sprintf "%08x", hex($addr) + $addrdiff if($dll);
  $func = $addrtab{$addr};
  $file = $filetab{$addr};
  # might be used for Excel...
  if ($cvs) {
    print "$addr;$count;$total;$avg;$func;$file\n";
  }
  else {
    write;
  }
}
close PROF;
