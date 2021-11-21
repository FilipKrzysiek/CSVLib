//
// Created by filip on 13.11.2021.
//

#ifndef LIBRARIES1_CSVOUTPUT_H
#define LIBRARIES1_CSVOUTPUT_H

#include <fstream>
#include <sstream>
#include <map>
#include <vector>

using namespace std;

class CsvOutput {
private:
    const char delimiter;
    string filePath;
    stringstream output;

    map<string, string> rowValues;

    bool lockedHeadModification = false;
    bool headIsSaved = false;

    bool isThereSpace(const string& text);


public:
    /**
     * Constructor, you mast pass file path where data will be saved
     * @param filePath output file path
     */
    CsvOutput(string filePath, char delimiter = ',');

    void addHeadLabel(const string& label);

    void addHeaders(const vector<string> &labels);

    void setData(const string& label, const string& value);

    void passRow(const map<string, string> &row);

    void flushRow();

    void flushToFile();
};


#endif //LIBRARIES1_CSVOUTPUT_H
