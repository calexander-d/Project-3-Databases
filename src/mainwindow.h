#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <vector>
#include "WineReview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onSearchClicked();

private:
    Ui::MainWindow *ui;
    std::vector<WineReview> wineData;
    QStandardItemModel *tableModel;
    QString currentSortField;


    void displayResults(const std::vector<WineReview>& sortedData);
};

#endif // MAINWINDOW_H
