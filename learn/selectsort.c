#include <stdio.h>
#include <unistd.h>


int main( int argc, char *argv[] )
{
  int arr[10] = { 1, 100, 80, 5, 7, 1, 7, 9, 6,5 };
  int i, j, temp;
  
  for( i = 0; i < 9 ; i++ )
    for( j = i+1; j < 10 ; j++ )
     if( arr[i] > arr[j] )
       {
         temp = arr[j];
         arr[j] = arr[i];
         arr[i] = temp;
       }


  for( i=0; i<10; i++ )
    printf( "%d ", arr[i] );
    
  return 0;
}
