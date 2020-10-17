#ifndef _CHECK_H_
#define _CHECK_H_

#define SPADES 0
#define CLUBS 0x10
#define HEARTS 0x20
#define DIAMONDS 0x30

#define HIGH_CARD 0
#define ONE_PAIR 1
#define TWO_PAIR 2
#define THREE_OF_A_KIND 3
#define STRAIGHT 4
#define FLUSH 5
#define FULL_HOUSE 6
#define FOUR_OF_A_KIND 7
#define STRAIGHT_FLUSH 8

char core_check_parse(const char *hand, unsigned int *out);
char core_check_print_hand(const char *hand);
char core_check_print_hand_ranking(unsigned int hand);

#endif
