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
    vector<int> features;
    int label;
};

vector<DataPoint> gatherData(string fileName);
double jaccardDistance(const vector<int>& a, const vector<int>& b);
int classifyKNN(const vector<DataPoint>& trainingData, const vector<int>& testSample, int K);

int main(int argc, char* argv[])
{
    // make sure there are 3 arguments
    if (argc != 4)
    {
        cout << "Usage: <F> <T> <K>" << endl;
        cout << "F: Name of the training file" << endl;
        cout << "T: Name of the testing file" << endl;
        cout << "K: Number of Nearest Neighbors" << endl;
        return 1;
    }


    // collect the data from arguments
    string trainName = argv[1];
    string testName = argv[2];
    int nearestNeighbors = stoi(argv[3]);

    vector<DataPoint> trainingData = gatherData(trainName);
    vector<DataPoint> testData = gatherData(testName);

    //test the test data
    int numOfCorrect = 0;
    for (int i = 0; i < testData.size(); i++) {
        int guess = classifyKNN(trainingData, testData[i].features, nearestNeighbors);
        cout << "Guess: " << guess << " | Real Data: " << testData[i].label << endl;
        if(guess == testData[i].label)
            numOfCorrect++;

    }
    cout << "Prediction Score: " << (double)numOfCorrect / (double)testData.size();
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