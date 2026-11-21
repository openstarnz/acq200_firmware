#!/usr/bin/perl
# acq32-init.pl - does essential first time initialisation

use FindBin qw($Bin);
use lib $Bin;		# getBoards.pm lives beside this script; '.' is not in @INC on perl >= 5.26
use getBoards;

$REVID = '$Revision: 1.1.2.1 $';

$root     = $ARGV[0];
$procroot = $ARGV[1];
$model    = $ARGV[2];
$slotbased= $ARGV[3];
$test     = $ARGV[4] eq "test";

$wavroot="/dev/wav232/wav232";

print "wav232-init.pl $REVID - signing on slotbased $slotbased";

$nboards = getBoards( $procroot, $slotbased, $model, $test );

sub acqcmd {
        my($brd, $cmd) = @_;

        open MASTER, "+>> $wavroot.$brd.m$brd";
        print MASTER "$cmd\n";
        $ans = "";
        while (<MASTER>) {
            $ans .= $_;
        }
        close MASTER;
    
        return $ans;
}


sub makeChannelMask {
    my ( $refb ) = @_;
    my $mask = '1' x $refb->{num_channels} . '0' x (32-$refb->{num_channels});
    
    return $mask;
}

sub holdboard {
        my ( $refb ) = @_;
        my $bn = $refb->{lognum};

	return if fork;

	# wedge open a path to the device and sleep forever

	open HOLD, ">$wavroot.$bn.m$bn";
	sleep
}



sub initboard {
	my ( $refb ) = @_;
	my $bn = $refb->{lognum};

}


$first_board = 1;

sub doSiteSpecials {
        my ( $refb ) = @_;
        my $bn = $refb->{lognum};

# site specific init here - eg set up LEMO MASTER/SLAVE clocking
# we decide to make the first board the master, and flash an LED
# to indicate which one it is
# (user LED control available from 01-Dec-2003)
# WORKTODO: if it's a vanilla ACQ32, we need to detect this and 
# set routing accordingly.

	if ($refb->{ACQ200MAJOR}){
		print "This is an ACQ2xx\n";
	}else{
		print "Not an ACQ2xx\n";
		return;
	}
	if ($first_board){
		$first_board = 0;
	}else{
		;
	}
}

#
# RACE CONDITION BEWARE: race between ACQ2xx boot and HOST
# If zero hearbeat seen, add delay
#
# sleep 10;

$SLEEP=3;
$TIMEOUT=20;    ;# one global timout serves complete board set

foreach $refb ( @DEVRECS ){
	print "board  $refb->{lognum}\n";

	if ($refb->{heartbeat} == 0 ){
	    TIMEOUT: 
	    while($TIMEOUT){
		print "Waiting $TIMEOUT x $SLEEP s for board $refb->{lognum}\n";
		sleep $SLEEP;
		if (getHeartbeat($refb, $procroot, $model) != 0){
		    last TIMEOUT;
		}else{
		    $TIMEOUT -= 1;
		}
	    }
	}

    if ( $refb->{heartbeat} == 0 ){
	print "Board $refb->{lognum} has ZERO heartbeat :-(\n";
    }else{
	holdboard $refb;
	initboard $refb;
	doSiteSpecials $refb
    }
}

