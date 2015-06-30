#!/bin/sh
valgrind --memcheck:leak-check=full \
         --show-reachable=yes \
         --show-leak-kinds=all \
         --suppressions=valgrind/iris.supp \
         $1 $2 $3 $4 $5 $6 $7 $8 $9 $10
