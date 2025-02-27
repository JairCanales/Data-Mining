// This code implements a K-Nearest neighbor (KNN) classifier using Jaccard's distance.

// Headers & Structs
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// Structure to store data points
struct DataPoint {
    vector<double> features;
    string label;
};

// Function Declarations 
vector<DataPoint> gatherData(string fileName);
vector<double> normalizeCol(vector<DataPoint> data, int col);

int main(int argc, char* argv[])
{
    // make sure there are 2 arguments
    if (argc != 3)
    {
        cout << "Usage: <F> <T>" << endl;
        cout << "F: Name of the file" << endl;
        cout << "T: Array of Colomns to be Normalized" << endl;
        return 1;
    }


    // collect the data from arguments
    string fileName = argv[1];
    // Collect column indices to normalize
    vector<int> columns;
    stringstream ss(argv[2]);  // Use stringstream to split string
    string temp;

    while (getline(ss, temp, ',')) {  // Split by comma
        columns.push_back(stoi(temp));  // Convert to integer and store
    }
    //gather the data
    vector<DataPoint> data = gatherData(fileName);
    //gather the columns
    vector<vector<double>> normalizedCol(columns.size(), vector<double>(data.size(), 0.0));
    for (int i = 0; i < columns.size(); i++) {
        normalizedCol[i] = normalizeCol(data, columns[i]);
    }
    //print them out
    for (int i = 0; i < data.size(); i++) {
        cout << data[i].label << ": ";
        for (int j = 0; j < columns.size(); j++) {
            cout << normalizedCol[j][i] << ", ";
        }
        cout << endl;
    }
    
}

vector<DataPoint> gatherData(string fileName) {
    vector<DataPoint> data;
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return {};
    }

    string line;

    while (getline(file, line)) {
        DataPoint point;
        stringstream ss(line);
        string value;

        //first line is label
        try {
            getline(ss, value, '\t');
            point.label = value;
        }
        catch (const invalid_argument& e) {
            cerr << "Error: Non-binary value in file " << fileName << endl;
        }



        while (getline(ss, value, '\t')) {
            try {
                point.features.push_back(stod(value));  // Convert to integer (0 or 1)
            }
            catch (const invalid_argument& e) {
                cerr << "Error: Non-binary value in file " << fileName << endl;
            }
        }
        data.push_back(point);

    }

    file.close();
    return data;
}
vector<double> normalizeCol(vector<DataPoint> data, int col) {
    //set the column to the array version
    col = col - 1;
    //find the min and max
    double min = data[0].features[col], max = data[0].features[col];
    //start at 1 because 0 is gotten
    for (int i = 1; i < data.size(); i++) {
        double value = data[i].features[col];
        if (value < min) {
            min = value;
        }
        if (value > max) {
            max = value;
        }
    }
    cout << "Min: " << min << " Max: " << max << endl;
    vector<double> normalizedCol(data.size(), 0.0);

    //now normalize the column
    for (int i = 0; i < data.size(); i++) {
        double normValue = (data[i].features[col] - min) / (max - min);
        normalizedCol[i] = normValue;
    }
    return normalizedCol;
}
