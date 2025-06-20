#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sys/stat.h>  // for fileExists
#include <cstdio>      // for remove()
#include <chrono>      // for timing

using namespace std;
using namespace std::chrono;

// Check if a file exists
bool fileExists(const string& filename) {
    struct stat buffer;
    return (stat(filename.c_str(), &buffer) == 0);
}

// Compare user's output with expected output
bool compareOutputs(const string& userFile, const string& expectedFile) {
    ifstream user(userFile), expected(expectedFile);
    if (!user.is_open() || !expected.is_open()) {
        cerr << "Could not open output files for comparison.\n";
        return false;
    }

    string userLine, expectedLine;
    int lineNum = 1;
    while (getline(expected, expectedLine)) {
        if (!getline(user, userLine)) {
            cerr << "Fewer lines than expected at line " << lineNum << "\n";
            return false;
        }
        if (userLine != expectedLine) {
            cerr << "Mismatch at line " << lineNum << ":\n";
            cerr << "   Expected: \"" << expectedLine << "\"\n";
            cerr << "   Got     : \"" << userLine << "\"\n";
            return false;
        }
        ++lineNum;
    }

    if (getline(user, userLine)) {
        cerr << "Extra output starting at line " << lineNum << "\n";
        return false;
    }

    return true;
}

int main() {
    string userCode;
    cout << "Submit file: ";
    cin >> userCode;

    string execName = "user_exec";

    // Compile user solution
    string compileCommand = "g++ \"" + userCode + "\" -o " + execName + " -std=c++17";
    if (system(compileCommand.c_str()) != 0) {
        cerr << "❌ Compilation failed.\n";
        return 1;
    }

    string inputDir = "Test Cases/inputs/";
    string outputDir = "Test Cases/outputs/";
    string userDir = "./user_outputs/";

    string createDir = "mkdir -p " + userDir;
    system(createDir.c_str());

    int totalCases = 0, passedCases = 0;
	string timeLimit = "1s";

    for (int i = 1;; ++i) {
        string inputFile = inputDir + "input" + to_string(i) + ".txt";
        string expectedOutputFile = outputDir + "output" + to_string(i) + ".txt";
        string actualOutputFile = userDir + "output" + to_string(i) + ".txt";

        if (!fileExists(inputFile) || !fileExists(expectedOutputFile)) {
            break;
        }

        ++totalCases;
		cout << "\033[0mTest Case #" << i << ":\n";

        // Run with timeout and measure time
        string runCommand = "gtimeout " + timeLimit + " ./" + execName + " < \"" + inputFile + "\" > \"" + actualOutputFile + "\"";
        
        auto start = high_resolution_clock::now();
        int runStatus = system(runCommand.c_str());
        auto end = high_resolution_clock::now();
        duration<double> durationSec = end - start;

        if (WIFEXITED(runStatus) && WEXITSTATUS(runStatus) == 124) {
            cout << "\033[34mTime Limit Exceeded (expected: " <<  timeLimit << ", got: " << durationSec.count() << "s)\033[0m" << "\n";
			break;
        }

        if (runStatus != 0) {
            cout << "\033[34mRuntime Error\n\033[0m" << "\n";
			break;
        }

        if (compareOutputs(actualOutputFile, expectedOutputFile)) {
            cout << "\033[32mAccepted in " << durationSec.count() << "s\n\033[0m";
			++passedCases;
        } else {
			cout << "\033[34mWrong Answer\n\033[0m" << "\n";
			break;
		}
	}

    cout << "\nSummary: " << passedCases << " / " << totalCases << " test cases passed.\n";

    // Clean up executable
    remove(execName.c_str());

    // Clean up user output directory
    string cleanupCommand = "rm -rf " + userDir;
    system(cleanupCommand.c_str());

    return 0;
}
