#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>

using namespace std;

// Function to check if a string is numeric
bool isNumeric(const string& s) {
    if (s.empty()) return false; // Prevent empty values
    bool decimalFound = false;
    for (char c : s) {
        if (!isdigit(c)) {
            if (c == '.' && !decimalFound) {
                decimalFound = true; // Allow one decimal point
            }
            else if (c != '-') {
                return false; // Allow negative sign only at start
            }
        }
    }
    return true;
}

int main() {
    string csvFile = "C:\\Users\\vince\\OneDrive\\Desktop\\SVM\\Project 1\\Training_dataset.csv";
    string svmFile = "C:\\Users\\vince\\OneDrive\\Desktop\\SVM\\Project 1\\Training.libsvm";

    ifstream inFile(csvFile);
    ofstream outFile(svmFile);

    if (!inFile) {
        cerr << "Error opening input file: " << csvFile << endl;
        return 1;
    }
    if (!outFile) {
        cerr << "Error opening output file: " << svmFile << endl;
        return 1;
    }

    string line;

    int lineNumber = 1; // Track line numbers for debugging
    while (getline(inFile, line)) {
        if (line.empty() || line.find_first_not_of(", ") == std::string::npos) {
            lineNumber++;
            continue; // Skip to the next line
        }

        stringstream ss(line);
        string value;
        bool first = true;
        int colIndex = 1; // LIBSVM feature index starts at 1

        while (getline(ss, value, ',')) {
            // Trim whitespace
            value.erase(0, value.find_first_not_of(" \t\r\n"));
            value.erase(value.find_last_not_of(" \t\r\n") + 1);

            // Skip empty values
            if (value.empty()) {
                colIndex++;
                continue;
            }

            // Check if the value is numeric
            if (!isNumeric(value)) {
                cerr << "Error: Non-numeric value in line " << lineNumber << ": '" << value << "'" << endl;
                // Optionally: return 1; // Comment this out if you want to keep processing
            }

            if (first) {
                //check if it is 1 or 0 and make it 1 or -1
                if (stoi(value) < 1)
                    value = "-1";
                outFile << value; // First value is label
                first = false;
            }
            else {
                outFile << " " << colIndex << ":" << value; // Feature index and value
            }
            colIndex++;
        }
        outFile << "\n"; // End of line for LIBSVM format
        lineNumber++;
    }

    inFile.close();
    outFile.close();
    cout << "LIBSVM data written to " << svmFile << " successfully!" << endl;

    return 0;
}
