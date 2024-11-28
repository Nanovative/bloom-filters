#pragma once

#include <vector>
#include <cstdint>
#include <string>
#include <iostream>
#include <stdexcept>

#include "hasher/HashGen32.hpp"
#include "Utils.hpp"

class CountingBloomFilter
{
public:
    // Constructor that automatically calculates optimal size and number of hash functions
    CountingBloomFilter(int n, double p, const std::string &algorithm = "sha256", const std::string &scheme = Hash32::SCHEME_ENHANCED_DOUBLE_HASHING)
        : algorithm_(algorithm), scheme_(scheme)
    {
        // Calculate the optimal number of bits (m) and hash functions (k) based on n (expected elements) and p (false positive rate)
        size_ = BloomFilterUtils::CalculateOptimalM(n, p);
        k_ = BloomFilterUtils::CalculateOptimalK(size_, n);
        maxRange_ = 1000; // Default, can be adjusted or calculated if necessary

        filter_.resize(size_, 0); // Initialize filter with 0
    }

    // Constructor that allows creating with arbitrary size and k (number of hash functions)
    CountingBloomFilter(int k, int size, const std::string &algorithm = "sha256", const std::string &scheme = Hash32::SCHEME_ENHANCED_DOUBLE_HASHING)
        : k_(k), size_(size), algorithm_(algorithm), scheme_(scheme)
    {
        if (size <= 0)
        {
            throw std::invalid_argument("Bloom filter size must be greater than 0.");
        }
        filter_.resize(size_, 0); // Initialize filter with 0
    }

    // Existing method to add data to the filter
    void Add(const std::vector<uint8_t> &data)
    {
        auto indices = hashGen_.Execute(data, algorithm_, scheme_);
        for (auto index : *indices)
        {
            // Increment the count for the bucket at 'index'
            filter_[index % size_] += 1;
        }
    }

    // Check if data exists in the filter
    bool Contains(const std::vector<uint8_t> &data)
    {
        auto indices = hashGen_.Execute(data, algorithm_, scheme_);
        for (auto index : *indices)
        {
            // Check if any of the corresponding buckets have a count of 0
            if (filter_[index % size_] == 0)
            {
                return false;
            }
        }
        return true;
    }

    // Remove data from the filter
    void Remove(const std::vector<uint8_t> &data)
    {
        auto indices = hashGen_.Execute(data, algorithm_, scheme_);
        for (auto index : *indices)
        {
            // Decrement the count for the bucket at 'index'
            if (filter_[index % size_] > 0)
            {
                filter_[index % size_] -= 1;
            }
        }
    }

    // Print the internal state of the Bloom filter
    void Print() const
    {
        for (size_t i = 0; i < filter_.size(); ++i)
        {
            std::cout << "Bucket " << i << ": " << filter_[i] << std::endl;
        }
    }

    int K() { return k_; }
    int MaxRange() { return maxRange_; }
    int Size() { return size_; };

private:
    int k_;                       // Number of hash functions
    int maxRange_;                // Maximum range for the hashes
    int size_;                    // Number of buckets in the Bloom filter
    std::string algorithm_;       // Hash algorithm to use
    std::string scheme_;          // Hashing scheme to use (default: EDH)
    Hash32::HashGen hashGen_;     // Hash generator for creating hash indices
    std::vector<uint8_t> filter_; // Bloom filter (counter array)
};