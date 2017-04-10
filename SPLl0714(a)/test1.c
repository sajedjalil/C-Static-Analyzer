#include <stdio.h>

int summation(int num1, int num2)
{

    int summation;
    summation = num1 + num2;
    /*
    Returns the sum*/
    return  summation;
}

int main (void) {
    int i;

    for( i=0; i<10; i++)  printf("//Value of i is %d\n", i);

    //Prints the sum of 2 and 3
    printf("Sum: %d\n", summation(2,3)); //Output 5

    return 0;
}
