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
         
    for(int i = 0 ; i < num_rows - 1 ; i++) //creating the nodes horizontally based on the num of cols
    {
        current_col->value = array[i][0];
        current_col->next_row = new Node();
        current_col = current_col ->next_row;
    }

    current_col->value = array[num_rows - 1][0]; // handle the last column separately

    Node* temp = head; //a temporary pointer to head
    for(int i = 0 ; i< num_rows; i++) //go back to head node and create node vertically
    {
        Node* new_head = temp; //a pointer to head to loop vertically
        for(int j = 1 ; j < num_cols ; j++)
        {
            new_head->next_col = new Node(array[i][j]); //create new nodes along the column
            new_head = new_head->next_col; //shift pointer along the column
        }
        temp = temp->next_row; //shift pointer along the row
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

//Matrix& Matrix::transpose()
//{
//    static Matrix trans; //a matrix to be returned by reference
//    Node* ptr_to_head = trans.head; //a pointer to the node of the return matrix
//    trans.num_rows = this->num_cols; //swap the rows and cols of the input parameter
//    trans.num_cols = this->num_rows;
//
//    for(int i = 0 ; i < this->num_rows ; i++)
//    {
//        int* current_row = this->getRow(i); //get the row
//        Node* row_head = ptr_to_head;
//
//        for(int j = 0 ; j < this->num_cols ; j++)
//        {
//            row_head->value = *(current_row+i); //assign value to the node
//            row_head->next_row = new Node(); //create node for the next row
//            row_head = row_head->next_row; //move pointer to the next row
//        }
//
//        delete [] current_row;
//        ptr_to_head = ptr_to_head->next_col; //move pointer to the next column
//    }
//
//    return trans;
//}
//getcolumn using get col and then insert each col into the trans array so that its 2d array and then pass it to the construcotr//but getCol is returning dynamic array so maybe put it into static array//


Matrix& Matrix::transpose()
{
    int col = this->num_rows;
    int row = this->num_cols;
    int** current_row = new int*[row];
    int trans_array[row][MAX_COL_SIZE]; // create a 2D array for the transpose of 4 row and 10 column
    
    for (int i = 0; i < this->num_rows; i++)
    {
        current_row[i] = this->getRow(i); // get the current row
        for (int j = 0; j < this->num_cols; j++)
        {
            trans_array[j][i] = current_row[i][j]; // assign the values to the transpose array
        }
    }
    
    for (int i = 0; i < row; i++)
    {
        delete[] current_row[i]; // free the memory allocated for each row
    }
    delete[] current_row; // free the memory allocated for the array of pointers
    
    static Matrix trans(trans_array, row, col); // create the transpose matrix using the constructor
    return trans;
}



//void Matrix::setRow(int i, int* values) {
//    int* row = getRow(i);  // get pointer to existing row
//    for (int j = 0; j < num_cols; j++) {
//        row[j] = values[j];  // copy new values into row
//    }
//    delete [] values;  // free memory allocated for values array
//}


//void Matrix::copyFrom(const Matrix& obj)
//{
//    num_rows = obj.num_rows;
//    num_cols = obj.num_cols;
//    head = new Node(obj.head->value);
//    Node* current = head;
//    MyIterator it(obj.head->next_col); // start iterator from obj.head->next_col
//    for (int i = 0; i < num_rows; i++)
//    {
//        Node* current_row = current;
//        for (int j = 0; j < num_cols+1; j++)
//        {
//            current_row->next_col = new Node(*it); // EXC_BAD_ACCESS ERROR
//            current_row = current_row->next_col;
//            it++; // move iterator to next column
//        }
//        if (i < num_rows - 1) // avoid creating an unnecessary row at the end
//        {
//            current_row->next_row = new Node(); // create new row
//            current_row = current_row->next_row;
//        }
//        current = current->next_row;
//    }
//}

void Matrix::copyFrom(const Matrix& obj)
{
    num_rows = obj.num_rows;
    num_cols = obj.num_cols;
    head = new Node(obj.head->value); // copy the head
    Node* current_row = head; // pointer to traverse through the rows
    Node* other_current_row = obj.head; // pointer to iterate through the other matrix rows

    // copy the rows
    while (other_current_row->next_row != nullptr)
    {
        current_row->next_row = new Node(other_current_row->next_row->value);
        other_current_row = other_current_row->next_row;
        current_row = current_row->next_row;
    }

    // copy the columns of each row
    current_row = head;
    other_current_row = obj.head;
    while (other_current_row != nullptr)
    {
        Node* current_col = current_row;
        Node* other_current_col = other_current_row;
        while (other_current_col->next_col != nullptr)
        {
            current_col->next_col = new Node(other_current_col->next_col->value);
            other_current_col = other_current_col->next_col;
            current_col = current_col->next_col;
        }
        current_row = current_row->next_row;
        other_current_row = other_current_row->next_row;
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

