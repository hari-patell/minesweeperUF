#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct NameTimePair {
    string name;
    string time;
};

class Leaderboard {
public:
    string display;
    vector<NameTimePair> nameTimePairs;

    void read() {
        nameTimePairs.clear();
        ifstream file("files/leaderboard.txt");
        string line;
        while (getline(file, line)) {
            stringstream ss(line);

            NameTimePair pair;

            getline(ss, pair.time, ',');
            getline(ss, pair.name);

            // Trim leading and trailing whitespace from the name
            pair.name.erase(0, pair.name.find_first_not_of(" \t")); // Remove leading whitespace
            pair.name.erase(pair.name.find_last_not_of(" \t") + 1); // Remove trailing whitespace

            // Add the pair to the vector
            nameTimePairs.push_back(pair);
        }

        // Close the file
        file.close();

        cout << "Leaderboard:" << endl;
        for (const auto& pair : nameTimePairs) {
            cout << pair.time << ", " << pair.name << endl;
        }
    }

    // Custom comparator function to compare NameTimePair objects based on time
    static bool compareNameTimePair(const NameTimePair& pair1, const NameTimePair& pair2) {
        return pair1.time < pair2.time;
    }

    void sort() {
        // Sort the vector of pairs based on time using the custom comparator function
        std::sort(nameTimePairs.begin(), nameTimePairs.end(), compareNameTimePair);

        // Open the leaderboard file for writing
        ofstream outFile("files/leaderboard.txt");

        // Check if the file opened successfully
        if (!outFile.is_open()) {
            cerr << "Error opening output file!" << endl;
            return;
        }

        // Write the sorted pairs back to the file
        for (const auto& pair : nameTimePairs) {
            outFile << pair.time << ", " << pair.name << endl;
        }

        // Close the output file
        outFile.close();

        cout << "\nLeaderboard sorted and updated in the file." << endl;
    }

    // Function to add a new entry to the leaderboard file
    void addEntry(int minutes, int seconds, const string& name) {
        // Format minutes and seconds to "00:00" format
        stringstream timeStream;
        timeStream << setw(2) << setfill('0') << minutes << ":" << setw(2) << setfill('0') << seconds;
        string formattedTime = timeStream.str();

        // Trim leading and trailing whitespace from the name
        string trimmedName = name;
        trimmedName.erase(0, trimmedName.find_first_not_of(" \t")); // Remove leading whitespace
        trimmedName.erase(trimmedName.find_last_not_of(" \t") + 1); // Remove trailing whitespace

        // Open the leaderboard file in append mode
        ofstream outFile("files/leaderboard.txt", ios::app);

        // Check if the file opened successfully
        if (!outFile.is_open()) {
            cerr << "Error opening output file!" << endl;
            return;
        }

        // Write the new entry to the file
        outFile << formattedTime << ", " << trimmedName << endl;

        // Close the output file
        outFile.close();

        cout << "\nNew entry added to the leaderboard file." << endl;
    }

    // Function to generate summary string from the first 5 name-value pairs in the file
    string generateSummary() {
        string summary;

        // Open the leaderboard file
        ifstream file("files/leaderboard.txt");

        // Check if the file opened successfully
        if (!file.is_open()) {
            cerr << "Error opening file!" << endl;
            return summary;
        }

        // Read the first 5 name-value pairs and format them into the summary string
        string line;
        int count = 0;
        while (getline(file, line) && count < 5) {
            stringstream ss(line);
            string time, name;
            getline(ss, time, ',');
            getline(ss, name);

            summary += to_string(count + 1) + "\t" + time + " " + name + "\n\n";
            count++;
        }

        // Close the file
        file.close();

        return summary;
    }
};
