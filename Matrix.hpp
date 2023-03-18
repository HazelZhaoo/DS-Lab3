//
//  Matrix.hpp
//  Lab3 _assignment
//
//  Created by Hazel Zhao on 2023-03-11.
//
#include <iostream>
#include <iterator>
using namespace std;
#ifndef Matrix_hpp
#define Matrix_hpp

 
class Node
{
public:
    int value;
    Node* next_col;
    Node* next_row;
    
    Node():value(0) , next_col(nullptr) , next_row(nullptr){};
    Node(Node* ptr , int val , Node* next_rowptr):next_row(next_rowptr) , value(val) , next_col(ptr) {};
};

class Matrix
{
private:
    Node* head; //a pointer to the head of the matrix//
    int num_rows;
    int num_cols;
public:
    Matrix();
    Matrix(int** array, int row_size , int col_size);  
    Matrix(const Matrix& copy_m);
    Matrix& operator =(const Matrix& obj);
    Matrix(const Matrix&& obj);
    Matrix& operator=(const Matrix&& obj);
    ~Matrix();
    Node* getRow(int row_num); //throw exception
    Node* getCol(int col_num);//throw exception
    Matrix transpose(int Matrix); // the user shouldnt be aware of the node type // should the 2d array be passed instead//
    Matrix operator+(Matrix obj); //throw exception if row not equal row
    Matrix operator*(Matrix obj); //throw exception if not multiplicable
   friend ifstream& operator>>(ifstream& input, Matrix& obj);
   friend ostream& operator<<(ostream& output , Matrix& obj);

     
    class MyIterator : public std::iterator<std::forward_iterator_tag  , int>
    {
    private:
        Node* current_node;
        Node* startOfMatrix;
    public:
        MyIterator(Node* some_node , Node* some_head);
        MyIterator& operator++();
        MyIterator& operator++(int dummy); //pre post fix
        int& operator*(); //dereference
        int* operator->();
        bool operator ==(const MyIterator& it);
        bool operator !=(const MyIterator& it);
       
    };
    
};

#endif /* Matrix_hpp */
