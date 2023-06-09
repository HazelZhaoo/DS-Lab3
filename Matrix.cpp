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
     if (this == &obj) // self-assignment chec k
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
    while (traverse_ptr != nullptr) // check if current node is nullptr
    {
        Node* delete_row = traverse_ptr;
        traverse_ptr = traverse_ptr->next_col; //go column wise
        while (delete_row != nullptr) //loop row wise with the inner loop
        {
            Node* delete_node = delete_row;
            delete_row = delete_row->next_row;
            delete delete_node;
        }
    }
    head = nullptr; // reset head pointer to nullptr
}


int* Matrix::getRow(int row_num) const {
    if (row_num < 0 || row_num >= num_rows) {
        throw runtime_error("Invalid row number.");
    }
    
    Node* current = head;
    for (int i = 0; i < row_num; i++) {
        current = current->next_row;
    }
    
    int* row_values = new int[num_cols];
    for (int i = 0; i < num_cols; i++) {
        row_values[i] = current->value;
        current = current->next_col;
    }
    
    return row_values;
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
    int col = this->num_rows;
    int row = this->num_cols;
    int** current_row = new int*[row];
    int trans_array[row][MAX_COL_SIZE]; // create a 2D array for the transpose of 4 row and 10 column
    
    int i = 0;
    while( i < this->num_rows)
    {
        current_row[i] = this->getRow(i); // get the current row
        for (int j = 0; j < this->num_cols; j++)
        {
            trans_array[j][i] = current_row[i][j]; // assign the values to the transpose array
        }
        i++;
    }
    
    for (int i = 0; i < row; i++)
    {
        delete[] current_row[i]; // free the memory allocated for each row
    }
    delete[] current_row; // free the memory allocated for the array of pointers
    
    static Matrix trans(trans_array, row, col); // create the transpose matrix using the constructor
    return trans;
}

 
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

