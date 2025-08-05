//
// Created by Brent Engle on 8/1/25.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "WineReview.h"
#include "QuickSort.h"

using namespace std;

vector<WineReview> readCSV(const string& filename) {
    cout << "Reading data..." << endl;
    vector<WineReview> data;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open file " << filename << endl;
    }
    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        WineReview review;
        vector<string> fields;
        string field;
        bool inQuotes = false;
        string current;

        // Parse CSV here
        for (char c : line) {
            if (c == '"' && !inQuotes) {
                inQuotes = true;
            } else if (c == '"' && inQuotes) {
                inQuotes = false;
            } else if (c == ',' && !inQuotes) {
                fields.push_back(current);
                current.clear();
            } else {
                current += c;
            }
        }
        fields.push_back(current);
        if (fields.size() < 12) continue;

        review.country = fields[0];
        review.description = fields[1];
        review.designation = fields[2];
        review.points = stoi(fields[3]);
        review.price = stof(fields[4]);
        review.province = fields[5];
        review.region_1 = fields[6];
        review.taster_name = fields[7];
        review.taster_twitter_handle = fields[8];
        review.title = fields[9];
        review.variety = fields[10];
        review.winery = fields[11];

        //add review to vector
        data.push_back(review);
    }
    return data;
}

void writeToFile(const vector<WineReview>& data, const string& filename) {
    ofstream out(filename);
    out << "region_1,price,points,title\n";
    for (const auto& r : data) {
        out << '"' << r.region_1 << "\"," << r.price << "," << r.points << "," << '"' << r.title << '"' << "\n";
    }
}

string sort_field;
bool isAscending;

void merge(vector<WineReview>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<WineReview> X(n1), Y(n2);
    for (int i = 0; i < n1; ++i) {
        X[i] = arr[left + i];
    }
    for (int j = 0; j < n2; ++j) {
        Y[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        bool condition = false;

        if (sort_field == "price") {
            condition = isAscending ? (X[i].price < Y[j].price) : (X[i].price > Y[j].price);
        } else if (sort_field == "points") {
            condition = isAscending ? (X[i].points < Y[j].points) : (X[i].points > Y[j].points);
        } else if (sort_field == "region_1") {
            condition = isAscending ? (X[i].region_1 < Y[j].region_1) : (X[i].region_1 > Y[j].region_1);
        }

        if (condition) {
            arr[k++] = X[i++];
        }
        else {
            arr[k++] = Y[j++];
        }
    }

    while (i < n1){
    arr[k++] = X[i++];
    }
    while (j < n2) {
        arr[k++] = Y[j++];
    }
}

void mergeSort(vector<WineReview>& arr, int left, int right) {
    if (left < right) {
        //m is where array is divided into 2 subarrays
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge subarrays
        merge(arr, left, mid, right);
    }
}