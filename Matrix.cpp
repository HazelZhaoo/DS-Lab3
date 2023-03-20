#include "Matrix.hpp"
#include <iterator>

Matrix::MyIterator::MyIterator(Node* some_node , Node* some_head):current_node(some_node) , startOfMatrix(some_head){}

Matrix::MyIterator& Matrix::MyIterator::operator++() //prefix
{
  if(current_node != nullptr)
  {
      current_node = current_node->next_col;
  }
  else
  {
      current_node = startOfMatrix->next_row; //its the head of the matrix
      startOfMatrix = startOfMatrix->next_row;
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
 
Matrix::Matrix():head(nullptr) , num_rows(0) , num_cols(0){};

Matrix::Matrix(int** array , int row_size , int col_size) //parametreized constructor
{
    head = new Node();
    Node* current_col = head;
    num_rows = row_size ;
    num_cols = col_size;
    for(int i = 0 ; i < num_cols ; i++) //creating the nodes horizontally based on the num of cols
    {
        current_col->next_col = new Node();
        current_col = current_col ->next_col;
    }
    
    Node* temp = head; //a temporary pointer to head
    for(int i = 0 ; i< num_cols; i++) //go back to head node and create node vertically
    {
        Node* new_head = head; //a pointer to head to loop vertically
        for(int j = 0 ; j < num_rows ; j++)
        {
            new_head->next_row = new Node(); //create new nodes along the column
            new_head = new_head->next_row; //shift pointer along the column
        }
        temp = temp->next_col; //shift pointer along the row
    }
    
    MyIterator it(head , head->next_row);
    for(int i = 0 ; i < num_cols ; i++)
    {
        for(int j = 0 ; j < num_rows; j++)
        {
            *it = array[i][j];
        }
        ++it;
    }
}
    
Matrix::~Matrix() //destructor
{
    Node* temp = head; //set a temporary pointer
    while(temp != nullptr)
    {
        Node* delete_row = head; //assign the first row as the now to be delete
        while(delete_row != nullptr) //delete nodes horizontally
        {
            Node* delete_node = delete_row; // hold the first node
            delete_row = delete_row->next_col;
            delete delete_node; //delete that node
        }
        temp = temp->next_row; // go to the next row and do the same thing
        
    }
}

// Copy constructor
Matrix::Matrix(const Matrix& copy_m)
{
    // Initialize new matrix with same dimensions
    num_rows = copy_m.num_rows;
    num_cols = copy_m.num_cols;
    head = new Node();

    
    Node* current_col = head;// Create nodes along horizontally
    for (int i = 0; i < num_cols; i++) {
        current_col->next_col = new Node();
        current_col = current_col->next_col;
    }

   
    Node* temp = head; // Create nodes vertically
    for (int i = 0; i < num_cols; i++) {
        Node* new_head = head;
        for (int j = 0; j < num_rows; j++) {
            new_head->next_row = new Node();
            new_head = new_head->next_row;
        }
        temp = temp->next_col;
    }

    // Copy elements from original matrix to new matrix using iterators
    MyIterator copy_it(copy_m.head, copy_m.head->next_row);
    MyIterator calling_obj(head, nullptr);
    int count = 0;
    while (count < num_rows*num_cols) {
        *calling_obj = *copy_it;
        ++calling_obj;
        ++copy_it;
        ++count;
    }
}

Matrix& Matrix::operator=(const Matrix& obj) //assignment operator
{
     if (this == &obj) // self-assignment check
     {
         return *this;
     }
     
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
     
     // Copy the other matrix
     head = new Node();
     Node* current_col = head;
     num_rows = obj.num_rows;
     num_cols = obj.num_cols;
     for (int i = 0; i < num_cols; i++)
     {
         current_col->next_col = new Node();
         current_col = current_col->next_col;
     }
     
     Node* temp = head;
     for (int i = 0; i < num_cols; i++)
     {
         Node* new_head = head;
         for (int j = 0; j < num_rows; j++)
         {
             new_head->next_row = new Node();
             new_head = new_head->next_row;
         }
         temp = temp->next_col;
     }
     
     MyIterator copy_it(obj.head, obj.head->next_row);
     MyIterator calling_obj(head, nullptr);
     int count = 0;
     while (count < num_rows*num_cols)
     {
         *calling_obj = *copy_it;
         ++calling_obj;
         ++copy_it;
         ++count;
     }
     
     return *this;
}
    
Node* Matrix::getRow(int row_num) //assuming the index start at 0//
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

Node* Matrix::getCol(int col_num)
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
    int i = 0 , j = 0;
    MyIterator it(this->head , (this->head)->next_row);
    while(j != this->num_rows) //loop row wise and assign column wise
    {
        while(i != this->num_cols) //loop column wise and assign row wise
        {
            ptr_to_head->value = *it; //assignt the value of the iterator to the new matrix//
            ptr_to_head ->next_row = new Node(); //create new node on the next row//
            ptr_to_head = ptr_to_head->next_row; //traverse row wise
            ++it; //traverse column wise
            i++;
        }
        j++; //go to the next row
    }
    
    return trans;
}
