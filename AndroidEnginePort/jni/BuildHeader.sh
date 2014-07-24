#!/bin/sh
#Simple script to generate default header for a JNI interface declared from
#a known and compiled Java class.
if ! [ $# -gt 1 ]
	then
		echo "Usage: BuildHeader <header file name> <java class name> [class path name]"
#No class path set assume ../bin/classes
elif [ $# -eq 2 ]
	then
		javah -classpath ../bin/classes -o $1 $2
elif [ $# -eq 3 ]
	then
		javah -classpath $3 -o $1 $2
fi 

