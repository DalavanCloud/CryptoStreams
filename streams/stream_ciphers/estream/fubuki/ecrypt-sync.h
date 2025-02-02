/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef FUBUKI_SYNC
#define FUBUKI_SYNC

#include "../../stream_interface.h"
#include "../ecrypt-portable.h"

namespace stream_ciphers {
namespace estream {

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define FUBUKI_NAME "FUBUKI" /* [edit] */
#define FUBUKI_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; FUBUKI_KEYSIZE(i) <= FUBUKI_MAXKEYSIZE; ++i)
 *   {
 *     keysize = FUBUKI_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define FUBUKI_MAXKEYSIZE 2048           /* [edit] */
#define FUBUKI_KEYSIZE(i) (128 + (i)*32) /* [edit] */

#define FUBUKI_MAXIVSIZE 2048           /* [edit] */
#define FUBUKI_IVSIZE(i) (128 + (i)*32) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * FUBUKI_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    s32 keysize, ivsize; /* size in bits */
    u8 key[FUBUKI_MAXKEYSIZE / 8];

    u32 mt[624]; /* the array for the state vector  */
    s32 i;

    u32 multi_table[32], inv_table[32];
    u32 add_table[32];
    s32 jump;
} FUBUKI_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Fubuki : public estream_interface {
    FUBUKI_ctx _ctx;

public:
    ECRYPT_Fubuki(int rounds)
        : estream_interface(rounds) {}
    /* Mandatory functions */

    /*
     * Key and message independent initialization. This function will be
     * called once when the program starts (e.g., to build expanded S-box
     * tables).
     */
    void ECRYPT_init(void) override;

    /*
     * Key setup. It is the user's responsibility to select the values of
     * keysize and ivsize from the set of supported values specified
     * above.
     */
    void ECRYPT_keysetup(const u8* key,
                         u32 keysize,          /* Key size in bits. */
                         u32 ivsize) override; /* IV size in bits. */

    /*
     * IV setup. After having called ECRYPT_keysetup(), the user is
     * allowed to call ECRYPT_ivsetup() different times in order to
     * encrypt/decrypt different messages with the same key but different
     * IV's.
     */
    void ECRYPT_ivsetup(const u8* iv) override;

    /*
     * Encryption/decryption of arbitrary length messages.
     *
     * For efficiency reasons, the API provides two types of
     * encrypt/decrypt functions. The ECRYPT_encrypt_bytes() function
     * (declared here) encrypts byte strings of arbitrary length, while
     * the ECRYPT_encrypt_blocks() function (defined later) only accepts
     * lengths which are multiples of ECRYPT_BLOCKLENGTH.
     *
     * The user is allowed to make multiple calls to
     * ECRYPT_encrypt_blocks() to incrementally encrypt a long message,
     * but he is NOT allowed to make additional encryption calls once he
     * has called ECRYPT_encrypt_bytes() (unless he starts a new message
     * of course). For example, this sequence of calls is acceptable:
     *
     * ECRYPT_keysetup();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_bytes();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_blocks();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_bytes();
     *
     * The following sequence is not:
     *
     * ECRYPT_keysetup();
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_bytes();
     * ECRYPT_encrypt_blocks();
     */

    void ECRYPT_encrypt_bytes(const u8* plaintext,
                              u8* ciphertext,
                              u32 msglen) override; /* Message length in bytes. */

    void ECRYPT_decrypt_bytes(const u8* ciphertext,
                              u8* plaintext,
                              u32 msglen) override; /* Message length in bytes. */

/* ------------------------------------------------------------------------- */

/* Optional features */

/*
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the FUBUKI_GENERATES_KEYSTREAM flag.
 */

#define FUBUKI_GENERATES_KEYSTREAM
#ifdef FUBUKI_GENERATES_KEYSTREAM

    void FUBUKI_keystream_bytes(FUBUKI_ctx* ctx,
                                u8* keystream,
                                u32 length); /* Length of keystream in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional optimizations */

/*
 * By default, the functions in this section are implemented using
 * calls to functions declared above. However, you might want to
 * implement them differently for performance reasons.
 */

/*
 * All-in-one encryption/decryption of (short) packets.
 *
 * The default definitions of these functions can be found in
 * "ecrypt-sync.c". If you want to implement them differently, please
 * undef the FUBUKI_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define FUBUKI_USES_DEFAULT_ALL_IN_ONE /* [edit] */

    void FUBUKI_encrypt_packet(
            FUBUKI_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void FUBUKI_decrypt_packet(
            FUBUKI_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * FUBUKI_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define FUBUKI_BLOCKLENGTH 16 /* [edit] */

#define FUBUKI_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef FUBUKI_USES_DEFAULT_BLOCK_MACROS

#define FUBUKI_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*FUBUKI_BLOCKLENGTH)

#define FUBUKI_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*FUBUKI_BLOCKLENGTH)

#ifdef FUBUKI_GENERATES_KEYSTREAM

#define FUBUKI_keystream_blocks(ctx, keystream, blocks)                                            \
    FUBUKI_keystream_bytes(ctx, keystream, (blocks)*FUBUKI_BLOCKLENGTH)

#endif

#else

    void FUBUKI_encrypt_blocks(FUBUKI_ctx* ctx,
                               const u8* plaintext,
                               u8* ciphertext,
                               u32 blocks); /* Message length in blocks. */

    void FUBUKI_decrypt_blocks(FUBUKI_ctx* ctx,
                               const u8* ciphertext,
                               u8* plaintext,
                               u32 blocks); /* Message length in blocks. */

#ifdef FUBUKI_GENERATES_KEYSTREAM

    void FUBUKI_keystream_blocks(FUBUKI_ctx* ctx,
                                 u8* keystream,
                                 u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the FUBUKI_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DFUBUKI_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (FUBUKI_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same FUBUKI_BLOCKLENGTH, etc.).
 */
#define FUBUKI_MAXVARIANT 1 /* [edit] */

#ifndef FUBUKI_VARIANT
#define FUBUKI_VARIANT 1
#endif

#if (FUBUKI_VARIANT > FUBUKI_MAXVARIANT)
#error this variant does not exist
#endif

} // namespace estream
} // namespace stream_ciphers

/* ------------------------------------------------------------------------- */

#endif
