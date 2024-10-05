#!/bin/perl
use strict;
use warnings;

my @names;

# generate basic prelude
print ".section .rodata\n";
print ".global azalea_symmap_start\n";
print ".global azalea_symmap_end\n";


print "azalea_symmap_start:\n";

# data in this format:
# nm -n build/kernel_partial | grep '.* [tT] '
while (<>) {
	my @parts = split ' ', $_;
	my ($address, $size, $name);
	$address = $parts[2];
	if ($#parts == 3) {
		$size = $parts[3];
	} else {
		$size = "F";
	}
	$name = $parts[0];

	push @names, $name;

	print "\t.quad 0x$address\n";
	print "\t.quad 0x$size\n";
	print "\t.quad name_$name\n";
}

print "azalea_symmap_end:\n";

foreach my $name ( @names ) {
	my $demangled = `./tools/demangle $name`;
	print "\tname_$name: .asciz \"$demangled\"\n"
}
