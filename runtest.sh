#!/bin/sh

for testfile in test/print/good*.bminor
do
	if ./bminor --print $testfile > $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

