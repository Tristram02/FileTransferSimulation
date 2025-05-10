#include "Simulation.hpp"
#include <chrono>
#include <mutex>
#include <random>
#include <thread>

Simulation::Simulation(int directoryCount) :
    running(false), paused(false), stopRequested(false),
    elapsedTime(0.0), processedFilesCount(0), totalWaitTime(0.0),
    timeStep(0.1), simulationSpeed(1.0)
{
    for (int i = 0; i < directoryCount; i++)
    {
        directories.push_back(i + 1);
    }
}

Simulation::~Simulation()
{
    stop();
    for (auto& customer : customers)
    {
        delete customer;
    }

    customers.clear();
}

void Simulation::initialize(int customerCount)
{
    for (auto& customer: customers)
    {
        delete customer;
    }
    customers.clear();

    for (auto& directory: directories)
    {
        directory.reset();
    }

    elapsedTime = 0.0;
    processedFilesCount = 0;
    totalWaitTime = 0;

    generateCustomers(customerCount);
}

void Simulation::start()
{
    if (running)
    {
        return;
    }

    running = true;
    paused = false;
    stopRequested = false;

    simulationThread = std::thread(&Simulation::simulationLoop, this);
}

void Simulation::pause()
{
    paused = true;
}

void Simulation::resume()
{
    if (running && paused)
    {
        paused = false;
        pauseCondition.notify_one();
    }
}

void Simulation::stop()
{
    if (running)
    {
        stopRequested = true;
        if (paused)
        {
            resume();
        }

        if (simulationThread.joinable())
        {
            simulationThread.join();
        }

        running = false;
        paused = false;
    }
}

void Simulation::reset()
{
    stop();

    for (auto& customer : customers)
    {
        delete customer;
    }
    customers.clear();
    
    for (auto& directory : directories)
    {
        directory.reset();
    }
    
    if (simulationThread.joinable())
    {
        simulationThread.join();
    }

    elapsedTime = 0.0;
    processedFilesCount = 0;
    totalWaitTime = 0.0;
}

bool Simulation::isRunning() const
{
    return running;
}

bool Simulation::isPaused() const
{
    return paused;
}

bool Simulation::isCompleted() const
{
    return running && allFilesProcessed();
}

int Simulation::getCustomersCount() const
{
    return static_cast<int>(customers.size());
}

int Simulation::getDirectoriesCount() const
{
    return static_cast<int>(directories.size());
}

const Customer& Simulation::getCustomer(int index) const
{
    return *(customers[index]);
}

const Directory& Simulation::getDirectory(int index) const
{
    return directories[index];
}

double Simulation::getElapsedTime() const
{
    return elapsedTime;
}

int Simulation::getProcessedFilesCount() const
{
    return processedFilesCount;
}

double Simulation::getTotalWaitTime() const
{
    return totalWaitTime;
}

void Simulation::setSpeed(int speed)
{
    // Convert from 1-10 to 0.2-2.0
    simulationSpeed = 0.2 + (speed - 1) * 0.2;
}

void Simulation::simulationLoop()
{
    while (running && !stopRequested)
    {
        if (paused)
        {
            std::unique_lock<std::mutex> lock(simulationMutex);
            pauseCondition.wait(lock, [this]()
                {
                    return !paused || stopRequested;
                });
            
            if (stopRequested) break;
        }

        double deltaTime = timeStep * simulationSpeed;
        elapsedTime += deltaTime;

        for (auto& customer: customers)
        {
            customer->updateWaitTimes(deltaTime);
        }

        int activeCustomerCount = 0;
        for (auto& customer: customers)
        {
            if (!customer->isCompleted()) {
                activeCustomerCount++;
            }
        }

        for (auto& customer: customers)
        {
            customer->updatePriorities(activeCustomerCount);
        }

        for (auto& directory: directories)
        {
            directory.update(deltaTime);
        }

        assignFiles();

        processedFilesCount = 0;
        totalWaitTime = 0;
        for (auto& customer: customers)
        {
            processedFilesCount += customer->getProcessedFilesCount();
            totalWaitTime += customer->getTotalWaitTime();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(timeStep * 1000)));

        if (allFilesProcessed())
        {
            break;
        }
    }

    running = false;
}

void Simulation::generateCustomers(int customerCount)
{
    auto rd = std::random_device{};
    auto gen = std::mt19937{rd()};
    auto fileCountDist = std::uniform_int_distribution<>{3, 10};
    auto fileSizeDist = std::uniform_int_distribution<>{1, 100};

    for (int i = 0; i < customerCount; i++)
    {
        auto customer = new Customer{i + 1};
        int fileCount = fileCountDist(gen);
        for (int j = 0; j < fileCount; j++)
        {
            int fileSize = fileSizeDist(gen);
            customer->addFile(fileSize);
        }
        customers.push_back(customer);
    }

    for (auto& customer: customers)
    {
        customer->updatePriorities(static_cast<int>(customers.size()));
    }
}

void Simulation::assignFiles()
{
    for (auto& directory: directories)
    {
        if (!directory.isProcessing())
        {
            Customer* bestCustomer = nullptr;
            File* bestFile = nullptr;
            
            double highestPriority = -1.0;

            for (auto& customer: customers)
            {
                if (customer->getPendingFilesCount() > 0)
                {
                    auto nextFile = customer->getNextFile();
                    if (nextFile && nextFile->getPriority() > highestPriority)
                    {
                        if (bestFile)
                        {
                            bestCustomer->addFile(bestFile);
                        }
                        bestCustomer = customer;
                        bestFile = nextFile;
                        highestPriority = nextFile->getPriority();
                    } else if (nextFile)
                    {
                        customer->addFile(nextFile);
                    }
                }
            }

            if (bestCustomer && bestFile)
            {
                directory.assignFile(bestCustomer, bestFile);
            }
        }
    }
}

bool Simulation::allFilesProcessed() const
{
    for (auto& customer : customers) {
        if (!customer->isCompleted()) {
            return false;
        }
    }
    return true;
}