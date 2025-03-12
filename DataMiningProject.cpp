// This code implements a K-Nearest neighbor (KNN) classifier using Jaccard's distance.

// Headers & Structs
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>

using namespace std;

// Structure to store data points
struct DataPoint {
    vector<int> features;
    int label;
};

// Function Declarations 
vector<DataPoint> gatherData(string fileName);
double jaccardDistance(const vector<int>& a, const vector<int>& b);
int classifyKNN(const vector<DataPoint>& trainingData, const vector<int>& testSample, int K);

vector<DataPoint> shuffleData(vector<DataPoint> data);
vector<vector<DataPoint>> generateFolds(vector<DataPoint> data);

int main(int argc, char* argv[])
{
    // make sure there are 2 arguments
    if (argc != 3)
    {
        cout << "Usage: <F> <K>" << endl;
        cout << "F: Name of the Test File" << endl;
        cout << "K: All or Odd (0|1)" << endl;
        return 1;
    }


    // collect the data from arguments
    string fileName = argv[1];
    int evenodd = stoi(argv[2]);
    vector<DataPoint> data = gatherData(fileName);
    data = shuffleData(data);
    //create folds
    vector<vector<DataPoint>> folds = generateFolds(data);
    //calculate increase
    int increase = 0;
    if (evenodd == 0) {
        increase = 1;
    }
    else {
        increase = 2;
    }
    //num Of Runs
    int numOfK = 1;
    
    //test the test data
    for (int h = 0; h < folds.size(); h++) {
        int TP = 0, TN = 0, FP = 0, FN = 0;
        for (int i = 0; i < folds.size(); i++) {
            //make sure that training is not the same as the tests
            if (i == h)
                continue;
            for (int j = 0; j < folds[0].size(); j++) {
                int guess = classifyKNN(folds[h], folds[i][j].features, numOfK);
                //cout << "Guess: " << guess << " | Real Data: " << testData[i].label << endl;
                //detect what it is
                int label = folds[i][j].label;
                if (guess == 0 && label == 0)
                    TN++;
                else if (guess == 1 && label == 0)
                    FP++;
                else if (guess == 0 && label == 1)
                    FN++;
                else if (guess == 1 && label == 1)
                    TP++;
            }
        }
        //display the stats
        cout << "Fold " << h << ": " << endl;
        cout << "TP: " << TP << endl;
        cout << "TN: " << TN << endl;
        cout << "FP: " << FP << endl;
        cout << "FN: " << FN << endl << endl;
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
    getline(file, line);  // Skip header line

    while (getline(file, line)) {
        DataPoint point;
        stringstream ss(line);
        string value;

        getline(ss, value, ',');  // Skip first column (name)

        //second column is label
        try {
            getline(ss, value, ',');
            point.label = stoi(value);
        }
        catch (const invalid_argument& e) {
            cerr << "Error: Non-binary value in file " << fileName << endl;
        }
            
        

        while (getline(ss, value, ',')) {
            try {
                point.features.push_back(stoi(value));  // Convert to integer (0 or 1)
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
vector<DataPoint> shuffleData(vector<DataPoint> data) {

    random_device rd;  // Obtain a random seed
    mt19937 g(rd());   // Initialize a Mersenne Twister PRNG

    // Shuffle the vector
    shuffle(data.begin(), data.end(), g);

    return data;
}
vector<vector<DataPoint>> generateFolds(vector<DataPoint> data) {
    int numOfFolds = 5;
    int dataPerFold = data.size() / numOfFolds;
    vector<vector<DataPoint>> folds(numOfFolds, vector<DataPoint>(dataPerFold));

    int index = 0;
    for (int i = 0; i < numOfFolds; i++) {
        for (int j = 0; j < dataPerFold; j++) {
            folds[i][j] = data[index];
            index++;
        }
    }
    return folds;
}
double jaccardDistance(const vector<int>& a, const vector<int>& b) {
    //cout differences and both positives
    int p = 0;
    int q = 0;
    int r = 0;
    for (int i = 0; i < a.size(); i++) {
        //check for both positive
        if (a[i] == 1 && b[i] == 1)
            p++;
        //check for a positive
        if (a[i] == 1 && b[i] == 0)
            q++;
        //check for b positive
        if (a[i] == 0 && b[i] == 1)
            r++;
    }
    //check for 0
    if (p + q + r == 0) return 1.0;
    //return the distance
    return static_cast<double>(q + r) / (p + q + r);
}
//use the jaccards distance to calculate ssn
int classifyKNN(const vector<DataPoint>& trainingData, const vector<int>& testSample, int K) {
    vector<pair<double, int>> distances;

    // Find distance
    for (const auto& trainPoint : trainingData) {
        double dist = jaccardDistance(trainPoint.features, testSample);
        distances.push_back({ dist, trainPoint.label });
    }

    // Sort by distance (ascending order)
    sort(distances.begin(), distances.end());

    // Count occurrences of each class in the K nearest neighbors
    map<int, int> classVotes;
    for (int i = 0; i < K; i++) {
        classVotes[distances[i].second]++;
    }

    // Find the class with the highest count
    int bestClass = -1;
    int maxVotes = 0;
    for (const auto& entry : classVotes) {
        if (entry.second > maxVotes) {
            maxVotes = entry.second;
            bestClass = entry.first;
        }
    }

    return bestClass;
}
