//
// Created by krzysiu on 02.11.2019.
//

#ifndef LIBRARIES1_CSVLIB_H
#define LIBRARIES1_CSVLIB_H


#include <string>
#include <fstream>
#include <istream>
#include <vector>
#include <map>
#include <iostream>

using namespace std;


class CSVlib {
public:
    /**
     * @brief Create object and open file and read it
     * @param fileName csv file name
     * @param head csv file have first line with col titles
     * @param delimiter default ','
     */
    CSVlib(string pathToFile, bool head = true, char delimiter = ',');

    CSVlib() {};

    /**
     * @brief Open csv file and read it
     * @param fileName csv file name
     * @param head csv file have first line with col titles
     * @param delimiter default ','
     * @return true if success or false if error
     */
    bool open(const string& pathToFile, bool head = true, char delimiter = ',');
    //TODO: Create heads for no header

    /**
     * @brief Closing file and clear
     */
    void close();

    /**
     * @brief Check critical error
     * @return
     */
    bool isOk();

    /**
     * @brief Return head. If enabled head, return head, if not generated
     * @return
     */
    vector<string> getHead();

    /**
     * @brief Return one row in vector
     * @return
     */
    vector<string> getRow();

    /**
     * @brief Return one row in map
     * @return
     */
    map<string, string> getRowMap();

    /**
     * @brief Increment iterator
     * @return if has next true, if not have false
     */
    bool next();

    /**
     * @breif Ignoring white chars on begin and end field
     * @param ignoreWhiteChars true - yes, false - no; default false
     */
    void setIgnoreWhiteChars(bool ignoreWhiteChars);

    /**
     * @brief Returns amount lines where is error
     * @return
     */
    unsigned short getReadLinesError();

    /**
     * @brief Returns vectors with all errors
     * @return
     */
    vector<string> getErrorsList();

private:
    ifstream csvFile;
    vector<string> headV;
    vector<string> errors;
    bool criticalError = false;
    char delimiter = ',';
    bool head = true;
    bool ignoreWhiteChars = false;
    string readedLine = "";
    unsigned short readLinesError = 0;
    unsigned short cols = 0;

    string prepareString(string txt);

    string deleteWhiteChars(string txt);

    string correctQuotes(string txt);

    int count(string txt, string find);

    void addError(const string& msg, bool critical = false);

    vector<string> getFileRow(string line);
    vector<string> getFileRowChecked(const string& line);
};


#endif //LIBRARIES1_CSVLIB_H
