#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <time.h>

#define matmax 2050
#define wordmax 240

struct wordbank
{
    char word[wordmax];
};

struct matrix
{
    char line[matmax];
};

int search(struct wordbank *lexis, char word[],int size);

int compare (const void * a, const void *b)
{
    struct wordbank * ia=(struct wordbank*)a;
    struct wordbank * ib=(struct wordbank*)b;
    return (strcmp(ia->word,ib->word));
}

int main (int argc, char **argv)
{
    if (argv[2] == NULL)
    {
        printf("Please Specify Minimum Word Size 'solver {path to puzzle} {minimum word size}'");
        exit(1);
    }

    FILE *puzzleptr;
    FILE *lexisptr;
    int i=0,
        j=0,
        k=0,
        l=0,
        m=0,
        lettercount=0,
        wordcount=0,
        wordsize=atoi(argv[2]),
        lexisrows=0,
        matrows=0,
        matcols=0;
    char buffer[matmax],
        tempword[wordmax],
        linechar;


    //---------------------------COPIES DATA FROM PUZZLE FILE TO MATRIX STRUCTURE-------------------------------

    if ((puzzleptr = fopen(argv[1], "r")) == NULL)
    {
        printf("Error Opening File, 'solver {path to puzzle} {minimum word size}'");
        exit(1);
    }

    //determines the amount of rows
    while (fgets(buffer, matmax, puzzleptr) != NULL)
    {
        matrows++;
    }
    //determines the amount of columns
    matcols = strlen(buffer);
    rewind(puzzleptr);

    //creates the structure instance used to store the matrix
    struct matrix *mat = malloc(matrows * sizeof(struct matrix));

    //copies the data from the puzzle file to the structure
    while (fgets(buffer, matmax, puzzleptr) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0; //removes "\n"s
        //converts all chars in the buffer to lowercase and removes all spaces
        for (j=0; j<strlen(buffer); j++)
        {
            buffer[j] = tolower(buffer[j]);
            buffer[j]=buffer[j+k];
            if(buffer[j]==' '|| buffer[j]=='\t')
            {
                k++;
                j--;
            }
        }
        strcpy(mat[i].line,buffer);
        i++;
    }
    i=k=0;
    fclose(puzzleptr);

//---------------------------COPIES DATA FROM WORD BANK FILE TO WORD BANK STRUCTURE-------------------------------

    if ((lexisptr = fopen("lexis.txt", "r")) == NULL)
    {
        printf("Error opening lexis");
        exit(1);
    }

    //determines the amount of rows
    while (fgets(buffer, wordmax, lexisptr) != NULL)
    {
        lexisrows++;
    }
    rewind(lexisptr);

    //creates the structure instance used to store the lexis
    struct wordbank *lexis = malloc(lexisrows * sizeof(struct wordbank));

    //copies data from the lexis file to the lexis structure
    while (fgets(buffer, wordmax, lexisptr) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        for (j=0; j<strlen(buffer); j++)
        {
            buffer[j] = tolower(buffer[j]);
            buffer[j]=buffer[j+k];
            if(buffer[j]==' '|| buffer[j]=='\t')
            {
                k++;
                j--;
            }
        }
        strcpy(lexis[i].word,buffer);
        i++;
    }
    i=0;
    fclose(lexisptr);

//---------------------------ACTUAL WORD SEARCH IMPLEMENTATION-------------------------------

    //sorts the lexis structure for binary search
    qsort(lexis,lexisrows, sizeof(struct wordbank), compare);

    printf("Searching For Words With %d Or More Letters, This May Take Some Time...\n\n",wordsize);

    //used to time to program
    clock_t t; 
    t = clock();

//---------------------------LEFT TO RIGHT-------------------------------
    //loops through rows
    for (i=0; i<matrows;i++)
    {
        //loops through columns
        for (j=0;j<matcols;j++)
        {
            //loops through columns and creates strings
            for(k=j; k<matcols; k++)
            {
                tempword[lettercount] = mat[i].line[k]; //assigns each character to temporary string variable
                lettercount++;
                if(strlen(tempword)>=wordsize) //checks if current string is larger than the minimum
                {
                    if((search(lexis,tempword,lexisrows)) != -1) //using the binary search 
                    {
                        printf("Left To Right → : %s\n",tempword);
                        wordcount++;
                    }
                }
            }
            //resets used values to zero
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
        }
    }

//---------------------------RIGHT TO LEFT-------------------------------
    for (i=matrows-1; i>=0;i--)
    {
        for (j=matcols-1;j>=0;j--)
        {
            for(k=j; k>=0; k--)
            {
                tempword[lettercount] = mat[i].line[k];
                lettercount++;
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Right To Left ← : %s\n",tempword);
                        wordcount++;
                    }
                }
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
        }
    }

