#!/bin/bash

MAKE=""
if [ "$1" == "make" ]; then
    MAKE="$2"
fi

mkdir -p /root/deriv/build && \
    cd /root/deriv/build && \
    /usr/bin/cmake/bin/cmake .. && \
    make -j $(($(nproc)+1)) $MAKE

if [ $? -ne 0 ]; then
    exit 1
fi

mkdir -p /deriv/bin && \
    cd /root/deriv/ && \
    cp build/Release/deriv_test /deriv/bin/

echo "***********************"
echo "running deriv_test R_10"
/deriv/bin/deriv_test R_10
