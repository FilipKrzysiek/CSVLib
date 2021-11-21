//
// Created by filip on 13.11.2021.
//

#include "../include/CsvOutput.h"

CsvOutput::CsvOutput(string filePath, char delimiter) : filePath(filePath), delimiter(delimiter) {

}

void CsvOutput::addHeadLabel(const string &label) {
    if (!lockedHeadModification)
        rowValues[label] = "";
    else throw "Try to add new label if start saving data!";
}

void CsvOutput::setData(const string &label, const string &value) {
    lockedHeadModification = true;
    rowValues[label] = value;
}

void CsvOutput::flushRow() {
    bool thisIsFirst = true;
    if (!headIsSaved) {
        headIsSaved = true;
        for (auto &[key, value]: rowValues) {
            if (thisIsFirst) thisIsFirst = false;
            else output << delimiter << " ";

//            if(isThereSpace(key))
            output << "\"" << key << "\"";
//            else
//                output<< "\"" << key << "\"";
        }
        output << endl;
    }

    thisIsFirst = true;
    for (auto &[key, value]: rowValues) {
        if (thisIsFirst) thisIsFirst = false;
        else output << delimiter << " ";

//            if(isThereSpace(key))
        output << "\"" << value << "\"";
    }
    output << endl;
}

bool CsvOutput::isThereSpace(const string &text) {
    for (auto &ch: text) {
        if (ch == ' ') return true;
    }
    return false;
}

void CsvOutput::flushToFile() {
    ofstream outputStream(filePath, ios_base::out);
    if(outputStream.fail()) {
        throw "Error on access to file";
    }
    outputStream << output.rdbuf();
    outputStream.close();

}

void CsvOutput::addHeaders(const vector<string> &labels) {
    if (!lockedHeadModification) {
        for (auto &label: labels) {
            rowValues[label] = "";
        }
        lockedHeadModification = true;
    }
    else throw "Try to add new label if start saving data!";
}

void CsvOutput::passRow(const map<string, string> &row) {
    for(auto &[key, value]: rowValues) {
        if(row.find(key) == row.end()) {
            throw "Passed map not have key: " + key;
        }
        rowValues[key] = row.at(key);
//        value = row.at(key);
    }
    flushRow();
}
