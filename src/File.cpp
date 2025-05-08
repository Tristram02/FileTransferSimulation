#include "File.hpp"

File::File(int id, int size)
    : id(id), size(size), waitTime(0.0), priority(0.0), processed(false)
{
}

int File::getId() const
{
    return id;
}

int File::getSize() const
{
    return size;
}

double File::getPriority() const
{
    return priority;
}

double File::getWaitTime() const
{
    return waitTime;
}

bool File::isProcessed() const
{
    return processed;
}

void File::setProcessed(bool processed)
{
    this->processed = processed;
}

void File::updateWaitTime(double deltaTime)
{
    if (!processed) {
        waitTime += deltaTime;
    }
}

void File::updatePriority(int customerCount)
{
    // P = T/c + c/s
    // T - time that file is waiting
    // c - number of customers (files that wait)
    // s - size of file
    
    if (customerCount <= 0) customerCount = 1;
    if (size <= 0) size = 1;
    
    priority = (waitTime / customerCount) + (static_cast<double>(customerCount) / size);
}