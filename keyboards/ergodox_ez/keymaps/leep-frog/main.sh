#!/bin/bash

export QMK_PATH="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
# One time setup:
# . a q $QMK_PATH

alias qc="qmk compile"
