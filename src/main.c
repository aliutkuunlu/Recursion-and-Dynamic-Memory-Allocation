#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void Read_File(char* s, char** matrix);/*Function for reading .txt files*/
void split(char** matrix, int** result, int lineCount);/*Function for delete whitespaces in the inputfiles*/
void search(int** map, int** key, int* rowIndex, int* columnIndex, int rowSize, int columnSize,
            int keyMapSize, int shift, FILE* output);/*Function for search the treasure in the map*/
void goUp(int* rowIndex, int keyMapSize);/*Function for going up on the map*/
void goDown(int* rowIndex, int keyMapSize, int rowSize, int shift);/*Function for going down on the map*/
void goLeft(int* columnIndex, int keyMapSize);/*Function for going left on the map*/
void goRight(int* columnIndex, int keyMapSize, int columnSize, int shift);/*Function for going right on the map*/

int main(int argc, char* argv[])
{

    FILE* output = fopen(argv[5], "w");/*output file*/
    if (output == NULL)
    {
        printf("Error opening file!\n");
        exit(1);
    }
    int i, j ,k, l, m, n;
    int cont =0;
    l=0;
    int rowIndex ;/*Starting row index*/
    int columnIndex; /*Starting row index*/
    int rowSize, columnSize; /*variables for map size*/
    int strLen = strlen(argv[1]);/*length of first argument*/
    char *firstInput = (char*)malloc(strLen*(sizeof(char)));/* Creating array for collect size of mapmatrix*/

    for (k=0; k<strLen;k++)
    {
        firstInput[k]= argv[1][k];
    }
    char** sizes;
    sizes = (char**)malloc(2* sizeof(char*));
    sizes[0] = (char*)malloc(((strLen-1)/2)* sizeof(char));/* space for row size of map matrix*/
    sizes[1] = (char*)malloc(((strLen-1)/2)* sizeof(char));/* space for column size of map matrix*/
    char* token = strtok(firstInput, "x|X");
    while (token != NULL)
    {
        if(token != "")
            strcpy(sizes[l],token);
        l++;
        cont++;
        token = strtok(NULL, "x|X");
    }
    rowSize = atoi(sizes[0]);/*row size of map matrix*/
    columnSize = atoi(sizes[1]);/*column size of map matrix*/

/*
 * In this part sizes of map are determined
 */
/*---------------------------------------------------------------------------------------------*/

    int keyMapSize = atoi(argv[2]);/*size of key map*/
    int shift = (keyMapSize-1)/2;/*shift amount on the map while searching*/
    rowIndex=shift;/*starting row index*/
    columnIndex=shift;/*starting column index*/
    if(columnSize%keyMapSize!=0)
    {
        /*
         * If column size of mapmatrix not divisible by keymatrix size
         * that might be a problem and exiting the program.
         */
        fprintf(output, "Sizes are not proper.\n" );
        return 0;
    }
    if(rowSize%keyMapSize!=0)
    {
        /*
         * If row size of mapmatrix not divisible by keymatrix size
         * that might be a problem and exiting the program.
         */
        fprintf(output, "Sizes are not proper.\n" );
        return 0;
    }
    char** tempTreasureMap;/* 2D map with whitespaces*/
    tempTreasureMap = (char**) malloc(rowSize* sizeof(char*));
    for(i = 0; i < rowSize; i++)
    {
        tempTreasureMap[i] = (char*)malloc(columnSize*2* sizeof(char));
    }

    char** tempKeyMap;/* 2D keymap with whitespaces*/
    tempKeyMap = (char**) malloc(keyMapSize* sizeof(char*));
    for(j = 0; j < keyMapSize; j++)
    {
        tempKeyMap[j]= (char*)malloc(keyMapSize*2* sizeof(char));
    }

    Read_File(argv[4], tempKeyMap); /*Reading forth argument*/
    Read_File(argv[3], tempTreasureMap);/*Reading third argument*/


    int** map;/*2D map */
    map = (int**)malloc(rowSize* sizeof(int*));
    for(m=0;m<rowSize;m++)
    {
        map[m] = (int*)malloc(columnSize* sizeof(int));
    }

    int** key;/*2D key map */
    key = (int**)malloc(keyMapSize* sizeof(int*));
    for(n=0;n<keyMapSize;n++)
    {
        key[n] = (int*)malloc(keyMapSize* sizeof(int));
    }

    split(tempTreasureMap, map, rowSize);/*Creating 2D int array for map*/
    split(tempKeyMap, key, keyMapSize);/*Creating 2D int array for key map*/


    /*
     * This function searches the treasure recursively and when it found, it will return.
     */
    search(map, key, &rowIndex, &columnIndex, rowSize, columnSize, keyMapSize, shift, output);

    fclose(output);/*closing output file.*/
    free(key);/*free the space for key*/
    free(map);/*free the space for map*/
    free(sizes);/*free the space for sizes*/
    free(tempTreasureMap);/*free the space for tempTreasureMap*/
    free(tempKeyMap);/*free the space for tempKeyMap*/
    return 0;
}

