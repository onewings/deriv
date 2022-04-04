Description
-----------
Using websockets API - available at https://api.deriv.com, build a small application
which calls https://api.deriv.com/api-explorer/#ticks to obtain a real-time tick stream of a given instrument, and then delays that tick stream by 1 minute.
Consider design choices that might help with efficient behaviour

How to compile
--------------
    mkdir build
    cd build
    cmake ..
    make

Usage
-----
    deriv_test <instrument>

Example
-------
    ./build/Debug/deriv_test R_10

    {"echo_req":{"ticks":"R_10"},"msg_type":"tick","subscription":{"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6"},"tick":{"ask":6860.995,"bid":6860.795,"epoch":1649058110,"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6","pip_size":3,"quote":6860.895,"symbol":"R_10"}}
    {"echo_req":{"ticks":"R_10"},"msg_type":"tick","subscription":{"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6"},"tick":{"ask":6861.003,"bid":6860.803,"epoch":1649058112,"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6","pip_size":3,"quote":6860.903,"symbol":"R_10"}}
    {"echo_req":{"ticks":"R_10"},"msg_type":"tick","subscription":{"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6"},"tick":{"ask":6861.067,"bid":6860.867,"epoch":1649058114,"id":"a7a57d33-6fbf-41fd-83ba-d64e0acd4ef6","pip_size":3,"quote":6860.967,"symbol":"R_10"}}