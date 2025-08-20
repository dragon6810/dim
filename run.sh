#!/usr/bin/env bash

if make; then
    mkdir -p run
    cd run
    ../bin/dim
    exit $?
fi

exit 1