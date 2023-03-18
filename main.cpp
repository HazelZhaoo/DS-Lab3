#include <iostream>
#include <iterator>
#include "Matrix.hpp"
using namespace std;


int main()
{
    int rows = 3, cols = 4;
        int** arr = new int*[cols];
        for (int i = 0; i < cols; i++) {
            arr[i] = new int[rows];
            for (int j = 0; j < rows; j++) {
                arr[i][j] = i * rows ; // fill array with some values
            }
        }
    for(int i = 0 ; i < cols ; i++)
    {
        for(int j = 0 ; j < rows ; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    

    Matrix mat(arr, rows, cols);
    Matrix copy_cat(mat);
     
 
}