void search(int** map, int** key, int* rowIndex, int* columnIndex, int rowSize, int columnSize,
            int keyMapSize, int shift, FILE* output)
{
    int mod;
    int result=0;/*Result of multiplication*/
    int i, j;/*indexes for treasure map*/
    int keyRow=0;/*row index for key*/
    int keyCol=0;/*column index for key*/
    for( i = *rowIndex - shift ; i < *rowIndex + shift + 1 ; i++)
    {
        for ( j = *columnIndex - shift ; j < *columnIndex + shift + 1 ; j++)
        {
            result += (map[i][j] * key[keyRow][keyCol]);
            keyCol++;/*Key's column index increased*/
        }
        keyRow++;/*Key's row index increased*/
        keyCol=0;/*Key's column index set to zero for next iteration*/
    }
    mod = (result%5);/*mod result for determine the searching way*/

    if(mod==1)
    {
        /*
         * For find the treasure, treasure hunter must go up if there is a way.
         */
        fprintf(output,"%d,%d:%d\n", *rowIndex, *columnIndex, result);
        goUp(rowIndex, keyMapSize);
    }
    else if(mod==2)
    {
        /*
         * For find the treasure, treasure hunter must go down if there is a way.
         */
        fprintf(output,"%d,%d:%d\n", *rowIndex, *columnIndex, result);
        goDown(rowIndex, keyMapSize, rowSize, shift);
    }
    else if(mod==3)
    {
        /*
         * For find the treasure, treasure hunter must go right if there is a way.
         */
        fprintf(output,"%d,%d:%d\n", *rowIndex, *columnIndex, result);
        goRight(columnIndex, keyMapSize, columnSize, shift);
    }
    else if(mod==4)
    {
        /*
         * For find the treasure, treasure hunter must go left if there is a way.
         */
        fprintf(output,"%d,%d:%d\n", *rowIndex, *columnIndex, result);
        goLeft(columnIndex, keyMapSize);
    }
    else if(mod==0)
    {
        /*
         * The treasure is found.
         */
        fprintf(output,"%d,%d:%d", *rowIndex, *columnIndex, result);
        return;
    }
    else
    {
        fprintf(output,"\n\nERROR\n\n");
    }
    /*
     * recursive function calling for next step
     */
    search(map, key, rowIndex, columnIndex, rowSize, columnSize, keyMapSize, shift, output);

}
void goUp(int* rowIndex, int keyMapSize)
{
    if(*rowIndex-keyMapSize <1)
    {
        *rowIndex+=keyMapSize;  /* No place to go... Going Down*/
    }
    else
    {
        *rowIndex -=keyMapSize;  /* Going Up*/
    }

}
void goDown(int* rowIndex, int keyMapSize, int rowSize, int shift)
{
    if(*rowIndex+keyMapSize >( rowSize - (shift +1 ) ))
    {
        *rowIndex -= keyMapSize; /*No place to go... Going Up*/
    }
    else
    {
        *rowIndex+=keyMapSize; /* Going Dwon*/
    }
}
void goLeft(int* columnIndex, int keyMapSize)
{
    if(*columnIndex-keyMapSize <1)
    {
        *columnIndex += keyMapSize; /*No place to go... Going Right*/
    }
    else
    {
        *columnIndex -= keyMapSize; /* Going Left*/
    }
}
void goRight(int* columnIndex, int keyMapSize, int columnSize, int shift)
{
    if(*columnIndex+keyMapSize > (columnSize -(shift +1)))
    {
        *columnIndex -= keyMapSize; /*No place to go... Going left*/
    }
    else
    {
        *columnIndex += keyMapSize; /* Going Right*/
    }
}
void Read_File(char* s, char** matrix)
{
    /*
     * This function reads the arguments and by using these datas create a 2D char array
     */

    int i = 0;
    FILE* input_file;
    char buff[1000];
    input_file = fopen(s, "r");

    while(fgets(buff, 1000, input_file) != NULL){
        strcpy(matrix[i], buff);
        i = i+1;
    }
    fclose(input_file);

}

void split(char** matrix,int** result, int lineCount)
{
    /*
     * This function takes 2D char array with whitespaces and return that matrix to
     * 2D int array without whitespaces.
     */
    int i, j;
    for (i = 0; i <lineCount ; i++)
    {
        j = 0;
        char* token = strtok(matrix[i], " ");

        while(token!= NULL)
        {

            if(token != "")
            {
                result[i][j] = atoi(token);
            }
            j++;
            token = strtok(NULL, " ");

        }
    }


}