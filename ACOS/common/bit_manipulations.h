#ifndef COMMON__BIT_MANIPULATIONS_H
#define COMMON__BIT_MANIPULATIONS_H

/*
 * Returns bit at BIT_POS position of BYTE+BYTE_POS byte.
 */
#define GET_BIT( BIT_POS, BYTE_POS, BYTE ) \
	(unsigned char)((GET_BYTE(BYTE, BYTE_POS) >> BIT_POS) & 1)

/*
 * Returns byte BYTE+BYTE_POS
 */
#define GET_BYTE( X, BYTE_POS) \
	( ((unsigned char*)&X)[BYTE_POS] )

/*
 * Sets bit at BIT_POS of BYTE to value 1.
 */
#define SET_TRUE( BIT_POS, BYTE ) { \
    BYTE |= 1<<BIT_POS; \
}

/*
 * Reads bit at BIT_POS of BYTE to BIT variable.
 */
#define READ_BIT( BIT, BIT_POS, BYTE ) { \
    BIT = (BYTE>>BIT_POS) & 1; \
}

#define BIT(BIT_POS, BYTE ) \
    ((BYTE>>BIT_POS) & 1)

/*
 * Sets bit at BIT_POS of BYTE to value BIT.
 */
#define WRITE_BIT( BIT, BIT_POS, BYTE ) { \
	BYTE ^= (-BIT^BYTE)&(1<<BIT_POS); \
}

#endif /* COMMON__BIT_MANIPULATIONS_H */