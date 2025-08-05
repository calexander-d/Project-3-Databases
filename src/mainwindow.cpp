#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QuickSort.h"
#include "sortutils.h"
#include <QStandardItemModel>
#include <QTime>
#include <fstream>

#include <sstream>

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::onSearchClicked);
    cout << "Initializing data..." << endl;
    string filename = "winemag-data.csv";
    wineData = readCSV(filename);
    cout << "Loaded " << wineData.size() << " wine reviews." << endl;
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::onSearchClicked() {
    std::vector<WineReview> reviews = wineData;
    // cout << "check" <<endl;
    string sortField;
    bool ascending = ui->ascending->isChecked();
    bool useMerge = ui->merge->isChecked();
    if (ui->region->isChecked()) sortField = "region_1";
    else if (ui->points->isChecked()) sortField = "points";
    else if (ui->price->isChecked()) sortField = "price";
    currentSortField = QString::fromStdString(sortField);


    auto start = chrono::high_resolution_clock::now();

    if (useMerge) {
        sort_field = sortField;
        isAscending = ascending;
        mergeSort(reviews, 0, reviews.size() - 1);
    } else {
        sort_field = sortField;
        isAscending = ascending;
        quicksort(reviews, sortField, isAscending);
        // cout << "used" << endl;
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    ostringstream stream;
    stream << fixed << setprecision(3) << duration.count();
    string strdur = stream.str();
    ui->sorttime->setText(("Sort Time: " + strdur + " ms").data());
    displayResults(reviews);
}

void MainWindow::displayResults(const std::vector<WineReview> &data) {
    tableModel = new QStandardItemModel(data.size(), 12, this);
    tableModel->setHorizontalHeaderLabels({
        "Country", "Description", "Designation", "Province", "Region", "Taster Name",
        "Twitter", "Title", "Variety", "Winery", "Points", "Price"
    });

    for (int row = 0; row < (int)data.size(); ++row) {
        const WineReview& r = data[row];
        tableModel->setItem(row, 0, new QStandardItem(QString::fromStdString(r.country)));
        tableModel->setItem(row, 1, new QStandardItem(QString::fromStdString(r.description)));
        tableModel->setItem(row, 2, new QStandardItem(QString::fromStdString(r.designation)));
        tableModel->setItem(row, 3, new QStandardItem(QString::fromStdString(r.province)));
        tableModel->setItem(row, 4, new QStandardItem(QString::fromStdString(r.region_1)));
        tableModel->setItem(row, 5, new QStandardItem(QString::fromStdString(r.taster_name)));
        tableModel->setItem(row, 6, new QStandardItem(QString::fromStdString(r.taster_twitter_handle)));
        tableModel->setItem(row, 7, new QStandardItem(QString::fromStdString(r.title)));
        tableModel->setItem(row, 8, new QStandardItem(QString::fromStdString(r.variety)));
        tableModel->setItem(row, 9, new QStandardItem(QString::fromStdString(r.winery)));
        tableModel->setItem(row, 10, new QStandardItem(QString::number(r.points)));
        tableModel->setItem(row, 11, new QStandardItem(QString::number(r.price)));


    }
    int sortCol = -1;
    if (currentSortField == "country") sortCol = 0;
    else if (currentSortField == "description") sortCol = 1;
    else if (currentSortField == "designation") sortCol = 2;
    else if (currentSortField == "province") sortCol = 3;
    else if (currentSortField == "region_1") sortCol = 4;
    else if (currentSortField == "taster_name") sortCol = 5;
    else if (currentSortField == "taster_twitter_handle") sortCol = 6;
    else if (currentSortField == "title") sortCol = 7;
    else if (currentSortField == "variety") sortCol = 8;
    else if (currentSortField == "winery") sortCol = 9;
    else if (currentSortField == "points") sortCol = 10;
    else if (currentSortField == "price") sortCol = 11;
    if (sortCol != -1) {
        for (int row = 0; row < data.size(); ++row) {
            QStandardItem* item = tableModel->item(row, sortCol);
            item->setBackground(QBrush(QColor("#001833")));
        }
    }

    ui->tableView->setModel(tableModel);

}
