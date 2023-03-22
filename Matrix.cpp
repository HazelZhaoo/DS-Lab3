#include "Matrix.hpp"
#include <iterator>

Matrix::MyIterator::MyIterator(Node* some_node)
{
    current_node = some_node;
    row_start = some_node;
}

Matrix::MyIterator& Matrix::MyIterator::operator++() //prefix
{
    if (current_node->next_row != nullptr) {
        // If there is a next element in the current row, move to it
        current_node = current_node->next_row;
    }
    else {
        // If there is no next element in the current row, move to the first element of the next row
        row_start = row_start->next_col;
        current_node = row_start;
    }
    return *this;
}

Matrix::MyIterator Matrix::MyIterator::operator++(int dummy) //postfix
{
    MyIterator temp(*this);
    current_node = current_node->next_row;
    current_node = current_node->next_col;
     
    return temp;
}

int& Matrix::MyIterator::operator*()
{
    return current_node->value;
}

int* Matrix::MyIterator::operator->()
{
    return &(current_node -> value); //& is the address of the pointer
}

bool Matrix::MyIterator::operator==(const MyIterator& it)
{
    return current_node == it.current_node;
}

bool Matrix::MyIterator::operator!=(const MyIterator& it)
{
//    if (current_node == nullptr || it.current_node == nullptr) {
//            return current_node != it.current_node; // handles case where one node is nullptr
//        }
    return current_node != it.current_node;
}
 

Matrix::MyIterator& Matrix::MyIterator::operator=(const MyIterator& it)
{
    if(this != &it)
    {
        current_node = it.current_node;
        row_start = it.row_start;
    }
    
    return *this;
}

Matrix::Matrix():head(nullptr) , num_rows(0) , num_cols(0){};

Matrix::Matrix(int array[][MAX_COL_SIZE], int row_size, int col_size)
{
    head = new Node();
    Node* current_col = head;
    num_rows = row_size;
    num_cols = col_size;

    // Resize the array if necessary
        if (num_cols > MAX_COL_SIZE)
        {
            num_cols = MAX_COL_SIZE;
        }

    // Create nodes horizontally based on the number of columns
    for(int i = 0; i < num_cols; i++)
    {
        current_col->next_col = new Node(-1); // Insert a dummy value, which will be overwritten later
        current_col = current_col->next_col;
    }

    // Create nodes vertically based on the number of rows and insert the value at once
    Node* temp = head;
    for(int i = 0; i < num_cols; i++)
    {
        Node* new_head = head;
        for(int j = 0; j < num_rows; j++)
        {
            new_head->next_row = new Node(array[j][i]);
            new_head = new_head->next_row;
        }
        temp = temp->next_col;
    }
}

Matrix::~Matrix() //destructor
{
    clear();
}

// Copy constructor
Matrix::Matrix(const Matrix& copy_m)
{
    copyFrom(copy_m);
}


Matrix& Matrix::operator=(const Matrix& obj) //assignment operator
{
     if (this == &obj) // self-assignment check
     {
         return *this;
     }
     
    clear();
    copyFrom(obj);
     return *this;
}

void Matrix::clear()
{
    // Delete the current matrix
   Node* traverse_ptr = head; //tried using iterator but cant compare to nullptr
   while(traverse_ptr != nullptr)
   {
       Node* delete_row = head; //assign the first row as the now to be delete
       while(delete_row != nullptr) //delete nodes horizontally
       {
           Node* delete_node = delete_row; // hold the first node
           delete_row = delete_row->next_col;
           delete delete_node; //delete that node
       }
       traverse_ptr = traverse_ptr ->next_row; // go to the next row and do the same thing
       
   }
    
}

int* Matrix::getRow(int row_num) const //assuming the index start at 0//
{
    if(row_num >= num_rows)
        throw runtime_error("Row number cannot exceed the number of rows!");
    
    Node* temp = head;//does the row and column start at 0 or 1
    int* arr = new int[num_cols];
    
    for(int i = 0 ; i < num_rows ; i++)
    {
        temp = temp->next_row;
    }
    
    for (int i = 0; i < num_cols && temp != nullptr; i++, temp = temp->next_col)
    {
        arr[i] = temp->value;
    }
 
    return arr;
}

int* Matrix::getCol(int col_num) const
{
    if(col_num >= num_cols)
    {
        throw runtime_error("Column number cannot exceed the number of columns!");
    }
    int* arr = new int[num_rows];
    Node* temp = head;
    for(int i = 0 ; i < col_num ; i++)
    {
        temp = temp->next_col;
    }
    for(int i = 0 ; i < num_rows && temp != nullptr ; i++ , temp = temp->next_row)
    {
        arr[i] = temp->value;
    }
    return arr;
}

Matrix& Matrix::transpose()
{
    static Matrix trans; //a matrix to be returned by reference
    Node* ptr_to_head = trans.head; //a pointer to the node of the return matrix
    trans.num_rows = this->num_cols; //swap the rows and cols of the input parameter
    trans.num_cols = this->num_rows;
    
    for(int i = 0 ; i < this->num_rows ; i++)
    {
        int* current_row = this->getRow(i); //get the row
        int* current_node = current_row; //now hold the node to traverse through it
        for(int j = 0 ; j < this->num_cols ; j++ )
        {
            ptr_to_head->value = current_node->value; //assign value to corresponding node
            ptr_to_head ->next_row = new Node(); //create new node on the next row//
            ptr_to_head = ptr_to_head->next_row; //traverse row wise
            current_node = current_node->next_col; //traverse column wise
        }
    }
    return trans;
}

void Matrix:: copyFrom(const Matrix& obj)
{
    // Copy the other matrix
    head = new Node(obj.head->value); //head is copied
    Node* current_col = head;
    Node* obj_head = obj.head->next_col; //will start copying from the next element
    num_rows = obj.num_rows;
    num_cols = obj.num_cols;
    for (int i = 1; i < num_cols; i++)
    {
        current_col->next_col = new Node(obj_head->value); //create node and copy the first row as well//
        current_col = current_col->next_col;
        obj_head = obj_head->next_col;
    }
    
    Node* obj_temp = obj.head->next_row; // a pointer to the second row of obj
    Node* temp = head->next_row; // a pointer to the second row
    for (int i = 1; i < num_cols; i++)
    {
        Node* new_head = temp->next_row;
        for (int j = 0; j < num_rows; j++)
        {
            new_head->next_row = new Node(obj_temp->value);
            new_head = new_head->next_row;
            obj_temp = obj_temp->next_row;
        }
        temp = temp->next_col;
    }
}
