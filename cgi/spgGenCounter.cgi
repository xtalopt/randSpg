#!/usr/bin/perl
print "Content-type: text/html\n\n";
open(HITS,"spgGenCounter.txt");
$hits = <HITS>;
close(HITS);
++$hits;
open(HITS,">spgGenCounter.txt");
print HITS $hits;
close(HITS);
print "$hits";
exit;
