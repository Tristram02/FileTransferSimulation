#pragma once

#include "Customer.hpp"
#include "Directory.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

class Simulation
{
public:
    Simulation(int directoryCount);
    ~Simulation();

    void initialize(int customerCount);
    void start();
    void stop();
    void pause();
    void resume();
    void reset();

    bool isRunning() const;
    bool isPaused() const;
    bool isCompleted() const;

    int getCustomersCount() const;
    int getDirectoriesCount() const;
    const Customer& getCustomer(int index) const;
    const Directory& getDirectory(int index) const;
    double getElapsedTime() const;
    int getProcessedFilesCount() const;
    double getTotalWaitTime() const;

    void setSpeed(int speed);

private:
    void simulationLoop();

    void generateCustomers(int customerCount);
    void assignFiles();
    bool allFilesProcessed() const;

    std::vector<Directory> directories;
    std::vector<Customer*> customers;

    std::thread simulationThread;
    std::mutex simulationMutex;
    std::condition_variable pauseCondition;
    std::atomic<bool> running;
    std::atomic<bool> paused;
    std::atomic<bool> stopRequested;

    double elapsedTime;
    int processedFilesCount;
    double totalWaitTime;
    double timeStep;
    double simulationSpeed;
};