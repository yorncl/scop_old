#!/bin/bash

cmake --version > /dev/null && make --version > /dev/null
if [ $? != 0 ]
then 
	echo "You at least need cmake and make to build this library" 1>&2
	exit 1
fi

echo "About to download, install and build glfw3 locally"
if [[ $OSTYPE == 'darwin'* ]]
then
	curl -L https://github.com/glfw/glfw/releases/download/3.3.5/glfw-3.3.5.bin.MACOS.zip -o glfw.zip
	unzip glfw.zip
	mv glfw-3.3.5.bin.MACOS glfw
	mv glfw/include/* include/
	mkdir lib/libglfw3
	mv glfw/lib-x86_64/* lib/libglfw3
else
	curl -L https://github.com/glfw/glfw/releases/download/3.3.5/glfw-3.3.5.zip -o glfw.zip && \
	unzip glfw.zip &&\
	mv glfw-3.3.5 glfw &&\
	# Building and locally installing the lib
	pushd . &&\
	cd glfw &&\
	cmake -S . -B build && cd build && make && \
	popd
	mkdir -p lib/libglfw3 &&\
	mv glfw/include/* include/ &&\
	mv glfw/build/src/libglfw3.a lib/libglfw3/
fi


