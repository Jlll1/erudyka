#!/bin/sh
test() {
    echo "TEST: $1"
}

setup() {
    mkdir -p test/scripts
}

teardown() {
    rm -rf test
    setup
}

erudyka() {
    ./erudyka --directory test/ "$@"
}

assert_equal() {
    expected="$1"
    actual="$2"

    if [ "$actual" = "$expected" ]; then
        echo "PASS"
    else
        echo "FAIL - Expected '$expected', but got '$actual'."
    fi

    echo ""
}

##### SETUP ########

setup
echo ""

##### TESTS ########

test "new <card> -> search <card> returns <card> with id 1"
    card="test card"
    expectedCard="1: test card"

    erudyka new "$card"
    result=$(erudyka search "$card")

    assert_equal "$expectedCard" "$result"
    teardown

test "new <card> -> get 1 returns <card>"
    card="test card"
    expectedCard="test card"

    erudyka new "$card"
    result=$(erudyka get 1)

    assert_equal "$expectedCard" "$result"
    teardown

test "get prints linked cards"
    card1="test card 1"
    card2="test card 2"
    card3="test card 3"
    card4="test card 4"
    expectedCard=$(echo "test card 1\ntest card 2\ntest card 4")

    erudyka new "$card1"
    erudyka new "$card2"
    erudyka new "$card3"
    erudyka new "$card4"
    erudyka link 2 1
    erudyka link 4 1
    result=$(erudyka get 1)

    assert_equal "$expectedCard" "$result"
    teardown

test "search doesn't return unrelated results"
    card1="test card 1"
    card2="unrelated card"
    expectedCard="1: test card 1"

    erudyka new "$card1"
    erudyka new "$card2"
    result=$(erudyka search "test")

    assert_equal "$expectedCard" "$result"
    teardown

test "search displays ids"
    card1="test"
    card2="abc"
    expectedCard=$(echo "1: test\n\n3: test\n\n4: test")

    erudyka new "$card1"
    erudyka new "$card2"
    erudyka new "$card1"
    erudyka new "$card1"
    result=$(erudyka search "test")

    assert_equal "$expectedCard" "$result"
    teardown

test "search is case insensitive"
    card1="TEST"
    expectedCard="1: TEST"

    erudyka new "$card1"
    result=$(erudyka search "test")

    assert_equal "$expectedCard" "$result"
    teardown

test "save"
    echo '#!/bin/sh
echo "abc $1"' > test/scripts/t.sh
    card1="test"
    expectedCard="abc test"

    erudyka save "$card1" t
    result=$(erudyka get 1)

    assert_equal "$expectedCard" "$result"
    teardown


##### CLEANUP ########

rm -rf test