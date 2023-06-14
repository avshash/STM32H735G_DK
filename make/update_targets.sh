#!/bin/bash
set -e
set -o pipefail

source targets.list

PROGRAM_NAME=$0
ROOT2CUR=$1
BUILD_PATH=$2
CUR_PWD=$PWD

for target in $TARGETS_LIST;
do
    echo -e "\t${ROOT2CUR}$target \\" >> $BUILD_PATH/Makefile
done

for subdir in $SUBDIRS_LIST;
do
    cd $subdir
    $PROGRAM_NAME ${ROOT2CUR}${subdir}/ $BUILD_PATH
    echo "  ${subdir} \\" >> $BUILD_PATH/subdirs.sh
    cd $CUR_PWD
done


