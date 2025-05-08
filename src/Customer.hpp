#pragma once

#include "File.hpp"
#include <vector>

class Customer
{
public:
    Customer(int id);
    ~Customer();

    void addFile(int size);
    File* getNextFile();
    void fileProcessed(File* file);

    int getId() const;
    int getPendingFilesCount() const;
    int getProcessedFilesCount() const;
    int getTotalFilesCount() const;
    double getTotalWaitTime() const;
    double getAveragePriority() const;

    const File& getPendingFile(int index) const;

    void updateWaitTimes(double deltaTime);
    void updatePriorities(int customerCount);

    bool isCompleted() const;

private:
    int id;
    std::vector<File*> pendingFiles;
    std::vector<File*> processedFiles;
    double totalWaitTime;
};