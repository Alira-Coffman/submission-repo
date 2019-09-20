
#include <stdio.h>

int main()
{
    int sixteenbit = 522;
    int sixteen2bit = 500;
    int result = 0; 
    for(int i =1; i <= sixteen2bit; i++)
    {
        result += sixteenbit;
    }
    
    printf("Result is %d", result);

    return;
}