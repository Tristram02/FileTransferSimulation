#include "MainWindow.hpp"
#include "Simulation.hpp"

#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), updateTimer(nullptr)
{
    setWindowTitle("File Transfer Simulation");
    resize(1000, 700);
    
    setupUI();

    simulation = new Simulation(5);
    
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &MainWindow::updateGUI);
}

MainWindow::~MainWindow()
{
    if (updateTimer) {
        updateTimer->stop();
        delete updateTimer;
    }
    
}

void MainWindow::setupUI()
{
    centralWidget = new QWidget();
    setCentralWidget(centralWidget);
    
    mainLayout = new QVBoxLayout(centralWidget);
    
    createDirectoriesUI();
    
    createControlsUI();
    
    createCustomersUI();
    
    createStatusUI();
}

void MainWindow::createDirectoriesUI()
{
    directoriesGroupBox = new QGroupBox("CPU Directories (5)");
    directoriesLayout = new QGridLayout(directoriesGroupBox);
    
    for (int i = 0; i < 5; i++) {
        auto directoryLabel = new QLabel(QString("Directory %1").arg(i + 1));
        auto progressBar = new QProgressBar();
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        auto statusLabel = new QLabel("Idle");
        
        directoryLabels.push_back(directoryLabel);
        directoryProgressBars.push_back(progressBar);
        directoryStatusLabels.push_back(statusLabel);
        
        directoriesLayout->addWidget(directoryLabel, i, 0);
        directoriesLayout->addWidget(progressBar, i, 1);
        directoriesLayout->addWidget(statusLabel, i, 2);
    }
    
    mainLayout->addWidget(directoriesGroupBox);
}

void MainWindow::createControlsUI()
{
    controlsGroupBox = new QGroupBox("Simulation Controls");
    controlsLayout = new QHBoxLayout(controlsGroupBox);
    
    startButton = new QPushButton("Start");
    pauseButton = new QPushButton("Pause");
    stopButton = new QPushButton("Stop");
    
    pauseButton->setEnabled(false);
    stopButton->setEnabled(false);
    
    speedLabel = new QLabel("Speed:");
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setRange(1, 10);
    speedSlider->setValue(5);
    
    customersLabel = new QLabel("Customers:");
    customersSpinBox = new QSpinBox();
    customersSpinBox->setRange(1, 1000);
    customersSpinBox->setValue(10);
    
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSimulation);
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::pauseSimulation);
    connect(stopButton, &QPushButton::clicked, this, &MainWindow::stopSimulation);
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::updateSimulationSpeed);
    
    controlsLayout->addWidget(startButton);
    controlsLayout->addWidget(pauseButton);
    controlsLayout->addWidget(stopButton);
    controlsLayout->addWidget(speedLabel);
    controlsLayout->addWidget(speedSlider);
    controlsLayout->addWidget(customersLabel);
    controlsLayout->addWidget(customersSpinBox);
    
    mainLayout->addWidget(controlsGroupBox);
}

void MainWindow::createCustomersUI()
{
    customersGroupBox = new QGroupBox("Customers");
    customersLayout = new QVBoxLayout(customersGroupBox);
    
    auto customersHLayout = new QHBoxLayout();
    
    customersList = new QListWidget();
    customersHLayout->addWidget(customersList, 1);
    
    customerDetailsText = new QTextEdit();
    customerDetailsText->setReadOnly(true);
    customersHLayout->addWidget(customerDetailsText, 2);
    
    customersLayout->addLayout(customersHLayout);
    
    connect(customersList, &QListWidget::currentRowChanged, this, &MainWindow::showCustomerDetails);
    
    mainLayout->addWidget(customersGroupBox, 1);
}

void MainWindow::createStatusUI()
{
    statusGroupBox = new QGroupBox("Simulation Status");
    statusLayout = new QVBoxLayout(statusGroupBox);
    
    auto statusHLayout = new QHBoxLayout();
    
    timeElapsedLabel = new QLabel("Time Elapsed: 0 secs");
    filesProcessedLabel = new QLabel("Files Processed: 0");
    averageWaitTimeLabel = new QLabel("Average Wait Time: 0 secs");
    simulationStatusLabel = new QLabel("Status: Ready");
    
    statusHLayout->addWidget(timeElapsedLabel);
    statusHLayout->addWidget(filesProcessedLabel);
    statusHLayout->addWidget(averageWaitTimeLabel);
    statusHLayout->addWidget(simulationStatusLabel);
    
    statusLayout->addLayout(statusHLayout);
    
    mainLayout->addWidget(statusGroupBox);
}

