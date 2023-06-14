#!/bin/bash
set -e
set -o pipefail

ANSI_COLOR_RED='\033[0;31m'
ANSI_COLOR_GREEN='\033[0;32m'
# No color.
ANSI_COLOR_NONE='\033[0m'

ORIG_PWD=$PWD
MAKE_PATH=$ORIG_PWD/$(dirname $0)
SRC_ROOT=$MAKE_PATH/src
if [ -z $1 ] || [ $3 ]
  then
    echo -e -n "$ANSI_COLOR_RED"
    echo "Usage is 'build_makefile.sh <targets_file> [target dir]'"
    echo -e -n "$ANSI_COLOR_NONE"
    exit
fi

if [ -e $1 ]
  then
    echo -e -n "$ANSI_COLOR_GREEN"
    echo "Input params good."
    echo -e -n "$ANSI_COLOR_NONE"
  else
    echo -e -n "$ANSI_COLOR_RED"
    echo "Can not find target_file '$1'"
    echo -e -n "$ANSI_COLOR_NONE"
    exit
fi

BIN_ROOT=$ORIG_PWD/bin
if [ $2 ]
  then
    BIN_ROOT=$ORIG_PWD/$2
fi

mkdir -p $BIN_ROOT
cd $BIN_ROOT
BIN2SRC=$(realpath --relative-to=$BIN_ROOT $SRC_ROOT/../../src)
SRC2BIN=$(realpath --relative-to=$SRC_ROOT/../../src $BIN_ROOT)

# Clean up previous build (if exists).
if [ -e ./remove_all.sh ]
  then
    ./remove_all.sh
fi

mkdir -p .tmp

TMP2MAKE_SRC=$(realpath --relative-to=$BIN_ROOT/.tmp $MAKE_PATH/src)o

cp $MAKE_PATH/subdirs.sh.prefix $BIN_ROOT/.tmp/subdirs.sh
echo "TARGETS = \\" > $BIN_ROOT/.tmp/Makefile
cd $MAKE_PATH/src
$MAKE_PATH/update_targets.sh "" $BIN_ROOT/.tmp Makefile
echo "" >> $BIN_ROOT/.tmp/Makefile
cat $MAKE_PATH/subdirs.sh.postfix >> $BIN_ROOT/.tmp/subdirs.sh
chmod +x $BIN_ROOT/.tmp/subdirs.sh

cd $BIN_ROOT/.tmp
echo "make_build: \${TARGETS}" >> Makefile
echo -e "\tg++ $^ -o make_build\n" >> Makefile
echo "%.o:" >> Makefile
echo -e "\tg++ -c -Wall -Werror -fmax-errors=3 -I ${SRC_ROOT}/redirect ${SRC_ROOT}/\$*.cc -o \$@\n" >> Makefile

echo "clean:" >> Makefile
echo -e "\trm -f \${TARGETS}\n" >> Makefile

make -s clean
./subdirs.sh create
make -s make_build

cd $BIN_ROOT
echo "Running '.tmp/make_build $BIN2SRC/ $SRC2BIN/ ../$1"
.tmp/make_build $BIN2SRC/ $SRC2BIN/ ../$1
rm .tmp -rf

chmod +x clean.sh
chmod +x remove_all.sh
./clean.sh
