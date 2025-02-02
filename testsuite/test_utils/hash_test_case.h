#pragma once

#include "test_case.h"
#include <streams/hash/hash_factory.h>
#include <streams/hash/hash_interface.h>
#include <streams/hash/sha3/sha3_interface.h>

#include <utility>

namespace testsuite {

class hash_test_case : test_case {
private:
    /**
     * base JSON configuration which all estream functions has in common
     * _stream_config is created based on this, but with addition of
     * function specific properties e.g. name of function, number of rounds
     */
    const static json base_config;

protected:
    /** size of plaintext **/
    size_t _length;

public:
    const size_t &length() const;

    hash_test_case(const std::string &algorithm, const std::size_t round)
        : test_case(algorithm, round, "hash")
        , _length(0) {}

    /**
     * Create new instance of stream for current function
     * First it creates JSON configuration of stream and
     * then create new stream using make_stream function
     * @return Pointer to instance of stream
     */
    std::unique_ptr<stream> prepare_stream() override;

    /**
     * Test raw function with current test vector
     * @param encryptor
     */
    void test() const;

    /**
     * test case is actually a functor which can done whole testing
     * It is necessary to have function name and round number and it
     * will step by step load all test vectors and test each with raw
     * function and stream
     */
    void operator()() override;

    /** Setter for single test vector **/
    void update_test_vector(const std::vector<value_type> &&plaintext,
                            const std::vector<value_type> &&hash) {
        _plaintext = plaintext;
        _ciphertext = hash;
    }

    /**
     * Reads test vector from input stream
     *
     * STRUCTURE OF TEST VECTORS (All present in
     * test-resources/sha3/{function-name}/test-vectors-{round-number}.txt) {bitsize-of-plaintext}
     * {plaintext in hex} {ciphertext in hex} {bitsize-of-plaintext2} {plaintext2 in hex}
     * {ciphertext2 in hex}
     *
     * @param input stream
     * @param test_case test_case instance
     * @return input stream
     */
    friend std::istream &operator>>(std::istream &input, hash_test_case &test_case);
};
} // namespace testsuite
