//
// Created by krzysiu on 02.11.2019.
//

#include <sstream>
#include "../include/CSVlib.h"

CSVlib::CSVlib(string pathToFile, bool head, char delimiter)
        : delimiter(delimiter), head(head) {
    open(pathToFile, head, delimiter);
}

bool CSVlib::open(const string& pathToFile, bool head, char delimiter) {
    this->delimiter = delimiter;
    this->head = head;
    csvFile.open(pathToFile);
    if (!csvFile.is_open()) {
        addError("Can't open file: " + pathToFile, true);
        return false;
    } else {
        if(head){
            if(next()){
                headV = getFileRow(readedLine);

                //Checking strings and deleting quotes
                for (string &cell: headV) {
                    if (cell.size() != 0) {
                        cell = prepareString(cell);
                    }
                }
                cols = headV.size();
            } else {
                addError("Can't get line when getting head", true);
                return false;
            }
        } else {
            if (next()){
                cols = getFileRow(readedLine).size();
                csvFile.seekg(0, csvFile.beg);
            } else {
                addError("Can't get First line in file", true);
                return false;
            }
        }
    }
    return true;
}

bool CSVlib::isOk() {
    return !criticalError;
}

vector<string> CSVlib::getFileRow(string line) {
    vector<string> row;
    stringstream lineStream(line);
    string field;
    //Read row
    while (getline(lineStream, field, delimiter)) {
        row.push_back(field);
    }

    //Check that last sign is delimiter
    if (line[line.size() - 1] == delimiter)
        row.emplace_back("");

    return row;
}

string CSVlib::prepareString(string txt) {
    short cquotes = count(txt, "\"");
    //cout << txt << " - " << cquotes << " - " << (cquotes % 2 != 0) << endl;
    if (cquotes % 2 != 0) {
        addError("Not even amount of quotes");
        return "";
    }

    if (cquotes == 0) {
        //If not find quotes
        if (ignoreWhiteChars) {
            return deleteWhiteChars(txt);
        } else {
            return txt;
        }
    } else {
        //If find quotes
        short doublecquotes = count(txt, "\"\"");
        if (cquotes == doublecquotes + 2) {
            //If find single qoutes
            string txtout = deleteWhiteChars(txt);
            //Checking quotes on begin and end
            //cout<< txtout << " " << (int)txtout[0] << "-" << txtout[0] <<endl;
            /*if(txtout == "\"\""){
                txtout = "";
            } else*/
            if (txtout[0] == '\"' && txtout[txtout.size() - 1] == '\"') {
                txtout = txtout.substr(1, txtout.size() - 2);
                return correctQuotes(txtout);
            } else {
                //throeing error
                addError("Find one quote in: " + txt);
                return "";
            }
        } else {
            //If only double quotes
            if (ignoreWhiteChars) {
                txt = deleteWhiteChars(txt);
            }

            return correctQuotes(txt);
        }
    }
}

void CSVlib::setIgnoreWhiteChars(bool ignoreWhiteChars) {
    this->ignoreWhiteChars = ignoreWhiteChars;
}

string CSVlib::deleteWhiteChars(string txt) {
    //Delete white chars from begin
    int indx = 0;
    while (isspace(txt[indx]) && indx < txt.size()) {
        indx++;
    }
    txt = txt.substr(indx);
    indx++;

    //delete white chars from end
    indx = txt.size() - 1;
    while (indx > 0 && isspace(txt[indx])) {
        indx--;
    }
    txt = txt.substr(0, indx + 1);
    return txt;
}

string CSVlib::correctQuotes(string txt) {
    //corecting quotes and checking correction
    if (txt == "\"\"")
        return "";

    int i = 0;
    while (i < txt.size()) {
        if (txt[i] == '"') {
            if (txt[i + 1] == '"') {
                txt.erase(i, 1);
            } else {
                addError("Quotes error in line: " + txt);
                return "";
            }
        }
        i++;
    }
    return txt;
}

bool CSVlib::next() {
    if(csvFile.is_open()) {
        if(getline(csvFile, readedLine)){
            if (isspace(readedLine[readedLine.size() - 1]))
                readedLine.pop_back();
        }
        else {
            readedLine = "";
            return false;
        }
    } else {
        readedLine = "";
        addError("File was closed!!", true);
        return false;
    }
    return true;
}

int CSVlib::count(string txt, string find) {
    if (txt.size() == 0 || find.size() == 0)
        return 0;

    int count = 0;
    for (int i = 0; i <= txt.size() - find.size(); i++) {
        if (txt[i] == find[0]) {
            bool equal = true;
            int j;
            for (j = 0; j < find.size(); j++) {
                if (txt[i + j] != find[j]) {
                    equal = false;
                    break;
                }
            }
            i += j - 1;
            if (equal) count++;
        }
    }

    return count;
}

unsigned short CSVlib::getReadLinesError() {
    return readLinesError;
}

void CSVlib::addError(const string& msg, bool critical) {
    errors.push_back(msg);
    readLinesError++;
    criticalError = critical;
}

vector<string> CSVlib::getErrorsList() {
    return errors;
}

vector<string> CSVlib::getHead() {
    return headV;
}

vector<string> CSVlib::getRow() {
    vector <string> ret;
    do {
        ret = getFileRowChecked(readedLine);
    }
    while(ret.size() == 0);
    return ret;
}

map<string, string> CSVlib::getRowMap() {
    map<string, string> output;
    vector <string> ret;
    do {
        ret = getFileRowChecked(readedLine);
    }
    while(ret.size() == 0);
    for(int i = 0; i< headV.size(); i++){
        output[headV[i]] = ret[i];
    }
    return output;
}

vector<string> CSVlib::getFileRowChecked(const string& line) {
    vector <string> rows = getFileRow(line);
    if (rows.size() != cols) {
        addError("Line :" + line + " - have no enought cells");
        return vector <string>();
    }

    //Checking strings and deleting quotes
    for (string &cell: rows) {
        if (cell.size() != 0) {
            cell = prepareString(cell);
        }
    }
    return rows;
}

void CSVlib::close() {
    csvFile.close();
    headV.clear();
    headV.shrink_to_fit();
    errors.clear();
    headV.shrink_to_fit();
    criticalError = false;
    delimiter = ',';
    head = true;
    ignoreWhiteChars = false;
    readedLine = "";
    readLinesError = 0;
    cols = 0;
}


