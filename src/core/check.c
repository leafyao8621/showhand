#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "check.h"

static const char *suits[] = {
    "Spades",
    "Clubs",
    "Hearts",
    "Diamonds"
};

static const char *ranks[] = {
    "ERROR",
    "Ace",
    "Two",
    "Three",
    "Four",
    "Five",
    "Six",
    "Seven",
    "Eight",
    "Nine",
    "Ten",
    "Jack",
    "Queen",
    "King"
};

static const char *hand_rankings[] = {
    "High Card",
    "One Pair",
    "Two Pair",
    "Three of a Kind",
    "Straight",
    "Flush",
    "Full House",
    "Four of a Kind",
    "Straight Flush"
};

char core_check_print_hand(const char *hand) {
    if (!hand) {
        return 1;
    }
    const char *iter = hand;
    for (char i = 0; i < 5; ++i, ++iter) {
        printf("%s of %s\n", ranks[*iter & 0xf], suits[*iter >> 4]);
    }
    return 0;
}

char core_check_print_hand_ranking(unsigned int hand) {
    printf("%s", hand_rankings[hand >> 20]);
    int msk = 0xf0000;
    switch (hand >> 20) {
    case HIGH_CARD:
    case ONE_PAIR:
    case THREE_OF_A_KIND:
    case FOUR_OF_A_KIND:
    case STRAIGHT:
    case STRAIGHT_FLUSH:
        printf(" %s\n", ranks[(hand & 0xf0000) >> 16]);
        break;
    case TWO_PAIR:
    case FULL_HOUSE:
        printf(" %s %s\n",
               ranks[(hand & 0xf0000) >> 16],
               ranks[(hand & 0xf000) >> 12]);
        break;
    case FLUSH:
        for (char i = 0, dist = 16;
             i < 5;
             ++i, msk >>= 4, dist -= 4) {
            printf(" %s", ranks[(hand & msk) >> dist]);
        }
        putchar(10);
        break;
    default:
        return 1;
    }
    return 0;
}

static int comp_rev(const void *a, const void *b) {
    unsigned char va = *(const unsigned char*)a;
    va = (va << 4) | (va >> 4);
    unsigned char vb = *(const unsigned char*)b;
    vb = (vb << 4) | (vb >> 4);
    return vb - va;
}

static int comp(const void *a, const void *b) {
    return *(const char*)b - *(const char*)a;
}

char core_check_parse(const char *hand, unsigned int *out) {
    if (!hand || !out) {
        return 1;
    }
    char buf[6];
    char cnt[5] = {0};
    char suit[4] = {0};
    char ncnt = 0;
    char streak = 1;
    char *iter_cnt = cnt;
    char *iter_buf = buf + 1;
    buf[5] = 0xff;
    memcpy(buf, hand, 5);
    qsort(buf, 5, 1, comp_rev);
    ++suit[*buf >> 4];
    for (char i = 1; i < 6; ++i, ++iter_buf) {
        if ((*iter_buf & 0xf) != (iter_buf[-1] & 0xf)) {
            ++ncnt;
            *(iter_cnt++) = (streak << 4) | (iter_buf[-1] & 0xf);
            streak = 1;
        } else {
            ++streak;
        }
        ++suit[*iter_buf >> 4];
    }
    qsort(cnt, 5, 1, comp);
    qsort(suit, 4, 1, comp);
    iter_cnt = cnt;
    char cnt_1 = *iter_cnt >> 4;
    char rnk_1 = *iter_cnt & 0xf;
    char cnt_2 = iter_cnt[1] >> 4;
    char rnk_2 = iter_cnt[1] & 0xf;
    char max = *buf & 0xf;
    unsigned int rank_based = HIGH_CARD;
    if (cnt_1 == 1) {
        rank_based = (HIGH_CARD << 20) | (max << 16);
    } else {
        switch (cnt_1) {
        case 2:
            rank_based = (ONE_PAIR << 20) | (rnk_1 << 16);
            if (cnt_2 == 2) {
                rank_based = (TWO_PAIR << 20) | (rnk_1 << 16) | (rnk_2 << 12);
            }
            break;
        case 3:
            rank_based = (THREE_OF_A_KIND << 20) | (rnk_1 << 16);
            if (cnt_2 == 2) {
                rank_based = (FULL_HOUSE << 20) | (rnk_1 << 16) | (rnk_2 << 12);
            }
            break;
        case 4:
            rank_based = (FOUR_OF_A_KIND << 20) | (rnk_1 << 16);
            break;
        }
    }
    unsigned int suit_based = ((*suit == 5 ? FLUSH : HIGH_CARD) << 20);
    char dist = 16;
    iter_buf = buf;
    for (char i = 0; i < 5; ++i, dist -= 4, ++iter_buf) {
        suit_based |= (*iter_buf & 0xf) << dist;
    }
    unsigned int seq_based = HIGH_CARD;
    if (!(rank_based >> 20)) {
        char sum = 0;
        iter_buf = buf;
        for (char i = 0; i < 4; ++i, ++iter_buf) {
            sum += (*iter_buf & 0xf) - (iter_buf[1]) & 0xf;
        }
        seq_based = ((sum == 4 ? STRAIGHT : HIGH_CARD) << 20) | (max << 16);
    }
    if ((seq_based >> 8) && (suit_based >> 8)) {
        *out = (STRAIGHT_FLUSH << 20) | (max << 16);
    } else if (seq_based > suit_based) {
        *out = seq_based > rank_based ? seq_based : rank_based;
    } else {
        *out = suit_based > rank_based ? suit_based : rank_based;
    }
    return 0;
}
