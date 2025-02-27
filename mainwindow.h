#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "IScanStrategy.h"
#include "file_size_model.h"

#include <QFileSystemModel>
#include <QMainWindow>
#include <QTreeView>
#include <QChartView>
#include <QComboBox>
#include <QTableView>
#include <QPushButton>  // Для кнопки выбора папки

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateStrategy(int index);

    void onDirectorySelected(const QItemSelection &selected);

    void updateView(int index);

    void showPieChart();

    void showBarChart();

    void onSelectFolder();  // Слот для кнопки выбора папки

private:
    QFileSystemModel *fileSystemModel;
    QTreeView *treeView;
    QTableView *tableView;
    QChartView *chartView;
    QComboBox *strategyBox;
    QComboBox *viewBox;
    QPushButton *selectFolderButton;  // Кнопка для выбора папки
    FileSizeModel *model;
    IScanStrategy *strategy;

    QString _currentFolderPath {};
    int _currentView = 0;
};
#endif // MAINWINDOW_H
