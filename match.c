#include <stdbool.h>
#include <ctype.h>
#include "match.h"

static bool match_asterisks(char c, const char *pattern, const char *text);
static bool match_set(const char *pattern, const char *text, int *chars_used);
static bool char_equal_ci(char p, char t){  //for case insensitive
    return tolower((unsigned char)p) == tolower((unsigned char)t);
}
static bool parse_braces(const char *brace_pattern, int *min_count, int *max_count, int *chars_used); //for braces



bool match(const char *pattern, const char *text){
    // anchor ^pattern
    if(pattern[0] == '^'){
        return match_here(pattern + 1, text);
    }
    // no ^ -> try matching starting at every position in text
    do{
        if(match_here(pattern, text)){
            return true;
        }
    } while(*text++ != '\0');
    return false;
}
// for $ [abc] c* c? . norm
bool match_here(const char *pattern, const char *text){
    // if end of pattern -> match
    if(pattern[0] == '\0'){
        return true;
    }

    // end anchor -> $
    if(pattern[0] == '$' && pattern[1] == '\0'){
        return *text == '\0';
    }

    // [abc] match any of listed
    if(pattern[0] == '['){
        int chars_used = 0;
        if(match_set(pattern, text, &chars_used)){
            // match one character from the set and move past it
            return match_here(pattern + chars_used, text + 1);
        }
        return false;
    }

    // c{n} or c{n, m} -> repeat previous character between n and m times
    if(pattern[1] == '{') {
        int min_count = 0;
        int max_count = 0;
        int brace_chars = 0;

        // pattern + 1 points at {
        if(!parse_braces(pattern + 1, &min_count, &max_count, &brace_chars)) {
            return false;
        }
        const char *text_ptr = text;
        int count = 0;
        // first -> match the min occurrences
        while(count < min_count) {
            if(!*text_ptr || !(pattern[0] == '.' || char_equal_ci(pattern[0], *text_ptr))) {
                return false;
            }
            text_ptr++;
            count++;
        }
        const char *next_pattern = pattern + 1 + brace_chars;
        // try all counts from min to max
        if(match_here(next_pattern, text_ptr)) {
            return true;
        }
        while(count < max_count && *text_ptr && (pattern[0] == '.' || char_equal_ci(pattern[0], *text_ptr))) {
            text_ptr++;
            count++;
            if(match_here(next_pattern, text_ptr)) {
                return true;
            }
        }
        return false;
    }

    // c* match 0 or more of the previous character
    if(pattern[1] == '*'){
        return match_asterisks(pattern[0], pattern + 2, text);
    }

    // c+ match 1 or more of previous character
    if(pattern[1] == '+') {
    // need at least 1 match
    if(*text && (pattern[0] == '.' || char_equal_ci(pattern[0], *text))) {
        // after one match -> c* on the rest
        return match_asterisks(pattern[0], pattern + 2, text + 1);
    }
    return false;
}

    // c? 0 or 1 of the previous character
    if(pattern[1] == '?'){
        if(*text && (pattern[0] == '.' || char_equal_ci(pattern[0], *text))){
            if(match_here(pattern + 2, text + 1)){
                return true;
            }
        }
        return match_here(pattern + 2, text);
    }
    // normal char or . must match current text char
    if(*text && (pattern[0] == '.' || char_equal_ci(pattern[0], *text))){
        return match_here(pattern + 1, text + 1);
    }
    return false;
}

// for c* - tries all cases for it
static bool match_asterisks(char c, const char *pattern, const char *text){
    const char *scan_text = text;
    do{
        if(match_here(pattern, scan_text)){
            return true;
        }
        // if next char not match c or . -> stop
    } while(*scan_text && (c == '.' || char_equal_ci(c, *scan_text)) && scan_text++);

    return false;
}

// for [abc] - returns true if text[0] is inside
static bool match_set(const char *pattern, const char *text, int *chars_used){
    if(*text == '\0'){
        return false;
    }
    int i = 1; // skips [
    bool matched = false;

    // char until ]
    while(pattern[i] != '\0' && pattern[i] != ']'){
        if(char_equal_ci(pattern[i], text[0])){
            matched = true;
        }
        i++;
    }

    // if never saw ] -> no match
    if(pattern[i] != ']'){
        return false;
    }

    // num of char in [abc]
    *chars_used = i + 1;

    return matched;
}

// parses {n} or {n,m}
static bool parse_braces(const char *brace_pattern, int *min_count, int *max_count, int *chars_used) {
    int i = 1; // skips {
    int first = 0;
    bool have_first_digit = false;

    // parse first number
    while(brace_pattern[i] >= '0' && brace_pattern[i] <= '9') {
        have_first_digit = true;
        first = first * 10 + (brace_pattern[i] - '0');
        i++;
    }

    if(!have_first_digit) {
        return false;
    }

    if(brace_pattern[i] == '}') {
        // {n}
        *min_count = first;
        *max_count = first;
        *chars_used = i + 1; // includes }
        return true;
    } else if(brace_pattern[i] == ',') {
        // {n,m}
        i++;
        int second = 0;
        bool have_second_digit = false;

        while(brace_pattern[i] >= '0' && brace_pattern[i] <= '9') {
            have_second_digit = true;
            second = second * 10 + (brace_pattern[i] - '0');
            i++;
        }

        if(!have_second_digit || brace_pattern[i] != '}') {
            return false;
        }

        if(second < first) {
            return false;
        }

        *min_count = first;
        *max_count = second;
        *chars_used = i + 1; // includes }
        return true;
    }

    return false;
}