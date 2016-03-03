#include <malloc.h>
#include <math.h>
#include <string.h>
#include "BitCoding.h"


/*
 * Sets bit at BIT_POS of BYTE to value BIT. 
 */
/*
 * #define WRITE_BIT( BIT, BIT_POS, BYTE ) { \ BYTE ^=
 * (-BIT^(BYTE>>BIT_POS))&1; \ }
 */

/*
 * #define WRITE_BIT( BIT, BIT_POS, BYTE ) { \ if (BIT)\ {\ BYTE |=
 * 1<<BIT_POS;\ } else {\ BYTE &= 0xff-1<<BIT_POS;\ }\ }
 */

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

int
BitEncode(const char *toEncode, const char *alphabet, char **answer)
{
    int             alphabetSize = (int) strlen(alphabet),	/* Number
								 * of
								 * symbols 
								 * * in
								 * alphabet. 
								 */
	strSize = (int) strlen(toEncode),	/* Lenght of input string. 
						 */
	symbolSize = (int) log(alphabetSize) + 2,	/* Bits in one
							 * character. */
	answerSize = (int) (strSize * symbolSize);	/* Length of
							 * output string
							 * (in bits). */
    const char     *in = toEncode;	/* Input iterator. */
    char           *out;	/* Output iterator. */
    char            character;
    char            bitPosToWrite = 0,
	i,
	bit;
    *answer = malloc(answerSize * sizeof(char) / 8 + 1);	/* Allocating 
								 * memory
								 * for
								 * answer. 
								 */
    out = *answer;
    while (*in) {
	character = (char) (strchr(alphabet, *in) - alphabet);	/* Integer 
								 * that
								 * will
								 * represent 
								 * character 
								 * *in
								 * encoded. 
								 */
	for (i = 0; i < symbolSize; ++i) {
	    bit = (character >> i) & 1;

	    /*
	     * Writing bit. 
	     */
	    /*
	     * WRITE_BIT( bit, bitPosToWrite, *out );
	     */
	    if (bit) {
		SET_TRUE(bitPosToWrite, *out);
	    }
	    ++bitPosToWrite;
	    if (bitPosToWrite > 7) {
		++out;
		bitPosToWrite = 0;
	    }
	}
	++in;
    }
    return answerSize;
}

int
BitDecode(const char *toDecode, const char *alphabet, char **answer,
	  int bits)
{
    int             alphabetSize = (int) strlen(alphabet),	/* Number
								 * of
								 * symbols 
								 * * in
								 * alphabet. 
								 */
	symbolSize = (int) log(alphabetSize) + 2;	/* Bits in one
							 * character. */
    int             i = 0,
	j;
    const char     *in = toDecode;	/* Input iterator. */
    char            bit,	/* Bit that we'll read and write. */
                    symbol,	/* Encoded character. */
                   *out,	/* Output iterator. */
                    bitPosToRead = 0;
    *answer = malloc((size_t) bits / symbolSize * 8 + 1);	/* Allocating 
								 * memory
								 * for
								 * answer. 
								 */
    out = *answer;

    while (i < bits) {
	symbol = 0;

	/*
	 * Reading symbolSize bits. 
	 */
	for (j = 0; j < symbolSize; ++j) {
	    /*
	     * Reading bit to bit. 
	     */
	    READ_BIT(bit, bitPosToRead, *in);
	    ++bitPosToRead;
	    if (bitPosToRead > 7) {
		++in;
		bitPosToRead = 0;
	    }

	    /*
	     * Writing bit to symbol. 
	     */
	    /*
	     * WRITE_BIT( bit, j, symbol );
	     */
	    if (bit) {
		SET_TRUE(j, symbol);
	    }
	    ++i;
	}
	*out = alphabet[(unsigned char) symbol];
	++out;
    }
    *out = '\0';		/* Adding "end of string" symbol. */
    return (int) (out - *answer);
}
