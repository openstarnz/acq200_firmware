#!/usr/bin/perl
# acq32-mknod.pl - builds acq32 device nodes from proc info

use getBoards;

$REVID = '$Revision: 1.30.4.5 $';

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

$DEVFMT   = "$root/acq32.%d.%-5s";
$JFMT     = "mknod " . $DEVFMT . " c %d %3d ;# %s\n";
$JFN      = '/tmp/'.$model.'.job';

$JFMT2    = "mknod /dev/acq200/acq200.%d.%02d c %d %3d;# %s\n";
$JFMT3    = "mknod /dev/acq200/acq200.%d.%s c %d %3d;# %s\n";


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

    if ($model =~ /acq32/ ){
	if ( !$done_generic ){
	    printf JF ( $JFMT, 0, "raw", $M, $DEVRAW, "Board 0 Raw generic");
	    $done_generic++;
	}

	printf JF ( $JFMT, 
		    $refb->{devnum}+1,
		    "flash", $M, $DEVFLASH, "Board $bn Flash RESOURCE ORDER" );
    }

    if ( $refb->{model} =~ /BLANK/ ){
	next;
    }
    printf JF ( $JFMT, $bn, "raw",   $M, $DEVRAW,   "Board $bn Raw"   );
    printf JF ( $JFMT, $bn, "dmabuf",$M, $DEVDMADB, "Board $bn dmabuf");
    printf JF ( $JFMT, $bn, "host",  $M, $DEVHOST,  "Board $bn host"  );
    printf JF ( $JFMT, $bn, "test",  $M, $TESTDEV,  "Board $bn test"  );
    printf JF ( $JFMT, $bn, "stat",  $M, $STATDEV,  "Board $bn status" );

    printf JF ( $JFMT, $bn, "XX",    $M, $DEVXX,    "Board $bn Row"   );

    for ( $ch = 1; $ch <= $refb->{num_channels}; $ch++ ){
        my $chs = sprintf '%02d', $ch;
        printf JF ( $JFMT, $bn, $chs, $MD, $ch-1, "Board $bn Channel $ch" );
    }
    

    if ( $refb->{model} =~ /ACQ32/ ){
	printf JF ( $JFMT, $bn, "AOi",  $M, $AOIDEV, "Board $bn AO immediate");
        printf JF ( $JFMT, $bn, "AOf",  $M, $AOFDEV, "Board $bn AO function" );
	printf JF ( $JFMT, $bn, "DOi",  $M, $DOIDEV, "Board $bn DO immediate");
        printf JF ( $JFMT, $bn, "DOf",  $M, $DOFDEV, "Board $bn DO function" );
        printf JF ( $JFMT, $bn, "AOfr", $M, $AOFRDEV, 
                    "Board $bn AO function recycler" );
        printf JF ( $JFMT, $bn, "DOfr", $M, $DOFRDEV,
			"Board $bn DO function recycler" );
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
	printf JF ( $JFMT3, $bn, "rsh",
                        $refb->{ACQ200MAJOR}, 128+101,
                        "acq200 response");

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

# then make master of the universe

    printf JF ( $JFMT, $master, 'm.ALL', $M, $MASTERALL,
                   "Board masters ALL"             );

    if ( !$made_master_all ){
    	my $dev = sprintf $DEVFMT, $master, $ident.".ALL";
	printf JF ("if [ ! -L $root/acq32.m.all ];then\n".
		   "    ln -s $dev $root/acq32.m.all ;# generic master all\n".
		   "fi\n");
	$made_master_all = 1;
	
    }
# then make potential masters for all possible combinations of boards

    foreach $refslaverec ( @DEVRECS ) {

	$slave = $refslaverec->{lognum};
        $ident .= $slave;

	if ( $slave < 6 ){
	    $scode |= 1<<($slave-1) | $smask;

	    if ( $ident ne $master_self_ident ){
		printf JF ( $JFMT, $master, $ident, $M, $mcode+$scode, 
			       "Board $master Master $ident" );
	    }else{
		printf JF ( "# not this one $ident\n" );
	    }
        }else{
	    printf JF ( "# can't slave board $slave (>=6)\n" );
	}
    }

    printf JF ( "# make potential masters for all boards from our number on\n" );

    my $got_to_me = 0;
    $scode = 0;
    $ident = 'm';

# this could be prettier and more effective, but it'll work
# .1. is a duplicate
# .3.m34 not covered

    SEARCHPARTNERS: foreach $refslaverec ( @DEVRECS ) {

        if ( !$got_to_me ) {
            if ( $refslaverec->{lognum} == $master ){
                $got_to_me = 1;
            }else{
                next SEARCHPARTNERS;
            }
        }
        
	$slave = $refslaverec->{lognum};
        $ident .= $slave;

	if ( $slave < 6 ){
	    $scode |= 1<<($slave-1) | $smask;

	    if ( $ident ne $master_self_ident ){
		printf JF ( $JFMT, $master, $ident, $M, $mcode+$scode, 
			       "Board $master Master $ident" );
	    }else{
		printf JF ( "# not this one $ident\n" );
	    }
        }else{
	    printf JF ( "# can't slave board $slave (>=6)\n" );
	}
    }
}
        
close JF;

if ( !$test ){
# Capture sh's stdout+stderr so we can see WHY the job file misbehaves
# when perl runs it (only 2 nodes get created) but succeeds when run
# manually with sh (all 130 nodes get created).  Shell metachars
# ('>', '2>&1') force perl to use "sh -c".
my $rc = system( "sh $JFN >/tmp/acq32.job.out 2>&1" );
print "mknod job: system returned $rc\n";
print "mknod job: node count after: ", `ls /dev/acq32/ 2>/dev/null | wc -l`;
print "mknod job: last 40 lines of output:\n";
system( "tail -40 /tmp/acq32.job.out" );
$rc == 0 or die "ERROR: jobfile failed";
}