//---------------------------TOP TO BOTTOM-------------------------------
    for (i=0; i<matcols;i++)
    {
        for (j=0;j<matrows;j++)
        {
            for(k=j; k<matrows; k++)
            {
                tempword[lettercount] = mat[k].line[i];
                lettercount++;
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Top To Bottom ↓ : %s\n",tempword);
                        wordcount++;
                    }
                }
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
        }
    }

//---------------------------BOTTOM TO TOP-------------------------------
    for (i=matcols-1; i>=0;i--)
    {
        for (j=matrows-1;j>=0;j--)
        {
            for(k=j; k>=0; k--)
            {
                tempword[lettercount] = mat[k].line[i];
                lettercount++;
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Bottom To Top ↑ : %s\n",tempword);
                        wordcount++;
                    }
                }
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
        }
    }

//---------------------------DIAGONAL UP TO RIGHT-------------------------------
    //first half
    for(k=0;k<matrows;k++)
    {
        j=0;
        for(i=k;i>=0;i--)
        {
            m=j;
            for(l=i;l>=0;l--)
            {
                tempword[lettercount] = mat[l].line[m];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Up To Right ↗ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m++;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            j++;
        }
    }
    //second half
    for(k=0;k<matcols;k++)
    {
        i=matrows-1;
        for(j=k;j<matrows;j++)
        {
            m=i;
            for(l=j;l<matrows;l++)
            {
                tempword[lettercount] = mat[m].line[l];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Up To Right ↗ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m--;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i--;
        }
    }

//---------------------------DIAGONAL DOWN TO LEFT-------------------------------
    //first half
    for(k=0;k<matrows;k++)
    {
        j=0;
        for(i=k;i>=0;i--)
        {
            m=j;
            for(l=i;l>=0;l--)
            {
                tempword[lettercount] = mat[m].line[l];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Down To Left ↙ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m++;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            j++;
        }
    }
    //second half
    for(k=0;k<matcols;k++)
    {
        i=matrows-1;
        for(j=k;j<matrows;j++)
        {
            m=i;
            for(l=j;l<matrows;l++)
            {
                tempword[lettercount] = mat[l].line[m];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Down To Left ↙ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m--;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i--;
        }
    }

//---------------------------DIAGONAL DOWN TO RIGHT-------------------------------
    //first half
    for(k=matcols;k>=0;k--)
    {
        i=0;
        for(j=k;j<matrows;j++)
        {
            m=i;
            for(l=j;l<matrows;l++)
            {
                tempword[lettercount] = mat[m].line[l];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Down To Right ↘ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m++;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i++;
        }
    }
    //second half
    for(k=0;k<=matrows;k++)
    {
        i=0;
        for(j=k;j<matrows;j++)
        {
            m=i;
            for(l=j;l<matrows;l++)
            {
                tempword[lettercount] = mat[l].line[m];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Down To Right ↘ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m++;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i++;
        }
    }

//---------------------------DIAGONAL UP TO LEFT-------------------------------
    //first half
    for(k=matcols;k>=0;k--)
    {
        i=matcols;
        for(j=k;j>=0;j--)
        {
            m=i;
            for(l=j;l>=0;l--)
            {
                tempword[lettercount] = mat[m].line[l];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Up To Left ↖ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m--;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i--;
        }
    }
    // //second half
    for(k=matcols;k>=0;k--)
    {
        i=matcols;
        for(j=k;j>=0;j--)
        {
            m=i;
            for(l=j;l>=0;l--)
            {
                tempword[lettercount] = mat[l].line[m];
                lettercount++; 
                if(strlen(tempword)>=wordsize)
                {
                    if((search(lexis,tempword,lexisrows)) != -1)
                    {
                        printf("Diagonal Up To Left ↖ : %s\n",tempword);
                        wordcount++;
                    }
                }
                m--;
            }
            memset(tempword, '\0', sizeof tempword);
            lettercount = 0;
            i--;
        }
    }
 


    
    printf("\n%d Words With %d Or More Letters Were Found In The (%dx%d) Matrix Located In '%s'\n", wordcount, wordsize,matrows,matcols,argv[1]);
    printf("\n");
    t = clock() - t; 
    double time_taken = ((double)t)/CLOCKS_PER_SEC; 
    printf("%f Seconds To Execute \n", time_taken);
    
}

//binart search implementation
int search(struct wordbank *lexis, char word[],int size)
{
    int low = 0;
    int high = size - 1;
    int middle=0;

    while( low <= high )
    {
        middle = (low + high) / 2;
        if (strcmp(lexis[middle].word, word) == 0)
        {
            return middle;
        }
        else if (strcmp(lexis[middle].word, word) < 0)
        {
            low=middle+1;
        }
        else
        {
            high=middle-1;
        }
    
    }
    
    return -1;
}






