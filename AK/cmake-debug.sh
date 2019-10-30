#! /bin/bash

cmake -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Unix Makefiles" \
	-D_ECLIPSE_VERSION=4.2 ../src/
#cmake -DCMAKE_BUILD_TYPE=Debug -G"Eclipse CDT4 - Ninja" ../src/

