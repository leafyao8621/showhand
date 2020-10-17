#include <stdio.h>
#include "core/check.h"

int main(void) {
    const char hand1[] = {
        HEARTS | 13,
        HEARTS | 12,
        HEARTS | 11,
        HEARTS | 10,
        HEARTS | 9
    };
    const char hand2[] = {
        SPADES | 12,
        SPADES | 11,
        SPADES | 10,
        SPADES | 9,
        SPADES | 8
    };
    unsigned int res1 = 0;
    unsigned int res2 = 0;
    core_check_print_hand(hand1);
    core_check_parse(hand1, &res1);
    core_check_print_hand_ranking(res1);
    core_check_print_hand(hand2);
    core_check_parse(hand2, &res2);
    core_check_print_hand_ranking(res2);
    puts(res1 > res2 ? "1 > 2" : (res1 == res2 ? "1 = 2" : "1 < 2"));
}
