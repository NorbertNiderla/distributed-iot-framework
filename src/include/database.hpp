#include <vector>
#include <string>
#pragma once

#define RECV_MESSAGE_BUFFER_LEN (100)
#define DEFAULT_COMMUNICATION_PORT  (IPPORT_USERRESERVED + 1300) //6300
#define SERVER_CONNECTION_REQUEST_QUEUE    (1)

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