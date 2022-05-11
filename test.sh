#!/bin/sh
test() {
    echo "$1"
}

assert_equal() {
    expected="$1"
    actual="$2"

    if [ "$actual" = "$expected" ]; then
        echo "* PASS\t$expected => $actual"
    else
        echo "# FAIL\tExpected $expected, but got $actual"
    fi
}

##### SETUP ########

mkdir -p test/scripts

##### TESTS ########

test "new <card> -> search <card> returns <card> with id 1"
    card="test card"
    expectedCard="1: test card"

    ./erudyka --directory test/ new "$card"
    result=$(./erudyka --directory test/ search "$card")

    assert_equal "$expectedCard" "$result"


##### CLEANUP ######

rm -rf test