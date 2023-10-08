#pragma once

struct CircularBuffer
{
    int length = 0;
    float readHead = 0.f;
    int writeHead = 0;
    float *buffer = nullptr;
};