#include <iostream>
#include "./../database.h"

bool TestDatabase(){
    Database db;
    uint32_t number_of_columns = 2;
    string column_names[] = {"SenderId", "ReceiverId"};
    db.DeclareColumns(column_names, number_of_columns);
    vector<uint32_t> data_one = vector<uint32_t>{1,2};
    vector<uint32_t> data_two = vector<uint32_t>{3,4};
    vector<uint32_t> data_three = vector<uint32_t>{5,6};
    db.AddRow(data_one);
    db.AddRow(data_two);
    db.AddRow(data_three);

    vector<uint32_t> data_ret_three = db.GetRow(3);
    vector<uint32_t> data_ret_one = db.GetRow(1);
    vector<uint32_t> data_ret_two = db.GetRow(2);

    bool result_one = equal(data_one.begin(), data_one.end(), data_ret_one.begin());
    bool result_two = equal(data_two.begin(), data_two.end(), data_ret_two.begin());
    bool result_three = equal(data_three.begin(), data_three.end(), data_ret_three.begin());

    return result_one & result_two & result_three;
}

int main(){
    if(TestDatabase())
        std::cout << "TestDatabase passed\n";
    else
        std::cout << "TestDatabase failed\n";
}