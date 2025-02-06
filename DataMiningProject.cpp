#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;
vector<vector<int>> gatherData(string fileName);

int main(int argc, char* argv[])
{
    // make sure there are 2 arguments
    if (argc != 3)
    {
        cout << "Usage: <F> <K>" << endl;
        cout << "F: Name of the training file" << endl;
        cout << "K: Name of the testing file" << endl;
        return 1;
    }


    // collect the data from arguments
    string trainName = argv[1];
    string testName = argv[2];

    vector<vector<int>> trainingData = gatherData(trainName);
    vector<vector<int>> testData = gatherData(testName);

    //print the data
    for (int i = 0; i < trainingData.size(); i++) {
        for (int j = 0; j < trainingData[0].size(); j++) {
            cout << trainingData[i][j] << ",";
        }
        cout << endl;
    }
}

vector<vector<int>> gatherData(string fileName) {
    vector<vector<int>> data;
    ifstream file(fileName);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << fileName << endl;
        return {};
    }
    

    string line;
    //discard the first line
    getline(file, line);
    while (getline(file, line)) {
        vector<int> currentLine;  // Create a new vector for each line

        stringstream ss(line);
        string name;
        getline(ss, name, ',');  // Extract and discard the name part

        string value;
        while (getline(ss, value, ',')) {
            try {
                currentLine.push_back(stoi(value));  // Convert and store integer values
            }
            catch (const invalid_argument& e) {
                cerr << "Error: Non-integer value in file " << fileName << endl;
            }
        }

        if (!currentLine.empty()) {
            data.push_back(currentLine);
        }
    }

    file.close();
    return data;
}