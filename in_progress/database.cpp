#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "database.h"

using namespace std;

Database::Database(){
    _column_names = vector<string>();
    _columns = vector<vector<uint32_t>>();
}

void Database::DeclareColumns(string* names, uint32_t number_of_columns){
    for(uint32_t i = 0; i < number_of_columns; i++){
        _column_names.push_back(names[i]);
    }
    _number_of_columns = number_of_columns;
}

void Database::AddRow(vector<uint32_t> data){
    _columns.push_back(data);
}

void Database::PrintDatabase(){
    vector<string>::iterator it;
    for(it = _column_names.begin(); it < _column_names.end(); it++){
        cout << *it << " * ";
    }
    cout << "\n";
    vector<vector<uint32_t>>::iterator it_rows;
    for(it_rows = _columns.begin(); it_rows < _columns.end(); it_rows++){
        for(uint_fast32_t idx = 0; idx < _number_of_columns; idx++){
            cout << (*it_rows)[idx] << " ";
        }
        cout << "\n";
    }
}

vector<uint32_t> Database::GetRow(uint32_t row_number){
    return _columns[row_number - 1];
}