============================
Compilation Process:Makefile
=============================

Usage:

make
make all
make DEBUG=1
make clean



 "make" or "make all", it will create release binary(i.e Minimal logs)

alternatively, for debugginmg purpose debug build can be made by issuing following command on terminal

"make DEBUG=1" 

Both of above command generates "GetArch" application

To remove object files and aplication, issue "make clean"


============================
About Application "GetArch"
============================
Getarch application reads file content and if its a library then, retreives architecture supported by that lib.
Getarch reads all files present in "Directory name" passed by user as an argument, user can pass single filename(with absoulte/relative) also.

==================================
Supported command line Arguments:
==================================
"-a"  		print supported architecture type i.e. X86, X86_64, ARM etc.


===============
How to Use it:
===============
Pass "-a" as first argument followed by Filename or Directory name
example:

Getarch -a /usr/lib

Getarch -a /usr/lib/"any_library_name"


===============
Output:
===============
Prints Library name followed by compatible architecture &  number of library found



