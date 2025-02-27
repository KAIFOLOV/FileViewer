#include "mainwindow.h"
#include "folder_scan_strategy.h"
#include "file_type_scan_strategy.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), strategy(nullptr) {
    auto widget = new QWidget();
    widget->setMinimumSize(1100, 600);
    auto layout = new QVBoxLayout(widget);
    auto mainContentLayout = new QHBoxLayout();

    strategyBox = new QComboBox();
    strategyBox->addItems({"Группировка по папкам", "Группировка по типам"});
    connect(strategyBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateStrategy);

    viewBox = new QComboBox();
    viewBox->addItems({"Таблица", "Круговая диаграмма", "Столбчатая диаграмма"});
    connect(viewBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateView);

    treeView = new QTreeView();
    treeView->setMinimumSize(600, 500);
    treeView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Создаем модель один раз
    fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");
    treeView->setModel(fileSystemModel);

    // Задаем ширину колонок сразу после установки модели
    treeView->setColumnWidth(0, 300);
    treeView->setColumnWidth(1, 100);
    treeView->setColumnWidth(2, 100);
    treeView->setColumnWidth(3, 100);

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onDirectorySelected);

    tableView = new QTableView;
    model = new FileSizeModel(this);
    tableView->setModel(model);

    chartView = new QChartView();
    chartView->setVisible(false);

    selectFolderButton = new QPushButton("Выбрать папку", this);
    connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::onSelectFolder);

    mainContentLayout->addWidget(treeView);
    mainContentLayout->addWidget(tableView);
    mainContentLayout->addWidget(chartView);

    layout->addWidget(strategyBox);
    layout->addWidget(viewBox);
    layout->addWidget(selectFolderButton);
    layout->addLayout(mainContentLayout);
    setCentralWidget(widget);

    updateStrategy(0);
}

MainWindow::~MainWindow() {}

void MainWindow::updateStrategy(int index) {
    delete strategy;
    index == 0 ? strategy = new FolderScanStrategy() : strategy = new FileTypeScanStrategy();
    model->setData(strategy->scan(_currentFolderPath));
    updateView(_currentView);
}

void MainWindow::onDirectorySelected(const QItemSelection &selected) {
    if (selected.isEmpty()) return;
    QString path = selected.indexes().first().data(QFileSystemModel::FilePathRole).toString();
    _currentFolderPath = path;
    model->setData(strategy->scan(path));
    updateView(_currentView);
}

void MainWindow::updateView(int index) {
    _currentView = index;

    tableView->setVisible(index == 0);
    chartView->setVisible(index != 0);
    if (index == 1) showPieChart();
    if (index == 2) showBarChart();
}

void MainWindow::showPieChart() {
    auto *series = new QPieSeries();
    for (const auto &item : model->dataStorage())
        series->append(item.name, item.percentage);

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Круговая диаграмма");

    chartView->setChart(chart);
}

void MainWindow::showBarChart() {
    auto *series = new QBarSeries();
    for (const auto &item : model->dataStorage()) {
        auto *set = new QBarSet(item.name);
        *set << item.percentage;
        series->append(set);
    }

    auto *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Столбчатая диаграмма");

    chartView->setChart(chart);
}

void MainWindow::onSelectFolder() {
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выбрать папку");
    _currentFolderPath = folderPath;

    if (!folderPath.isEmpty()) {
        fileSystemModel->setRootPath(folderPath);
        treeView->setRootIndex(fileSystemModel->index(folderPath));

        model->setData(strategy->scan(folderPath));
    }
}
