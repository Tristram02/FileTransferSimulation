#include "Customer.hpp"
#include <algorithm>

Customer::Customer(int id)
    : id(id), totalWaitTime(0.0)
{
}

Customer::~Customer()
{
    for (File* file : pendingFiles) {
        delete file;
    }
    pendingFiles.clear();
    
    for (File* file : processedFiles) {
        delete file;
    }
    processedFiles.clear();
}

void Customer::addFile(int size)
{
    int fileId = pendingFiles.size() + processedFiles.size() + 1;
    pendingFiles.push_back(new File(fileId, size));
}

File* Customer::getNextFile()
{
    if (pendingFiles.empty()) {
        return nullptr;
    }
    
    auto highestPriorityIter = std::max_element(
        pendingFiles.begin(), 
        pendingFiles.end(),
        [](const File* a, const File* b) {
            return a->getPriority() < b->getPriority();
        }
    );
    
    if (highestPriorityIter != pendingFiles.end()) {
        File* nextFile = *highestPriorityIter;
        pendingFiles.erase(highestPriorityIter);
        return nextFile;
    }
    
    return nullptr;
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
    for (const File* file : pendingFiles) {
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
    for (File* file : pendingFiles) {
        file->updateWaitTime(deltaTime);
    }
}

void Customer::updatePriorities(int customerCount)
{
    for (File* file : pendingFiles) {
        file->updatePriority(customerCount);
    }
}

bool Customer::isCompleted() const
{
    return pendingFiles.empty();
}