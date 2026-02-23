#include <stdio.h>
#include <stdbool.h>
#include "match.h"

// helper that runs one test and prints pass or fail
void run_test(const char *pattern, const char *text, bool expected) {
    bool result = match(pattern, text);

    printf("pattern=\"%s\"  text=\"%s\"  ->  %s  (%s)\n",
           pattern,
           text,
           result ? "true" : "false",
           (result == expected) ? "PASS" : "FAIL");
}

int main(void) {
    // basic exact match
    run_test("hello", "hello", true);
    run_test("hello", "world", false);

    // case insensitive
    run_test("hello", "HELLO", true);
    run_test("HeLLo", "hello", true);

    // dot wildcard
    run_test("h.llo", "hello", true);
    run_test("h.llo", "hallo", true);
    run_test("h.llo", "hllo", false);

    // [abc]
    run_test("[abc]at", "bat", true);
    run_test("[abc]at", "Cat", true); // case insensitive 
    run_test("[abc]at", "hat", false);

    // c*
    run_test("ab*c", "ac", true); // zero b
    run_test("ab*c", "abc", true); // one b
    run_test("ab*c", "abbbc", true); // many b
    run_test("ab*c", "accc", true);

    // plus +
    run_test("ab+c", "abc", true); // one b
    run_test("ab+c", "abbbc", true); // many b
    run_test("ab+c", "ac", false); // zero b should fail

    // question mark ?
    run_test("colou?r", "color", true); // zero u
    run_test("colou?r", "colour", true); // one u
    run_test("colou?r", "colouur", false);

    // {n}
    run_test("a{3}", "aaa", true);
    run_test("a{3}", "aa", false);
    run_test("a{3}", "aaaa", true);

    // {n,m}
    run_test("a{2,4}", "a", false);
    run_test("a{2,4}", "aa", true);
    run_test("a{2,4}", "aaa", true);
    run_test("a{2,4}", "aaaa", true);
    run_test("a{2,4}", "aaaaa", true);

    // ^ and $
    run_test("^hello", "hello world", true);
    run_test("^world", "hello world", false);
    run_test("world$", "hello world", true);
    run_test("world$", "world hello", false);

    return 0;
}
