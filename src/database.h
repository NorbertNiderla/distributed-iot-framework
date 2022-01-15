#include <vector>
#include <string>
#pragma once


using namespace std;

class Database{
private:
    vector<string> _column_names;
    uint_fast32_t _number_of_columns;
    vector<vector<uint32_t>> _columns;

public:
    Database();
    void DeclareColumns(string* names, uint32_t number_of_columns);
    void AddRow(vector<uint32_t> data);
    void PrintDatabase();
    vector<uint32_t> GetRow(uint32_t row_number);
};