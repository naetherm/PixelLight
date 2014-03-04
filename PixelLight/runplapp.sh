#!/bin/sh
# Change to application directory
CANONPATH=`readlink -f $0`
apppath=`dirname "${CANONPATH}"`
appname=`basename "${CANONPATH}"`
basepath=`readlink -f ${apppath}@CMAKETOOLS_BASEPATH_GO_UPS@`

appPath32=${apppath}/x86
appPath64=${apppath}/x64

#default to 32Bit
runtimeArch=x86
apppath=${appPath32}

MACHINE=`uname -m`
if [ "$MACHINE" = "x86_64" -a -e "${appPath64}/${appname}" ]
then
	runtimeArch=x64
	apppath=${appPath64}
fi

# Path to the PixelLight runtime
PLRuntimePath="${basepath}/Runtime/${runtimeArch}"

cd $apppath > /dev/null
# The environment variable PL_RUNTIME is read by PixelLight itself to get the path to its runtime directory
# LD_LIBRARY_PATH must be set to the PixelLight runtime path also, otherwise the Linux library loader wont find all PixelLight libraries on application start
PL_RUNTIME=${PLRuntimePath} LD_LIBRARY_PATH=${PLRuntimePath} ./${appname} $@
