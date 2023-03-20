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

template <typename T>
class Node
{
public:
    int value;
    Node<T>* next_col;
    Node<T>* next_row;

    Node() :value(0), next_col(nullptr), next_row(nullptr) {};
    Node(Node<T>* ptr, int val, Node<T>* next_rowptr) :next_row(next_rowptr), value(val), next_col(ptr) {};
};

template <typename T>
class Matrix
{
private:
    Node<T>* head; //a pointer to the head of the matrix//
    int num_rows;
    int num_cols;
public:
    Matrix();//--
    Matrix(int** array, int row_size, int col_size);//--
    Matrix(const Matrix<T>& copy_m);//--
    Matrix<T>& operator =(const Matrix<T>& obj);
    Matrix(const Matrix<T>&& obj);
    Matrix<T>& operator=(const Matrix<T>&& obj);
    ~Matrix();//--
    Node<T>* getRow(int row_num); //throw exception
    Node<T>* getCol(int col_num);//throw exception

    //getElement

    Matrix<T> transpose(int Matrix); // the user shouldnt be aware of the node type // should the 2d array be passed instead//
    Matrix<T> operator+(Matrix<T> obj); //throw exception if row not equal row
    Matrix<T> operator*(Matrix<T> obj); //throw exception if not multiplicable
   
    friend ostream& operator<<(ostream& output, Matrix<T>& obj);


    class MyIterator : public std::iterator<std::forward_iterator_tag, int>
    {
    private:
        Node<T>* current_node;
        Node<T>* startOfMatrix;
    public:
        MyIterator(Node<T>* some_node, Node<T>* some_head);
        MyIterator& operator++();
        MyIterator& operator++(int dummy); //pre post fix
        int& operator*(); //dereference
        int* operator->();
        bool operator ==(const MyIterator& it);
        bool operator !=(const MyIterator& it);

    };

};

#endif /* Matrix_hpp */
