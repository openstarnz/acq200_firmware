#!/usr/bin/perl
# wav-init.pl - initializes wavegen nodes

use FindBin qw($Bin);
use lib $Bin;		# getBoards.pm lives beside this script; '.' is not in @INC on perl >= 5.26
use getBoards;

$REVID = 'Revision:$';

$JFN='/tmp/wavgen.job';
$WROOT='/dev/wav232';

$nboards = getBoards( '/proc/acq32', $slotbased, 0 );


sub encodeTC {
    my ( $T, $C ) = @_;
    return ($T << 5) | ($C - 1);
}

open JF, ">$JFN" or
    die "acq32-mknod:ERROR failed to create job file";

printf JF ( "rm -Rf $WROOT\n" );

foreach $refb ( @DEVRECS ){
    if ( $refb->{WAVMAJOR} ){ 
	my $B = $refb->{lognum};
	my $BROOT = "$WROOT/$B";

        for (my $T = 0; $T != 8; ++$T ){
	    my $TF = "$BROOT/data/$T";
            printf JF ( "mkdir -p $TF\n" );

            for (my $C = 1; $C <= 32; ++$C ){
		printf JF ( "mknod $TF/%02d c %d %d\n",
			                   $C,
                                           $refb->{WAVMAJOR},
                                           encodeTC($T, $C) );
            }
        }
	printf JF ( "chown -R dt100.dt100 $BROOT\n" );
    }
}

close( JF );
system( "chmod u+rx $JFN" );

