#include "mainwindow.h"
#include "folder_scan_strategy.h"
#include "file_type_scan_strategy.h"

#include <QVBoxLayout>
#include <QFileSystemModel>
#include <QFileDialog>
#include <QPieSeries>
#include <QBarSeries>
#include <QBarSet>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), strategy(nullptr) {
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout(widget);

    strategyBox = new QComboBox;
    strategyBox->addItems({"Группировка по папкам", "Группировка по типам"});
    connect(strategyBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateStrategy);

    viewBox = new QComboBox;
    viewBox->addItems({"Таблица", "Круговая диаграмма", "Столбчатая диаграмма"});
    connect(viewBox, &QComboBox::currentIndexChanged, this, &MainWindow::updateView);

    treeView = new QTreeView;
    QFileSystemModel *fileSystemModel = new QFileSystemModel(this);
    fileSystemModel->setRootPath("");  // Устанавливаем корень модели в пустую строку для возможности отображать весь файл
    treeView->setModel(fileSystemModel);

    connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MainWindow::onDirectorySelected);

    tableView = new QTableView;
    model = new FileSizeModel(this);
    tableView->setModel(model);

    chartView = new QChartView;
    chartView->setVisible(false);

    // Кнопка для выбора папки
    selectFolderButton = new QPushButton("Выбрать папку", this);
    connect(selectFolderButton, &QPushButton::clicked, this, &MainWindow::onSelectFolder);

    layout->addWidget(strategyBox);
    layout->addWidget(viewBox);
    layout->addWidget(selectFolderButton);  // Добавляем кнопку на layout
    layout->addWidget(treeView);
    layout->addWidget(tableView);
    layout->addWidget(chartView);
    setCentralWidget(widget);

    updateStrategy(0);
}

MainWindow::~MainWindow() {}

void MainWindow::updateStrategy(int index) {
    delete strategy;
    index == 0 ? strategy = new FolderScanStrategy() : strategy = new FileTypeScanStrategy();
}

void MainWindow::onDirectorySelected(const QItemSelection &selected) {
    if (selected.isEmpty()) return;
    QString path = selected.indexes().first().data(QFileSystemModel::FilePathRole).toString();
    model->setData(strategy->scan(path));
}

void MainWindow::updateView(int index) {
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
    // Открываем диалог выбора папки
    QString folderPath = QFileDialog::getExistingDirectory(this, "Выбрать папку");

    if (!folderPath.isEmpty()) {
        // Устанавливаем путь выбранной папки для отображения в дереве
        QFileSystemModel *fileSystemModel = new QFileSystemModel(this);
        fileSystemModel->setRootPath(folderPath);  // Устанавливаем выбранную папку как корень
        treeView->setModel(fileSystemModel);  // Обновляем модель

        // Обновляем модель с данными по выбранной папке
        model->setData(strategy->scan(folderPath));
    }
}
