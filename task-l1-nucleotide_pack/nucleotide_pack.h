#ifndef TASK_L1__NUCLEOTIDE_PACK
#define TASK_L1__NUCLEOTIDE_PACK

/* nucleotide_pack_task
 * Input file .fasta looks like:
 * >string_name
 * atgcc...[actually nucleotide sequence. May be interrupted with ANY OTHER characters (except '>'),
 * which should be skipped without errors. ]
 * We CAN store THE WHOLE SEQUENCE (but only one) in memory.
 * Output file .gfasta looks like:
 * >string_name
 * [encoded nucleotide sequence, end of sequence is coded as character distinct from the alphabet.]
 * We should store 6 groups of 10 symbols on a single line.
 */

extern void nucleotide_pack(char direction, const char* inFileName, const char* outFileName);

#endif /* TASK_L1__NUCLEOTIDE_PACK */