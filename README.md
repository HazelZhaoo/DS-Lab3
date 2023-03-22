# DS-Lab3

A "Matrix" is a two-dimensional data structure including values that are organized into rows and columns. Each element in the matrix is accessed using two indices: the row number and the column number. In an MxN matrix, row numbers are in the range [1, M] and column numbers are in  the range [1, N].
The "Matrix" ADT can be implemented as a sequence of nodes including the following attributes:
"value": The value of that node (i.e. the matrix element)
"nextInRow": A pointer to the next element in the same row.
"nextInColumn": A pointer to the next element in the same column.
The following example, shows how matrix "m" is represented in the memory.


Part 1:
Implement the "Matrix" ADT including:
A default constructor that creates an empty matrix. 
A parameterized constructor that creates a matrix including the values of a two-dimensional array.
A copy constructor and a copy assignment operator.
A move constructor and a move assignment operator.
A destructor.
A method that returns a specific row of the metrix. This method may throw an exception.
A method that returns a specific column of the matrix. This method may throw an exception.
A method that returns a specific element of the matrix. This method may throw an exception.
A method that returns the transpose of the matrix as a Matrix object.
Overload the addition operator to add two matrices of the same size. This method may throw an exception.
Overload the multiplication operator to multiply two matrices of compatible sizes. This method may throw an exception.
Overload the stream insertion operator to insert a matrix in the matrix format into an output stream.
Part 2:
Implement a forward iterator for the Matrix ADT that traverses the items of the matrix row by row.
Part 3:
Analyze the efficiency of your implementation and compare it with a contiguous implementation (i.e. using a two-dimensional array).
