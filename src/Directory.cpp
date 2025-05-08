#include "Directory.hpp"

Directory::Directory(int id)
    : id(id), processing(false), customer(nullptr), file(nullptr),
        processingTime(0.0), elapsedTime(0.0), progress(0)
{
}

bool Directory::isProcessing() const
{
    return processing;
}

bool Directory::assignFile(Customer* customer, File* file)
{
    if (processing) {
        return false;
    }
    
    this->customer = customer;
    this->file = file;
    
    if (file) {
        processingTime = calculateProcessingTime(file->getSize());
        elapsedTime = 0.0;
        progress = 0;
        processing = true;
        return true;
    }
    
    return false;
}

void Directory::update(double deltaTime)
{
    if (!processing || !file) {
        return;
    }
    
    elapsedTime += deltaTime;
    
    progress = static_cast<int>((elapsedTime / processingTime) * 100);
    if (progress > 100) progress = 100;
    
    if (elapsedTime >= processingTime) {
        if (customer && file) {
            customer->fileProcessed(file);
        }
        
        processing = false;
        customer = nullptr;
        file = nullptr;
        elapsedTime = 0.0;
        progress = 0;
    }
}

int Directory::getId() const
{
    return id;
}

int Directory::getProgress() const
{
    return progress;
}

Customer* Directory::getCurrentCustomer() const
{
    return customer;
}

File* Directory::getCurrentFile() const
{
    return file;
}

double Directory::getProcessingTime() const
{
    return processingTime;
}

double Directory::getRemainingTime() const
{
    if (!processing) {
        return 0.0;
    }
    
    return processingTime - elapsedTime;
}

void Directory::reset()
{
    processing = false;
    customer = nullptr;
    file = nullptr;
    processingTime = 0.0;
    elapsedTime = 0.0;
    progress = 0;
}

double Directory::calculateProcessingTime(int fileSize) const
{
    // Simple model: 1 KB takes 0.1 seconds to process
    // Minimum processing time is 0.5 seconds
    double time = fileSize * 0.1;
    return (time < 0.5) ? 0.5 : time;
}