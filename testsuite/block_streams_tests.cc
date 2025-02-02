#include <gtest/gtest.h>
#include <testsuite/test_utils/block_test_case.h>

TEST(aes, test_vectors) {
    testsuite::block_test_case("AES", 10)();
}

TEST(aria, test_vectors) {
    testsuite::block_test_case("ARIA", 1)();
    testsuite::block_test_case("ARIA", 2)();
    testsuite::block_test_case("ARIA", 6)();
    testsuite::block_test_case("ARIA", 7)();
    testsuite::block_test_case("ARIA", 11)();
    testsuite::block_test_case("ARIA", 12)();
}

TEST(blowfish, test_vectors) {
    testsuite::block_test_case("BLOWFISH", 16)();
}

TEST(mars, test_vectors) {
    testsuite::block_test_case("MARS", 32)();
}

TEST(rc6, test_vectors) {
    testsuite::block_test_case("RC6", 20)();
}

TEST(serpent, test_vectors) {
    testsuite::block_test_case("SERPENT", 32)();
}

TEST(twofish, test_vectors) {
    testsuite::block_test_case("TWOFISH", 16)();
}

TEST(camellia, test_vectors) {
    testsuite::block_test_case("CAMELLIA", 18)();
    testsuite::block_test_case("CAMELLIA", 24)();
}

TEST(seed, test_vectors) {
    testsuite::block_test_case("SEED", 16)();
}

TEST(simon, test_vectors) {
    testsuite::block_test_case("SIMON", 32)();
    testsuite::block_test_case("SIMON", 36)();
    testsuite::block_test_case("SIMON", 42)();
    testsuite::block_test_case("SIMON", 44)();
    testsuite::block_test_case("SIMON", 52)();
    testsuite::block_test_case("SIMON", 54)();
    testsuite::block_test_case("SIMON", 68)();
    testsuite::block_test_case("SIMON", 69)();
    testsuite::block_test_case("SIMON", 72)();
}

TEST(speck, test_vectors) {
    testsuite::block_test_case("SPECK", 22)();
    testsuite::block_test_case("SPECK", 23)();
    testsuite::block_test_case("SPECK", 26)();
    testsuite::block_test_case("SPECK", 28)();
    testsuite::block_test_case("SPECK", 29)();
    testsuite::block_test_case("SPECK", 32)();
    testsuite::block_test_case("SPECK", 33)();
    testsuite::block_test_case("SPECK", 34)();
}

TEST(single_des, test_vectors) {
    testsuite::block_test_case("SINGLE-DES", 16)();
}

TEST(triple_des, test_vectors) {
    testsuite::block_test_case("TRIPLE-DES", 16)();
}

TEST(tea, test_vectors) {
    testsuite::block_test_case("TEA", 32)();
}

TEST(gost_block, test_vectors) {
    testsuite::block_test_case("GOST", 32)();
}

TEST(idea, test_vectors) {
    testsuite::block_test_case("IDEA", 8)();
}

TEST(cast, test_vectors) {
    testsuite::block_test_case("CAST", 16)();
}

TEST(kasumi, test_vectors) {
    testsuite::block_test_case("KASUMI", 8)();
}

TEST(kuznyechik, test_vectors) {
    testsuite::block_test_case("KUZNYECHIK", 10)();
}

TEST(misty1, test_vectors) {
    testsuite::block_test_case("MISTY1", 4)();
}

TEST(noekeon, test_vectors) {
    testsuite::block_test_case("NOEKEON", 16)();
}

TEST(shacal2, test_vectors) {
    testsuite::block_test_case("SHACAL2", 64)();
}

TEST(xtea, test_vectors) {
    testsuite::block_test_case("XTEA", 32)();
}
