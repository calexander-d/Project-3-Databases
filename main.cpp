#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "WineReview.h"
#include "QuickSort.h"

using namespace std;

vector<WineReview> readCSV(const string& filename) {
    vector<WineReview> data;
    ifstream file(filename);
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

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    cout << "Initializing data..." << endl;
    string filename = "winemag-data.csv";
    vector<WineReview> reviews = readCSV(filename);
    cout << "Loaded " << reviews.size() << " wine reviews." << endl;

    int fieldSelect, sortOrder, algorithmSelect;
    bool isAscending;
    cout << "Choose a field to sort by:\n1. Region\n2. Price\n3. Points" << endl;
    cin >> fieldSelect;

    cout << "Choose order:\n1. Ascending\n2. Descending"<< endl;
    cin >> sortOrder;

    cout << "Choose sorting algorithm:\n1. Merge Sort\n2. Quick Sort" << endl;
    cin >> algorithmSelect;

    string field;
    switch (fieldSelect) {
        case 1: field = "region_1"; break;
        case 2: field = "price"; break;
        case 3: field = "points"; break;
        default: cout << "Invalid choice." << endl; return 1;
    }
    switch (sortOrder) {
        case 1: isAscending = true; break;
        case 2: isAscending = false; break;
        default: cout << "Invalid choice." << endl; return 1;
    }

    string typeOfAlgorithm = "null";
    switch (algorithmSelect) {
    case 1: start = chrono::high_resolution_clock::now();  sort_field = field; mergeSort(reviews, 0, reviews.size() - 1); typeOfAlgorithm = "Mergesort"; break;
    case 2: start = chrono::high_resolution_clock::now();  quicksort(reviews, field, isAscending); typeOfAlgorithm = "Quicksort"; break;
        default: cout << "Invalid choice." << endl; return 1;
    }
    auto end = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> duration = end - start;
    if (typeOfAlgorithm != "null")
    {
        cout << typeOfAlgorithm << " took " << duration.count() << " ms/n" << endl;
    }

    cout << "Attempting to write to output.csv..." << endl;
    writeToFile(reviews, "output.csv");
    cout << "Saved to output.csv" << endl;
    return 0;
}
