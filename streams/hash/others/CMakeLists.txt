add_library(others STATIC EXCLUDE_FROM_ALL
    hash_functions/hash_functions.h
    hash_functions/sha1/sha1
    hash_functions/sha1/sha1_factory
    hash_functions/sha2/sha256
    hash_functions/sha2/sha256_factory
    hash_functions/sha3/sha3
    hash_functions/sha3/sha3_factory
    hash_functions/sha3/byte_order.h
    hash_functions/sha3/byte_order.c
    hash_functions/md5/md5
    hash_functions/md5/md5_factory
    hash_functions/gost/gost
    hash_functions/gost/gost_factory
    hash_functions/ripemd160/ripemd160
    hash_functions/ripemd160/ripemd160_factory
    hash_functions/tiger/tiger
    hash_functions/tiger/tiger_sbox.cpp
    hash_functions/tiger/tiger_factory
    hash_functions/whirlpool/whirlpool
    hash_functions/whirlpool/whirlpool_sbox.cpp
    hash_functions/whirlpool/whirlpool_factory
    hash_functions/whirlpool/byte_order
    )
target_link_libraries(others eacirc-core)

