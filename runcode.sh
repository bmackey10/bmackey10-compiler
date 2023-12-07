#!/bin/bash

for testfile in test/codegen/good[0-9].bminor
do
    program=${testfile:13:5}
    if bminor --codegen test/codegen/$program.bminor test/codegen/$program.s > test/codegen/$program.bminor.out
    then 
        gcc -g test/codegen/$program.s library.c -o test/codegen/$program
        ./test/codegen/$program > test/codegen/$program.out
        RETURN=$?
        echo $RETURN >> test/codegen/$program.out
        echo "$testfile successful codegen"
	else
		echo "$testfile failed on codegen"
	fi
done

for testfile in test/codegen/good1[0-9].bminor
do
    program=${testfile:13:6}
    if bminor --codegen test/codegen/$program.bminor test/codegen/$program.s > test/codegen/$program.bminor.out
    then 
        gcc -g test/codegen/$program.s library.c -o test/codegen/$program
        ./test/codegen/$program > test/codegen/$program.out
        RETURN=$?
        echo $RETURN >> test/codegen/$program.out
        echo "$testfile successful codegen"
	else
		echo "$testfile failed on codegen"
	fi
done

#!/bin/bash

for testfile in test/codegen/book_tests/good[0-9].bminor
do
    program=${testfile:24:5}
    if bminor --codegen test/codegen/book_tests/$program.bminor test/codegen/book_tests/$program.s > test/codegen/book_tests/$program.bminor.out
    then 
        gcc -g test/codegen/book_tests/$program.s library.c -o test/codegen/book_tests/$program
        ./test/codegen/book_tests/$program > test/codegen/book_tests/$program.out
        RETURN=$?
        echo $RETURN >> test/codegen/book_tests/$program.out
        echo "$testfile successful codegen"
	else
		echo "$testfile failed on codegen"
	fi
done

for testfile in test/codegen/book_tests/good1[0-9].bminor
do
    program=${testfile:24:6}
    if bminor --codegen test/codegen/book_tests/$program.bminor test/codegen/book_tests/$program.s > test/codegen/book_tests/$program.bminor.out
    then 
        gcc -g test/codegen/book_tests/$program.s library.c -o test/codegen/book_tests/$program
        ./test/codegen/book_tests/$program > test/codegen/book_tests/$program.out
        RETURN=$?
        echo $RETURN >> test/codegen/book_tests/$program.out
        echo "$testfile successful codegen"
	else
		echo "$testfile failed on codegen"
	fi
done