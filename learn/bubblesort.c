#include <stdio.h>
#include <unistd.h>


int main( int argc, char *argv[] )
{
  int arr[10] = { 1, 100, 80, 5, 7, 1, 7, 9, 6,5 };
  int i, j, temp;
  
  for( i = i; i < 9 ; i++ )
    for( j = 0; j < 9 ; j++ )
     if( arr[j] > arr[j+1] )
       {
         temp = arr[j+1];
         arr[j+1] = arr[j];
         arr[j] = temp;
       }


  for( i=0; i<10; i++ )
    printf( "%d ", arr[i] );
    
  return 0;
}
