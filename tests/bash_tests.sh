#!/bin/bash


echo "----------------------------------------"
echo " Here are some test cases which should be considered!!!"
echo "----------------------------------------"
echo ""


echo "There is a difference between echo \"\$USER\" and echo \'\$USER\' "
echo ""
USER=celgert
echo "USER=celgert: echo \"\$USER\":"
echo "$USER"
echo "USER=celgert: echo \'\$USER\':"
echo '$USER'
echo ""

echo "Also check what happens with echo \"\'\$USER\'\" or echo \'\"\$USER\"\'"
USER=frafal
echo "USER=frafal: echo \"\'\$USER\'\":"
echo "'$USER'"
echo "USER=frafal: echo \'\"\$USER\"\':"
echo '"$USER"'
echo ""

echo "Some test cases for input handeling:"
echo ""

echo "try echo test>test1:"
echo test>test1; cat test1
echo ""

echo test >test1; cat test1
echo test> test1; cat test1
echo test > test1; cat test1


 #   echo "this <test >>test1
 #   echo "this <test >>test1"
#    (its not the same, you cant test this in bash though)
    
  #  echo 'this ">test | <<          " hello
 #   echo "'h"'2"'"'1"'"a'"'b"'
#    echo test | cat || grep t
echo ""
echo "--------------------------"
echo "TESTS for PIPEX - MANDATORY PART"
echo "--------------------------"

echo "everything should work, grep a test | wc:"
#./pipex test_in "grep a" "wc" test_out; cat test_out; echo "------";
grep a test | wc
echo ""

echo "input file does not exists, grep a test_n | wc:"
#./pipex test_n "grep a" "wc" test_out; cat test_out; echo "------";
grep a test_n | wc
echo ""

echo "first command does not exist, greppy a test_in | wc:"
#./pipex test_in "greppy a" "wc" test_out; cat test_out; echo "------";
greppy a test_in | wc
echo ""

echo "second command does not exist, grep a test_in | wm:"
#./pipex test_in "grep a" "wm" test_out; cat test_out; echo "------";
grep a test_in | wm
echo ""

echo " input file and second command does not exist, grep a test_n | wm:"
#./pipex test_n "grep a" "wm" test_out; cat test_out; echo "------"; 
grep a test_n | wm
echo ""

echo "--------------------------"
echo "TESTS - BONUS "
echo "--------------------------"
echo "everything should work, grep a test_in | grep t | wc:"
#./pipex test_in "grep a" "grep t" "wc" test_out; cat test_out; echo "------"; 
grep a test_in | grep t | wc
echo ""

echo "input file does not exist, grep a test_n | grep t | wc:"
#./pipex test_n "grep a" "grep t" "wc" test_out; cat test_out; echo "------";
grep a test_n | grep t | wc
echo ""






