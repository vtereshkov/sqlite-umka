#!/bin/sh
gcc sqlite.c sqlite3.c -O3 -shared -fPIC -static-libgcc -o sqlite_linux.umi 
