#!/usr/bin/perl
# acq32-mknod.pl - builds acq32 device nodes from proc info

use FindBin qw($Bin);
use lib $Bin;		# getBoards.pm lives beside this script; '.' is not in @INC on perl >= 5.26
use getBoards;

$REVID = '$Revision: 1.1.2.1 $';

print "acq32-mknod $REVID - signing on\n";


if ($#ARGV < 3 ){
    die "acq32-mknod.pl new calling convention root procroot model slot\n";
}

$root     = $ARGV[0];
$procroot = $ARGV[1];
$model    = $ARGV[2];
$slotbased= $ARGV[3];
$test     = $ARGV[4] eq "test";

print "acq32-mknod.pl:root=$root procroot=$procroot model=$model ".
      "slotbased=$slotbased\n";

### Main code body starts here


# minor node coding consts - taken from acq32.h

$MASTER   = 0x80;
$MASTERALL= 0xff;
$SPECIAL  = 0x40;
$CHANNEL  = 0x00;



$DEVXX    = $SPECIAL + 0x3f;
$DEVRAW   = $SPECIAL + 1;
$DEVDMADB = $SPECIAL + 2;
$DEVHOST  = $SPECIAL + 3;

$AOIDEV   = $SPECIAL + 4;
$AOFDEV   = $SPECIAL + 5;
$DOIDEV   = $SPECIAL + 6;
$DOFDEV   = $SPECIAL + 7;

$AOFRDEV  = $SPECIAL + 8;
$DOFRDEV  = $SPECIAL + 9;
$STATDEV  = $SPECIAL + 10;

$TESTDEV  = $SPECIAL + 15;
$DEVFLASH = $SPECIAL + 0;

$DEVFMT   = "$root/wav232.%d.%-5s";
$JFMT     = "mknod " . $DEVFMT . " c %d %3d ;# %s\n";
$JFN      = '/tmp/'.$model.'.job';

$JFMT2    = "mknod /dev/acq200/acq200.%d.%02d c %d %3d;# %s\n";
$JFMT3    = "mknod /dev/acq200/acq200.%d.%s c %d %3d;# %s\n";

$JFMTW    = "mknod /dev/wav232/%d/%d/%02d c %d %3d;# %s\n";

$nboards = getBoards( $procroot, $slotbased, $model, $test );
open JF, ">$JFN" or
    die "acq32-mknod:ERROR failed to create job file";

if ( $nboards == 0 ){
    printf JF ( "# No boards found - make flash devices in case blank\n" );
#print "after getBoards() nboards $nboards DEVRECS $#DEVRECS\n";
}

foreach $refb ( @DEVRECS ){

#print "refb $refb\n";

    my $bn = $refb->{lognum};
    my $M  = $refb->{MAJOR};
    my $MD = $refb->{DATA_MAJOR};

    printf JF ("rm -f $DEVFMT ;#%s\n", $bn, '*', 'remove any existing def');


    if ( $refb->{model} =~ /BLANK/ ){
	next;
    }
 
    if ( $refb->{ACQ200MAJOR} ){
	for ( $file = 1; $file != 9; ++$file ){
	    printf JF ( $JFMT2, $bn, $file, 
			$refb->{ACQ200MAJOR}, 128+$file-1, 
			"acq200 file tfr $file");
	}
	printf JF ( $JFMT3, $bn, "rsh-to",
                        $refb->{ACQ200MAJOR}, 128+99,
                        "acq200 command");
	printf JF ( $JFMT3, $bn, "rsh-out",
                        $refb->{ACQ200MAJOR}, 128+100,
                        "acq200 response");

# data devices T{0:7}, C{1:32}

	for ( my $track = 0; $track <= 7; ++$track ){
	    printf JF ( "mkdir -p $root/%d/%d\n", $bn, $track );
	    for ( my $ch = 1; $ch <= 32; ++$ch ){
		printf JF ( $JFMTW, $bn, $track, $ch, $refb->{WAVMAJOR},
			    ($track<<5)|($ch-1), 
			    "B:$bn t:$track c:$ch" ); 
	    }
	}
    }
    if ( $refb->{WAVMAJOR} ){
	printf JF ( "# WAV232 placeholder here\n" );
    }
}

# build the master device .. assume board 1 masters the rest

foreach $refb ( @DEVRECS ) {

    my $master= $refb->{lognum};
    my $ident = 'm';
    my $scode = 0;
    my $mcode = $MASTER;
    my $M = $refb->{MAJOR};

# first make a master for myself only

    my $master_self_ident =  $ident.$master;
    printf JF ( $JFMT, $master, $master_self_ident, $M, $mcode,
                   "Board masters self only"             );


}
        
close JF;

if ( !$test ){
@args = ( "chmod", "u+x", "$JFN" );
system( @args ) == 0 
   or die "ERROR: failed chmod";

@args = ( "$JFN" );
system( @args ) == 0 
   or die "ERROR: jobfile failed";
}
