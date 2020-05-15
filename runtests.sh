#!/usr/bin/env bash

make test name=lexer || exit

echo "* All tests OK *"
exit 0