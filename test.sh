#!/bin/sh
test() {
    echo "$1"
}

assert_equal() {
    expected="$1"
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        echo "* PASS\t$expected => $actual"
    else
        echo "# FAIL\tExpected $expected, but got $actual"
    fi
}

##### SETUP ########

mkdir -p test/scripts

##### TESTS ########




##### CLEANUP ######

rm -rf test