void MainWindow::startSimulation()
{
    if (!simulation) return;

    if (simulation->isRunning())
    {
        simulation->resume();
    } else {
        simulation->reset();
        simulation->initialize(customersSpinBox->value());
        simulation->start();
        customersList->clear();

        for (int i = 0; i < simulation->getCustomersCount(); i++)
        {
            customersList->addItem(QString("Customer %1").arg(i + 1));
        }
    }

    startButton->setEnabled(false);
    pauseButton->setEnabled(true);
    stopButton->setEnabled(true);
    customersSpinBox->setEnabled(false);
    simulationStatusLabel->setText("Status: Running");
    updateTimer->start(100);
}

void MainWindow::pauseSimulation()
{
    if (!simulation) return;

    simulation->pause();

    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
    stopButton->setEnabled(true);
    simulationStatusLabel->setText("Status: Paused");
    updateTimer->stop();
}

void MainWindow::stopSimulation()
{
    if (!simulation) return;

    simulation->stop();
    startButton->setEnabled(true);
    pauseButton->setEnabled(false);
    stopButton->setEnabled(false);
    customersSpinBox->setEnabled(true);
    simulationStatusLabel->setText("Status: Stopped");

    updateTimer->stop();
    for (auto progressBar : directoryProgressBars) {
        progressBar->setValue(0);
    }
    for (auto statusLabel : directoryStatusLabels) {
        statusLabel->setText("Idle");
    }
}

void MainWindow::updateGUI()
{
    if (!simulation || !simulation->isRunning()) return;

    for (int i = 0; i < 5; i++) {
        auto dir = simulation->getDirectory(i);
        directoryProgressBars[i]->setValue(dir.getProgress());
        
        if (dir.isProcessing()) {
            auto customer = dir.getCurrentCustomer();
            auto file = dir.getCurrentFile();
            if (customer && file) {
                directoryStatusLabels[i]->setText(QString("Customer %1, File %2, %3KB")
                    .arg(customer->getId())
                    .arg(file->getId())
                    .arg(file->getSize()));
            }
        } else {
            directoryStatusLabels[i]->setText("Idle");
        }
    }

    timeElapsedLabel->setText(QString("Time Elapsed: %1 secs").arg(simulation->getElapsedTime()));
    filesProcessedLabel->setText(QString("Files Processed: %1").arg(simulation->getProcessedFilesCount()));
    averageWaitTimeLabel->setText(QString("Average Wait Time: %1 secs").arg(
        simulation->getProcessedFilesCount() > 0 ? 
        QString::number(simulation->getTotalWaitTime() / simulation->getProcessedFilesCount(), 'f', 2) : 
        "0.00"));

    int currentRow = customersList->currentRow();
    if (currentRow >= 0 && currentRow < simulation->getCustomersCount()) {
        showCustomerDetails(currentRow);
    }

    if (simulation->isCompleted()) {
        QMessageBox::information(this, "Simulation Complete", 
            "All files have been processed!\n\n"
            "Total files processed: " + QString::number(simulation->getProcessedFilesCount()) + "\n"
            "Average wait time: " + QString::number(simulation->getTotalWaitTime() / simulation->getProcessedFilesCount(), 'f', 2) + " secs");
        stopSimulation();
    }
}

void MainWindow::showCustomerDetails(int customerIndex)
{
    if (!simulation || customerIndex < 0 || customerIndex >= simulation->getCustomersCount()) {
        customerDetailsText->clear();
        return;
    }
    
    const auto& customer = simulation->getCustomer(customerIndex);
    
    std::stringstream ss;
    ss << "Customer " << customer.getId() << " Details:\n";
    ss << "------------------------\n";
    ss << "Pending Files: " << customer.getPendingFilesCount() << "\n";
    ss << "Processed Files: " << customer.getProcessedFilesCount() << "\n";
    ss << "Total Files: " << customer.getTotalFilesCount() << "\n";
    ss << "Average Priority: " << customer.getAveragePriority() << "\n";
    ss << "Wait Time: " << customer.getTotalWaitTime() << " secs\n\n";
    
    ss << "Pending Files:\n";
    for (int i = 0; i < customer.getPendingFilesCount(); i++) {
        const auto& file = customer.getPendingFile(i);
        ss << "- File " << file.getId() << ": " << file.getSize() << "KB, Priority: " 
            << file.getPriority() << ", Wait Time: " << file.getWaitTime() << " secs\n";
    }
    
    customerDetailsText->setText(QString::fromStdString(ss.str()));
}

void MainWindow::updateSimulationSpeed(int value)
{
    if (!simulation) return;
    simulation->setSpeed(value);
}