#include "Matrix.hpp"
#include <iterator>

Matrix::MyIterator::MyIterator(Node* some_node , Node* some_head):current_node(some_node) , startOfMatrix(some_head){}

Matrix::MyIterator& Matrix::MyIterator::operator++() //prefix
{
    if (current_node != nullptr) {
        // If there is a next element in the current row, move to it
        current_node = current_node->next_col;
    }
    else {
        // If we reached the end of the current row, move to the next row
        current_node = startOfMatrix->next_row; // Set current node to beginning of next row
        startOfMatrix = current_node; // Update start of matrix to current node
    }
    return *this;
}


Matrix::MyIterator Matrix::MyIterator::operator++(int dummy) //postfix
{
    MyIterator temp(*this);
    ++(*this);
     
    return temp;
}

int& Matrix::MyIterator::operator*()
{
    return current_node->value;
}

int* Matrix::MyIterator::operator->()
{
    return &current_node -> value; //& is the address of the pointer
}

bool Matrix::MyIterator::operator==(const MyIterator& it)
{
    return current_node == it.current_node;
}

bool Matrix::MyIterator::operator!=(const MyIterator& it)
{
    if (current_node == nullptr || it.current_node == nullptr) {
            return current_node != it.current_node; // handles case where one node is nullptr
        }
    return current_node != it.current_node;
    }
 

Matrix::MyIterator& Matrix::MyIterator::operator=(const MyIterator& it)
{
    if(this != &it)
    {
        current_node = it.current_node;
        startOfMatrix = it.startOfMatrix;
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
    // Initialize new matrix with same dimensions
    num_rows = copy_m.num_rows;
    num_cols = copy_m.num_cols;
    head = new Node();

    // Copy each element from copy_m to the new matrix
    for (int i = 0; i < num_rows; i++) {
        Node* current_row = getRow(i);
        Node* copy_row = copy_m.getRow(i);

        MyIterator it(current_row, head);
        MyIterator copy_it(copy_row, copy_m.head); //create two iterators to copy them with a loop
        for (int j = 0; j < num_cols; j++) {
            *it = *copy_it;
            it++;
            copy_it++;
        }
    }
}


Matrix& Matrix::operator=(const Matrix& obj) //assignment operator
{
     if (this == &obj) // self-assignment check
     {
         return *this;
     }
     
    clear();
     
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
         Node* new_head = temp;
         for (int j = 0; j < num_rows; j++)
         {
             new_head->next_row = new Node(obj_temp->value);
             new_head = new_head->next_row;
             obj_temp = obj_temp->next_row;
         }
         temp = temp->next_col;
     }
     
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

Node* Matrix::getRow(int row_num) const //assuming the index start at 0//
{
    if(row_num >= num_rows)
        throw runtime_error("Row number cannot exceed the number of rows!");

    Node* temp = head;//does the row and column start at 0 or 1
    for(int i = 0 ; i < row_num ; i++)
    {
        temp = temp->next_row;
    }
    
    return temp;
}

Node* Matrix::getCol(int col_num) const
{
    if(col_num >= num_cols)
    {
        throw runtime_error("Column number cannot exceed the number of columns!");
    }
    
    Node* temp = head;
    for(int i = 0 ; i < col_num ; i++)
    {
        temp = temp->next_col;
    }
    
    return temp;
}

Matrix& Matrix::transpose()
{
    static Matrix trans; //a matrix to be returned by reference
    Node* ptr_to_head = trans.head; //a pointer to the node of the return matrix
    trans.num_rows = this->num_cols; //swap the rows and cols of the input parameter
    trans.num_cols = this->num_rows;
    
    for(int i = 0 ; i < this->num_rows ; i++)
    {
        Node* current_row = this->getRow(i); //get the row
        Node* current_node = current_row; //now hold the node to traverse through it
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
 
ostream& operator<<(ostream& output, Matrix& obj)
{
    Matrix::MyIterator it(obj.head, obj.head->next_row);
    
    for(int i = 0; i < obj.num_rows; i++)
    {
        for(int j = 0; j < obj.num_cols; j++, it++)
        {
            output << *it << " ";
        }
        output << "\n"; 
    }
    return output;
}

