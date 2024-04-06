#include "Hamming.h"

uint8_t kSizeOfBoolVector = 8;

std::vector<bool> EncodeHamming(const std::vector<bool>& message) {
    int message_size = message.size();
    int count_of_add_bits = 0;
    while ((1 << count_of_add_bits) < message_size + count_of_add_bits + 1) {
        ++count_of_add_bits;
    }
    std::vector<bool> encoded(message_size + count_of_add_bits);
    int j = 0;
    for (int i = 0; i < message_size + count_of_add_bits; ++i) {
        if (((i + 1) & i) == 0) {
            continue;
        }
        encoded[i] = message[j++];
    }
    for (int i = 0; i < count_of_add_bits; ++i) {
        int parity = 0;
        int bit_idx = (1 << i) - 1;
        while (bit_idx < message_size + count_of_add_bits) {
            for (int j = 0; j < (1 << i); ++j) {
                if (bit_idx < message_size + count_of_add_bits) {
                    parity ^= encoded[bit_idx];
                }
                ++bit_idx;
            }
            bit_idx += (1 << i);
        }
        encoded[(1 << i) - 1] = parity;
    }

    return encoded;
}

std::vector<bool> DecodeHamming(std::vector<bool>& encoded) {
    int count_of_add_bits = 0;
    while ((1 << count_of_add_bits) < encoded.size()) {
        ++count_of_add_bits;
    }
    std::vector<bool> decoded(encoded.size() - count_of_add_bits);
    int error_bit = -1;
    for (int i = 0; i < count_of_add_bits; ++i) {
        int parity = 0;
        int bit_idx = (1 << i) - 1;
        while (bit_idx < encoded.size()) {
            for (int j = 0; j < (1 << i); ++j) {
                if (bit_idx < encoded.size()) {
                    parity ^= encoded[bit_idx];
                }
                ++bit_idx;
            }
            bit_idx += (1 << i);
        }
        if (parity != 0) {
            error_bit += (1 << i);
        }
    }
    if (error_bit >= 0 && error_bit < encoded.size()) {
        encoded[error_bit] = !encoded[error_bit];
    }
    int j = 0;
    for (int i = 0; i < encoded.size(); ++i) {
        if (((i + 1) & i) == 0) {
            continue;
        }
        decoded[j++] = encoded[i];
    }

    return decoded;
}

char ConvertBoolVectorToChar(const std::vector<bool>& boolVector) {
    char result = 0;
    for (int i = 0; i < kSizeOfBoolVector; i++) {
        if (boolVector[i]) {
            result |= (1 << i);
        }
    }

    return result;
}