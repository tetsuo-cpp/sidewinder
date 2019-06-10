#!/bin/bash

current_dir=`basename $PWD`
if [ "$current_dir" != "sidewinder" ]; then
    echo "format.sh must be executed from the sidewinder project root"
    exit 1
fi

find . \( -name "*.cpp" -o -name "*.h" \) -a -not -path "./external/*" -a -not -path "./CMakeFiles/*" | xargs clang-format -i
