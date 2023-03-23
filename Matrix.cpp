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
    if (current_node->next_col != nullptr) {
        current_node = current_node->next_col;
    }
    else if (current_node->next_row != nullptr) {
        current_node = current_node->next_row;
        current_node = current_node->next_col;
    }
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

Matrix::Matrix(int array[][MAX_COL_SIZE], int row_size, int col_size):num_rows(row_size), num_cols(col_size)
{
     if (row_size <= 0 || col_size <= 0 || col_size > MAX_COL_SIZE)
        throw runtime_error("Rows and columns cannot be negative and cannot exceed ten");
         
        head = new Node();
        Node* current_col = head;
         
        for(int i = 0 ; i < num_rows -1 ; i++) //creating the nodes horizontally based on the num of cols
        {
            current_col->next_row = new Node();
            current_col = current_col ->next_row;
        }

        Node* temp = head; //a temporary pointer to head
        for(int i = 0 ; i< num_rows ; i++) //go back to head node and create node vertically
        {
            Node* new_head = temp; //a pointer to head to loop vertically
            for(int j = 0 ; j < num_cols ; j++)
            {
                new_head->next_col = new Node(array[i][j]); //create new nodes along the column
                new_head = new_head->next_col; //shift pointer along the column
            }
            temp = temp->next_row; //shift pointer along the row
        }
    //head = head -> next_col;
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

Matrix::Matrix(Matrix&& obj) noexcept : head(nullptr), num_rows(0), num_cols(0) //move constructor
{

     head = obj.head;
     num_rows = obj.num_rows;
     num_cols = obj.num_cols;


     obj.head = nullptr;
     obj.num_rows = 0;
     obj.num_cols = 0;
 }

Matrix Matrix::operator=(Matrix&& obj) noexcept//move assignment operator
{
    if (this != &obj)
     {
         clear();
     }

     head = obj.head;
     num_rows = obj.num_rows;
     num_cols = obj.num_cols;

     obj.head = nullptr;
     obj.num_rows = 0;
     obj.num_cols = 0;

     return (*this);
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

   Node* traverse_ptr = head;  
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
    
    for(int i = 0 ; i < num_rows-1 ; i++)
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
            ptr_to_head->value = current_node[j]; //assign value to corresponding node
            ptr_to_head ->next_row = new Node(); //create new node on the next row//
            ptr_to_head = ptr_to_head->next_row; //traverse row wise
            current_node++; //traverse column wise
        }
        
        delete [] current_row;
    }
    return trans;
}

//void Matrix::copyFrom(const Matrix& obj)
//{
//    num_rows = obj.num_rows;
//    num_cols = obj.num_cols;
//
//    // Allocate memory for the new matrix
//    head = new Node();
//    Node* current = head;
//    for (int j = 0; j < num_cols; j++) {
//        current->next_col = new Node();
//        current = current->next_col;
//    }
//
//    // Copy the elements from the other matrix
//    Node* other = obj.head;
//    Node* current_row = head;
//    for (int i = 0; i < num_rows; i++) {
//        Node* current_col = current_row;
//        Node* other_col = other;
//        for (int j = 0; j < num_cols; j++) {
//            current_col->next_row = new Node(other_col->value);
//            current_col = current_col->next_row;
//            other_col = other_col->next_row;
//        }
//        current_row = current_row->next_col;
//        other = other->next_col;
//    }
//}

void Matrix::copyFrom(const Matrix& obj)
{
    num_rows = obj.num_rows;
    num_cols = obj.num_cols;
    head = new Node();
    Node* current = head;
    for (int i = 0; i < num_rows; i++) // create nodes row wise
    {
        current->next_row = new Node();
        current = current->next_row;
    }

    MyIterator it(obj.head);
    Node* current_node = head->next_row; // move to first row
    for (int i = 0; i < num_rows; i++) {
        Node* current_col = current_node;
        for (int j = 0; j < num_cols; j++) {
            current_col->next_col = new Node(*it);
            current_col = current_col->next_col;
            it++;
        }
        current_node = current_node->next_row; // move to next row
    }
}



//ostream& operator<<(ostream& output, const Matrix& obj) {
//    Matrix::MyIterator it(obj.head);
//
//    for (int i = 0; i < obj.num_rows; i++) {
//        Matrix::MyIterator row_it = it;
//        for (int j = 0; j < obj.num_cols; j++) {
//            output << *row_it << " ";
//            row_it++;
//        }
//        output << "\n";
//        it++;
//    }
//
//    return output;
//}


//ostream& operator<<(ostream& out , const Matrix& mat)
//{
//    Node* curr = mat.head;
//    while(curr->next_row != nullptr)
//    {
//        Node* rowhead = curr;
//       while(rowhead != nullptr)
//       {
//           out << rowhead->value << " ";
//           rowhead = rowhead->next_col;
//       }
//        out << endl;
//        curr = curr->next_row;
//    }
//    return out;
//}

ostream& operator<<(ostream& output, const Matrix& mat)
{
    Node* curr = mat.head;
    while(curr != nullptr)
    {
        Node* row_head = curr;
        while(row_head != nullptr)
        {
            output << row_head->value << " ";
            row_head = row_head->next_col;
        }
        output<< endl;
        curr = curr ->next_row;
    }

    return output;
}

