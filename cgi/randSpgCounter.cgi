#!/usr/bin/perl
print "Content-type: text/html\n\n";
open(HITS,"randSpgCounter.txt");
$hits = <HITS>;
close(HITS);
++$hits;
open(HITS,">randSpgCounter.txt");
print HITS $hits;
close(HITS);
print "$hits";
exit;
