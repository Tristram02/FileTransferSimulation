#pragma once

#include "Simulation.hpp"
#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QListWidget>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QSpinBox>
#include <QTimer>
#include <QTextEdit>
#include <vector>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void startSimulation();
    void pauseSimulation();
    void stopSimulation();
    void updateGUI();
    void showCustomerDetails(int customerIndex);
    void updateSimulationSpeed(int value);

    private:
    void setupUI();
    void createDirectoriesUI();
    void createControlsUI();
    void createCustomersUI();
    void createStatusUI();
    
    Simulation* simulation;
    QTimer *updateTimer;
    
    QWidget *centralWidget;
    QVBoxLayout *mainLayout;
    
    QGroupBox *directoriesGroupBox;
    QGridLayout *directoriesLayout;
    std::vector<QLabel*> directoryLabels;
    std::vector<QProgressBar*> directoryProgressBars;
    std::vector<QLabel*> directoryStatusLabels;
    
    QGroupBox *controlsGroupBox;
    QHBoxLayout *controlsLayout;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QSlider *speedSlider;
    QLabel *speedLabel;
    QSpinBox *customersSpinBox;
    QLabel *customersLabel;
    
    QGroupBox *customersGroupBox;
    QVBoxLayout *customersLayout;
    QListWidget *customersList;
    QTextEdit *customerDetailsText;
    
    QGroupBox *statusGroupBox;
    QVBoxLayout *statusLayout;
    QLabel *timeElapsedLabel;
    QLabel *filesProcessedLabel;
    QLabel *simulationStatusLabel;
};