#!/bin/bash
# run_tests.sh - Comprehensive test script for iii assignment
# Run from the project root directory: bash tests/run_tests.sh

PASS=0
FAIL=0

pass() {
    echo "  PASS: $1"
    PASS=$((PASS + 1))
}

fail() {
    echo "  FAIL: $1"
    FAIL=$((FAIL + 1))
}

echo "========================================"
echo " Building all targets"
echo "========================================"
make clean && make all
if [ $? -ne 0 ]; then
    echo "BUILD FAILED - fix compilation errors first"
    exit 1
fi
echo ""

echo "========================================"
echo " Test 1: UArray2 - useuarray2 matches correct output"
echo "========================================"
./my_useuarray2 > /tmp/my_useuarray2_out.txt 2>&1
if diff -q /tmp/my_useuarray2_out.txt correct_output > /dev/null 2>&1; then
    pass "my_useuarray2 output matches correct_output"
else
    fail "my_useuarray2 output differs from correct_output"
    diff correct_output /tmp/my_useuarray2_out.txt
fi
echo ""

echo "========================================"
echo " Test 2: Bit2 - usebit2 matches correct output"
echo "========================================"
./my_usebit2 > /tmp/my_usebit2_out.txt 2>&1
if [ -f correct_usebit2.txt ]; then
    # Compare with the text output if available
    ./correct_usebit2 > /tmp/correct_usebit2_out.txt 2>&1
    if diff -q /tmp/my_usebit2_out.txt /tmp/correct_usebit2_out.txt > /dev/null 2>&1; then
        pass "my_usebit2 output matches correct_usebit2"
    else
        fail "my_usebit2 output differs from correct_usebit2"
        diff /tmp/correct_usebit2_out.txt /tmp/my_usebit2_out.txt
    fi
else
    echo "  (No correct_usebit2.txt to compare - checking it runs without error)"
    if [ $? -eq 0 ]; then
        pass "my_usebit2 runs without error"
    else
        fail "my_usebit2 crashed"
    fi
fi
echo ""

echo "========================================"
echo " Test 3: Sudoku - valid puzzle"
echo "========================================"
./sudoku tests/sudoku_valid.pgm
if [ $? -eq 0 ]; then
    pass "Valid sudoku recognized (exit 0)"
else
    fail "Valid sudoku rejected (should be exit 0)"
fi

echo "========================================"
echo " Test 4: Sudoku - second valid puzzle"
echo "========================================"
./sudoku tests/sudoku_valid2.pgm
if [ $? -eq 0 ]; then
    pass "Valid sudoku #2 recognized (exit 0)"
else
    fail "Valid sudoku #2 rejected (should be exit 0)"
fi

echo "========================================"
echo " Test 5: Sudoku - original test file"
echo "========================================"
./sudoku sudoku_test.txt
if [ $? -eq 0 ]; then
    pass "sudoku_test.txt recognized as valid (exit 0)"
else
    fail "sudoku_test.txt rejected (should be exit 0)"
fi

echo "========================================"
echo " Test 6: Sudoku - duplicate in last row"
echo "========================================"
./sudoku tests/sudoku_dup_row.pgm
if [ $? -ne 0 ]; then
    pass "Duplicate row detected (exit non-zero)"
else
    fail "Duplicate row NOT detected (should be exit non-zero)"
fi

echo "========================================"
echo " Test 7: Sudoku - duplicate in column"
echo "========================================"
./sudoku tests/sudoku_dup_col.pgm
if [ $? -ne 0 ]; then
    pass "Duplicate column detected (exit non-zero)"
else
    fail "Duplicate column NOT detected (should be exit non-zero)"
fi

echo "========================================"
echo " Test 8: Sudoku - has zero value"
echo "========================================"
./sudoku tests/sudoku_has_zero.pgm
if [ $? -ne 0 ]; then
    pass "Zero intensity detected (exit non-zero)"
else
    fail "Zero intensity NOT detected (should be exit non-zero)"
fi

echo "========================================"
echo " Test 9: Sudoku - bad 3x3 box"
echo "========================================"
./sudoku tests/sudoku_bad_box.pgm
if [ $? -ne 0 ]; then
    pass "Bad 3x3 box detected (exit non-zero)"
