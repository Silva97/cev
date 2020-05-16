#!/usr/bin/env bash

make test name=lexer || exit
make test name=parser || exit

echo "* All tests OK *"
exit 0