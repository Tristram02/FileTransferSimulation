#include "Customer.hpp"
#include <algorithm>
#include <iostream>

Customer::Customer(int id)
    : id(id), totalWaitTime(0.0), fileId(0)
{
}

Customer::~Customer()
{
    for (auto& file : pendingFiles) {
        delete file;
    }
    pendingFiles.clear();
    
    for (auto& file : processedFiles) {
        delete file;
    }
    processedFiles.clear();
}

void Customer::addFile(int size)
{
    pendingFiles.push_back(new File(++fileId, size));
    std::sort(pendingFiles.begin(), pendingFiles.end(), [](const File* a, const File* b) {
        return a->getSize() < b->getSize();
    });
}

void Customer::addFile(File* file)
{
    pendingFiles.push_back(file);
    std::sort(pendingFiles.begin(), pendingFiles.end(), [](const File* a, const File* b) {
        return a->getSize() < b->getSize();
    });
}

File* Customer::getNextFile()
{
    if (pendingFiles.empty()) {
        return nullptr;
    }
    
    auto nextFile = pendingFiles.front();
    pendingFiles.erase(pendingFiles.begin());
    return nextFile;
}

void Customer::fileProcessed(File* file)
{
    if (file) {
        file->setProcessed(true);
        totalWaitTime += file->getWaitTime();
        processedFiles.push_back(file);
    }
}

int Customer::getId() const
{
    return id;
}

int Customer::getPendingFilesCount() const
{
    return static_cast<int>(pendingFiles.size());
}

int Customer::getProcessedFilesCount() const
{
    return static_cast<int>(processedFiles.size());
}

int Customer::getTotalFilesCount() const
{
    return getPendingFilesCount() + getProcessedFilesCount();
}

double Customer::getTotalWaitTime() const
{
    return totalWaitTime;
}

double Customer::getAveragePriority() const
{
    if (pendingFiles.empty()) {
        return 0.0;
    }
    
    double totalPriority = 0.0;
    for (auto& file : pendingFiles) {
        totalPriority += file->getPriority();
    }
    
    return totalPriority / pendingFiles.size();
}

const File& Customer::getPendingFile(int index) const
{
    return *(pendingFiles[index]);
}

void Customer::updateWaitTimes(double deltaTime)
{
    for (auto& file : pendingFiles) {
        file->updateWaitTime(deltaTime);
    }
}

void Customer::updatePriorities(int customerCount)
{
    for (auto& file : pendingFiles) {
        file->updatePriority(customerCount);
    }
}

bool Customer::isCompleted() const
{
    return pendingFiles.empty() && processedFiles.size() == fileId;
}