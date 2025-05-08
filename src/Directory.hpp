#pragma once

#include "Customer.hpp"
#include "File.hpp"

class Directory
{
public:
    Directory(int id);
    
    bool isProcessing() const;
    bool assignFile(Customer* customer, File* file);
    void update(double deltaTime);
    
    int getId() const;
    int getProgress() const;
    Customer* getCurrentCustomer() const;
    File* getCurrentFile() const;
    double getProcessingTime() const;
    double getRemainingTime() const;
    
    void reset();
    
private:
    int id;                
    bool processing;       
    Customer* customer;    
    File* file;            
    double processingTime; 
    double elapsedTime;    
    int progress;
    
    double calculateProcessingTime(int fileSize) const;
};