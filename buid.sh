#!/bin/sh

#定义SDK源码头文件目录
SDK_INC_DIR=/home/aron566/Workspace/C_SDK/device-sdk-c/include

#定义SDK动态库文件目录
SDK_LIB_DIR=/home/aron566/Workspace/C_SDK/device-sdk-c/build/release/_CPack_Packages/Linux/TGZ/csdk-1.3.0/lib

#定义编译生成的APP名称
TARGET_APP_NAME=custom_devices_service

#定义源码文件名称
SOURCE_FILE_NAME=devices_service/custom-device.c

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SDK_LIB_DIR

case "$1" in
	make)
		$CC -I$SDK_INC_DIR -L$SDK_LIB_DIR -o $TARGET_APP_NAME $SOURCE_FILE_NAME -lcsdk
		;;
	run)
		./$TARGET_APP_NAME -c res
		;;
	*)
		echo "Usage: $0 {make|run}"
		exit 1
esac