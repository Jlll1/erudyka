#!/bin/sh
test() {
    echo "$1"
}

setup() {
    mkdir -p test/scripts
}

teardown() {
    rm -rf test
    setup
}

assert_equal() {
    expected="$1"
    actual="$2"

    if [ "$actual" = "$expected" ]; then
        echo "PASS"
    else
        echo "FAIL - Expected '$expected', but got '$actual'"
    fi

    echo ""
}

##### SETUP ########

setup

##### TESTS ########

echo ""

test "new <card> -> search <card> returns <card> with id 1"
    card="test card"
    expectedCard="1: test card"

    ./erudyka --directory test/ new "$card"
    result=$(./erudyka --directory test/ search "$card")

    assert_equal "$expectedCard" "$result"
    teardown

test "new <card> -> get 1 returns <card>"
    card="test card"
    expectedCard="test card"

    ./erudyka --directory test/ new "$card"
    result=$(./erudyka --directory test/ get 1)

    assert_equal "$expectedCard" "$result"
    teardown
