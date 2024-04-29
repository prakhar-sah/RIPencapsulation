#ifndef AES_TEST_H_
#define AES_TEST_H_

/** need to choose which AES implementation to run **/
#define tiny_aes

/** need to define key size **/
#define aes_128 1

/** AES constants **/
#define AES_BLOCK_SIZE_BITS 128
#define AES_BLOCK_SIZE_BYTES (AES_BLOCK_SIZE_BITS/8)

 // Stop watchdog timer
#define board_init()    WDTCTL = WDTPW | WDTHOLD

#endif /* AES_TEST_H_ */
