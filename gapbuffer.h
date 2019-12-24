//
// Created by Hassan Raza Bukhari on 23/12/2019.
//
#include <iostream>
#include <string.h>
#include <vector>
#ifndef GAPBUFFER_GAPBUFFER_H
#define GAPBUFFER_GAPBUFFER_H
using namespace std;
class gap_buffer{
public:
    vector <char> buffer;
    int gap_size = 10;
    int gap_left = 0;
    int gap_right = gap_size - gap_left - 1;
    int size = 10;

    void grow(int k, int position);
    void left(int position);
    void right(int position);
    void move_cursor(int position);
    void insert(string input_string, int position);
    void delete_character(int position);
    string convertToString();
};
#endif //GAPBUFFER_GAPBUFFER_H
