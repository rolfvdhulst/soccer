#!/bin/bash

find -L | grep -E "\.(c|cpp|h|hpp)\>" --color=never | xargs clang-format -i -style="$(cat ~/.config/soccer_clang-format.file)"