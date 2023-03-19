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

Matrix::Matrix(int** array , int row_size , int col_size)
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
    
    MyIterator it(head , head->next_row );
    for(int i = 0 ; i < num_cols ; i++)
    {
        for(int j = 0 ; j < num_rows; j++)
        {
            *it = array[i][j];
        }
        ++it;
    }
}
    
Matrix::~Matrix()
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

Matrix:: Matrix(const Matrix& copy_m)
{
    //copied from parameterized constructor//---------------
    head = new Node();
    Node* current_col = head;
    num_rows = copy_m.num_rows;
    num_cols = copy_m.num_cols;
    for(int i = 0 ; i < num_cols ; i++)
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
    MyIterator copy_it(copy_m.head , copy_m.head->next_row);
    MyIterator calling_obj(head , nullptr);
    int count = 0; // keep track of the number of elements copied
    while (count < num_rows*num_cols)
    {
        *calling_obj = *copy_it;
        ++calling_obj;
        ++copy_it;
        ++count;
    }
}

Matrix& Matrix::operator=(const Matrix& obj)
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
    
