#!/usr/bin/env bash

make test name=lexer || exit
make test name=parser || exit
make test name=eval || exit

echo "* All tests OK *"
exit 0