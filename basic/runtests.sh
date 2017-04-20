:
#!/bin/sh
#
#	@(#)runtests	1.3 00/07/10 Connectathon Testsuite
#

InitFile="../tests.init"

if test $# -ge 1
then
	TESTARG=$1
else
	. $InitFile
fi

echo "Starting BASIC tests: test directory $NFSTESTDIR (arg: $TESTARG)"
mkdir $NFSTESTDIR
if test ! -d $NFSTESTDIR
then
	echo "Can't make directory $NFSTESTDIR"
	exit 1
fi

set -e

# File and Directory tree creation test
echo ""
echo "test file and directory"
./test1 $TESTARG

# File and Directory tree removal test
echo ""
./test2 $TESTARG

# Lookup across mount point
echo ""
./test3 $TESTARG

# Setattr, getattr and lookup tests
echo ""
if [ "$CIFS" != "yes" ]; then
	./test4 $TESTARG
# Getattr and lookup tests
else
	./test4a $TESTARG
fi

# Write and read tests
echo ""
./test5 $TESTARG
# Test 5a and 5b separate out the read and write tests into simpler components.
# echo ""
 ./test5a $TESTARG
# echo ""
 ./test5b $TESTARG

# Read Directory test
echo ""
./test6 $TESTARG

# Rename and Link test
echo ""
if [ "$HARDLINKS"o != no ]
then
	./test7 $TESTARG
else
	./test7a $TESTARG
fi
# Link Test
# echo ""
./test7b $TESTARG

# Symlink and readlink test
echo ""
./test8 $TESTARG

# Getfs test
echo ""
./test9 $TESTARG

echo ""

echo "Congratulations, you passed the basic tests!"

exit 0
