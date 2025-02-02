#!/bin/sh
gcc sqlite.c sqlite3.c -O3 -shared -pthread -fPIC -static-libgcc -o sqlite_linux.umi 
