#!/bin/sh

. test/env.sh

TEST_TIMEOUT=4000000
TEST_BIN=test/test_os/asp/ttsp/obj/check_library/exception/asp

util_set_cmdname $0
util_do_test 
util_do_log_check