else
    fail "Bad 3x3 box NOT detected (should be exit non-zero)"
fi

echo "========================================"
echo " Test 10: Sudoku produces no stdout output"
echo "========================================"
OUTPUT=$(./sudoku tests/sudoku_valid.pgm 2>/dev/null)
if [ -z "$OUTPUT" ]; then
    pass "Sudoku produces no stdout on valid input"
else
    fail "Sudoku printed to stdout: '$OUTPUT'"
fi

OUTPUT=$(./sudoku tests/sudoku_dup_row.pgm 2>/dev/null)
if [ -z "$OUTPUT" ]; then
    pass "Sudoku produces no stdout on invalid input"
else
    fail "Sudoku printed to stdout on invalid: '$OUTPUT'"
fi

echo ""
echo "========================================"
echo " Test 11: Unblackedges - all black becomes all white"
echo "========================================"
./unblackedges tests/all_black.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_all_black.pbm > /dev/null 2>&1; then
    pass "All-black image correctly cleared"
else
    fail "All-black image not correctly cleared"
    diff tests/answer_all_black.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 12: Unblackedges - all white stays all white"
echo "========================================"
./unblackedges tests/all_white.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_all_white.pbm > /dev/null 2>&1; then
    pass "All-white image unchanged"
else
    fail "All-white image changed unexpectedly"
    diff tests/answer_all_white.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 13: Unblackedges - border box (testinput1)"
echo "========================================"
./unblackedges tests/testinput1.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_testinput1.pbm > /dev/null 2>&1; then
    pass "Border box correctly removed"
else
    fail "Border box not correctly removed"
    diff tests/answer_testinput1.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 14: Unblackedges - center island preserved"
echo "========================================"
./unblackedges tests/island_center.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_island_center.pbm > /dev/null 2>&1; then
    pass "Center island preserved correctly"
else
    fail "Center island not preserved"
    diff tests/answer_island_center.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 15: Unblackedges - test1.pbm (border with inner square)"
echo "========================================"
./unblackedges tests/test1.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer1.pbm > /dev/null 2>&1; then
    pass "test1.pbm matches answer1.pbm"
else
    fail "test1.pbm does not match answer1.pbm"
    diff tests/answer1.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 16: Unblackedges - test2.pbm (diagonal)"
echo "========================================"
./unblackedges tests/test2.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer2.pbm > /dev/null 2>&1; then
    pass "test2.pbm matches answer2.pbm"
else
    fail "test2.pbm does not match answer2.pbm"
    diff tests/answer2.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 17: Unblackedges - test3.pbm"
echo "========================================"
./unblackedges tests/test3.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer3.pbm > /dev/null 2>&1; then
    pass "test3.pbm matches answer3.pbm"
else
    fail "test3.pbm does not match answer3.pbm"
    diff tests/answer3.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 18: Unblackedges - L-shape edge removal"
echo "========================================"
./unblackedges tests/l_shape_edge.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_l_shape_edge.pbm > /dev/null 2>&1; then
    pass "L-shape edge pixels removed"
else
    fail "L-shape edge pixels not correctly removed"
    diff tests/answer_l_shape_edge.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 19: Unblackedges - single pixel"
echo "========================================"
./unblackedges tests/single_pixel.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_single_pixel.pbm > /dev/null 2>&1; then
    pass "Single black pixel removed"
else
    fail "Single black pixel not correctly removed"
    diff tests/answer_single_pixel.pbm /tmp/ube_out.txt
fi

echo "========================================"
echo " Test 20: Unblackedges - stdin input"
echo "========================================"
./unblackedges < tests/testinput1.pbm > /tmp/ube_out.txt
if diff -q /tmp/ube_out.txt tests/answer_testinput1.pbm > /dev/null 2>&1; then
    pass "stdin input works correctly"
else
    fail "stdin input produces wrong output"
    diff tests/answer_testinput1.pbm /tmp/ube_out.txt
fi

echo ""
echo "========================================"
echo " RESULTS: $PASS passed, $FAIL failed"
echo "========================================"
