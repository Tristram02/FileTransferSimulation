#include "MainWindow.hpp"

#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), updateTimer(nullptr)
{
    setWindowTitle("File Transfer Simulation");
    resize(1000, 700);
    
    setupUI();
    
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
        QLabel *directoryLabel = new QLabel(QString("Directory %1").arg(i + 1));
        QProgressBar *progressBar = new QProgressBar();
        progressBar->setRange(0, 100);
        progressBar->setValue(0);
        QLabel *statusLabel = new QLabel("Idle");
        
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
    customersSpinBox->setRange(1, 20);
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
    
    QHBoxLayout *customersHLayout = new QHBoxLayout();
    
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
    
    QHBoxLayout *statusHLayout = new QHBoxLayout();
    
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
    
}

void MainWindow::pauseSimulation()
{
    
}

void MainWindow::stopSimulation()
{

}

void MainWindow::updateGUI()
{
    
}

void MainWindow::showCustomerDetails(int customerIndex)
{
    
}

void MainWindow::updateSimulationSpeed(int value)
{

}