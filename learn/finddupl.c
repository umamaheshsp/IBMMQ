#include <stdio.h>
#include <unistd.h>


int main( int argc, char *argv[] )
{
  int arr[10] = { 1, 100, 80, 5, 7, 1, 7, 9, 6,5 };
  int i, j, temp;
  int Index, ChkIndex;

  for( Index = 10 - 1
     ; Index >= 0
     ; Index-- )
 {
    for( ChkIndex = 10 - 1
         ; ChkIndex > Index
         ; ChkIndex-- )
      {
            if(arr[Index] == arr[ChkIndex])
                  break;
      }
   printf( "%d\n", arr[Index] );
 }
  
  return 0;
}
