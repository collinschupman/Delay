#include <gtest/gtest.h>
#include "CircularBuffer.h"

class CircularBufferTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        sampleRate = 48000.0f;
        maxDelayTime = 1.0f; // 1 second
        buffer = std::make_unique<CircularBuffer>(sampleRate, maxDelayTime);
        buffer->Init();
    }

    float sampleRate;
    float maxDelayTime;
    std::unique_ptr<CircularBuffer> buffer;
};

TEST_F(CircularBufferTest, Initialization)
{
    EXPECT_FLOAT_EQ(buffer->getReadHead(), 0.0f);
}

TEST_F(CircularBufferTest, WriteAndRead)
{
    // Write a value
    buffer->write(0.5f);
    buffer->updateWriteHead();

    // Set delay time to read immediately
    buffer->updateReadHead(1.0f); // 1 sample delay
    
    // The value should be retrievable
    unsigned readPos = static_cast<unsigned>(buffer->getReadHead());
    float value = buffer->getValue(readPos);
    EXPECT_FLOAT_EQ(value, 0.5f);
}

TEST_F(CircularBufferTest, WriteMultipleSamples)
{
    // Write several samples
    for (int i = 0; i < 10; i++)
    {
        buffer->write(static_cast<float>(i) * 0.1f);
        buffer->updateWriteHead();
    }

    // Verify we can read back with delay
    buffer->updateReadHead(5.0f); // 5 samples delay
    unsigned readPos = static_cast<unsigned>(buffer->getReadHead());
    float value = buffer->getValue(readPos);
    EXPECT_FLOAT_EQ(value, 0.5f); // Should be sample 5
}

TEST_F(CircularBufferTest, WrapAround)
{
    // Fill buffer beyond its initial position
    size_t bufferSize = sampleRate * maxDelayTime;
    
    for (size_t i = 0; i < bufferSize + 100; i++)
    {
        buffer->write(1.0f);
        buffer->updateWriteHead();
    }

    // Should wrap around without crashing
    buffer->updateReadHead(10.0f);
    EXPECT_GE(buffer->getReadHead(), 0.0f);
}

TEST_F(CircularBufferTest, DelayTimeUpdate)
{
    // Write some samples
    for (int i = 0; i < 100; i++)
    {
        buffer->write(static_cast<float>(i));
        buffer->updateWriteHead();
    }

    // Test different delay times
    buffer->updateReadHead(10.0f);
    float readHead1 = buffer->getReadHead();
    
    buffer->updateReadHead(20.0f);
    float readHead2 = buffer->getReadHead();
    
    // Longer delay should move read head further back
    EXPECT_LT(readHead2, readHead1);
}

TEST_F(CircularBufferTest, ThrowsOnOversizedBuffer)
{
    // Try to create buffer larger than MAX_BUFFER_SIZE
    float hugeSampleRate = 96000.0f;
    float hugeDelayTime = 10.0f;  // Would need 960,000 samples
    
    EXPECT_THROW({
        CircularBuffer hugeBuffer(hugeSampleRate, hugeDelayTime);
    }, std::runtime_error);
}

TEST_F(CircularBufferTest, AcceptsMaxSizeBuffer)
{
    // Create buffer at exactly max size - should succeed
    float maxSamples = static_cast<float>(CircularBuffer::MAX_BUFFER_SIZE);
    
    EXPECT_NO_THROW({
        CircularBuffer maxBuffer(maxSamples, 1.0f);
    });
}
