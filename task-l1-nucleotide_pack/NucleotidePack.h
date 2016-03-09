#ifndef TASK_L1__NUCLEOTIDE_PACK
#define TASK_L1__NUCLEOTIDE_PACK

/* nucleotide_pack_task
 * Input file .fasta looks like:
 * >string_name
 * atgcc...[actually nucleotide sequence. May be interrupted with termination characters, which should be skipped.]
 * Output file .gfasta looks like:
 * >string_name
 * [encoded nucleotide sequence, end of sequence is coded as character distinct from the alphabet.]
 */

extern void nucleotide_pack(const char* inFileName, const char* outFileName);

extern void nucleotide_unpack( const char* inFileName, const char* outFileName );

#endif /* TASK_L1__NUCLEOTIDE_PACK */