#pragma once

class File
{
public:
    File(int id, int size);

    int getId() const;
    int getSize() const;
    double getPriority() const;
    double getWaitTime() const;
    bool isProcessed() const;

    void setProcessed(bool processed);
    void updateWaitTime(double deltaTime);
    void updatePriority(int customerCount);

private:
    int id;
    int size;
    double waitTime;
    double priority;
    bool processed;
};