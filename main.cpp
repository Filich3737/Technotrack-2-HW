/**
  *@file
  * @author Philip Khristolyubov
  * @version 1.0
  * @date 04.10.2018
  * @brief Второе домашнее задание по курсу "Введение в промышленное программирование и структуры данных"
  * @todo
  */

#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <clocale>
#include <string.h>
#include <wchar.h>
//------------------------------------------------------------------------------------------------

char *scanFile(char *filename, int *nStringsPtr);
char **strs_from_txt(char* input_txt, int n);
char **sortText(char *input_txt, char **strs, int n, int (*comparator)(const void *, const void *));
int addToFile(char *filename, char *txt, char **strs, int nStringsPtr);
int compareStr(const void *str1, const void *str2);
int compareStrRev(const void *val1, const void *val2);
int numFromLetter(char *str, int i);
int nextLetter(char *str, int *i);

int str1_i = 0, str2_i = 0;

/*void printstr(char* str)
{
    for(int i = 0; str[i] != '\0'; i++)
    {
        int res = numFromLetter(str, i);
        printf("%d ", res);
        if(res)
            i++;
    }
    puts("");
}*/

int main(int argv, char* argc[])
{
    printf("# PoemSort v1.0 (c) Philip Khristolyubov\n");
    if (argv < 2)
    {
        printf("# Please, launch this programm with the name of file to sort as first argument\n");
        return -1;
    }
    assert(argc[1] != nullptr);

    int nStrings;
    char *text = scanFile(argc[1], &nStrings);
    assert(text != nullptr);
    assert(nStrings > 0);

    char **strings = strs_from_txt(text, nStrings);
    assert(strings != nullptr);

    char **strings_sorted = sortText(text, strings, nStrings, compareStr);
    assert(strings_sorted != nullptr);

    char **strings_sorted_reverse = sortText(text, strings, nStrings, compareStrRev);
    assert(strings_sorted_reverse != nullptr);

    for(int i = 0; i < nStrings; i++)
    {
        int j = 0;
        while(strings_sorted_reverse[i][j] == 0x9)
            j++;
        if (numFromLetter(&strings_sorted_reverse[i][j], 0) > 0)
            puts(&strings_sorted_reverse[i][j]);
    }

    //assert(addToFile(argc[1], text, strings_sorted, nStrings) == 0);

    return 0;
}

char *scanFile(char *filename, int *nStringsPtr)
{
    assert(filename != nullptr);
    assert(nStringsPtr != nullptr);
    FILE * ptrFile = fopen(filename, "r");
    if (ptrFile == nullptr)
    {
        printf("# Can't open file with name %s\n", filename);
        perror("# Error: ");
        exit(-1);
    }

    fseek(ptrFile, 0, SEEK_END);                            // устанавливаем позицию в конец файла
    long lSize = ftell(ptrFile);                            // получаем размер в байтах
    rewind (ptrFile);                                       // устанавливаем указатель в конец файла

    char* buffer = (char*) calloc(lSize, 1);          // выделить память для хранения содержимого файла
    assert(buffer != nullptr);

    size_t result = fread(buffer, 1, lSize, ptrFile);       // считываем файл в буфер
    assert(!ferror(ptrFile));
    assert(result == lSize);

    assert(fclose (ptrFile) != EOF);

    *nStringsPtr = 0;
    for(int i = 0; i < lSize; i++)
        if (buffer[i] == '\n')
            (*nStringsPtr)++;
    return buffer;
}

char **strs_from_txt(char *input_txt, int n)
{
    assert(input_txt != nullptr);
    assert(n > 0);
    char **buf = (char**) calloc(n+1, sizeof(char*));
    assert(buf != nullptr);
    buf[0] = input_txt;
    int i = 1;
    for (char *s = strchr(input_txt, '\n'); s; s = strchr(s+1, '\n'))
    {
        buf[i] = s+1;
        *s = '\0';
        i++;
    }
    buf[i-1] = nullptr;
    return buf;
}

char **sortText(char *input_txt, char **strs, int n, int ( * comparator ) ( const void *, const void * ))
{
    assert(input_txt != nullptr); assert(strs != nullptr);
    assert(n > 0);

    char **sorted_strs = (char**) calloc(n+1, sizeof(char*));
    assert(sorted_strs != nullptr);
    memcpy(sorted_strs, strs, (n+1)*sizeof(char*));

    qsort(sorted_strs, n, sizeof(char*), comparator);
    return sorted_strs;
}

int compareStr(const void *val1, const void *val2)
{
    assert(val1 != nullptr); assert(val2 != nullptr);
    char *str1 = *(char**)val1;
    char *str2 = *(char**)val2;

    int res = 0;
    str1_i = 0;
    str2_i = 0;
    do
    {
        int n1 = nextLetter(str1, &str1_i);
        int n2 = nextLetter(str2, &str2_i);
        assert(n1 >= -1);
        assert(n2 >= -1);
        if ((n1 == -1) && (n2 != -1))
            return -1;
        if ((n2 == -1) && (n1 != -1))
            return 1;
        if ((n2 == -1) && (n1 == -1))
            return 0;
        int res = n1 - n2;
        if (res != 0)
            return res;
    }
    while (res == 0);
    return 0;
}

int compareStrRev(const void *val1, const void *val2)
{
    return -1;
}

int numFromLetter(char *str, int i)
{
    assert(str != nullptr);
    assert(i >= 0);
    if (str[i] == 0xffffffd0)
    {
        if ( (str[i+1] >= 0xffffff90) && (str[i+1] <= 0xffffff95) )
            return str[i+1] - 0xffffff90 + 1;
        else if (str[i+1] == 0xffffff81)
            return 7;
        else if ( (str[i+1] >= 0xffffff96) && (str[i+1] <= 0xffffffaf) )
            return str[i+1] - 0xffffff90 + 2;
        else if ( (str[i+1] >= 0xffffffb0) && (str[i+1] <= 0xffffffb5) )
            return str[i+1] - 0xffffffb0 + 1;
        else if ( (str[i+1] >= 0xffffffb6) && (str[i+1] <= 0xffffffbf) )
            return str[i+1] - 0xffffffb0 + 2;
        else return -2;

    }
    else if (str[i] == 0xffffffd1)
    {
        if ( (str[i+1] >= 0xffffff80) && (str[i+1] <= 0xffffff8f) )
            return str[i+1] - 0xffffff80 + 18;
        else if (str[i+1] == 0xffffff91)
            return 7;
        else return -2;
    }
    else if (str[i] == '\0')
        return -1;
    else
        return 0;
}

int nextLetter(char *str, int *i)
{
    int x = 0;
    do
    {
        x = numFromLetter(str, *i);
        assert(x >= -2);
        if (x == 0)
            *i += 1;
        else if (x > 0)
            *i += 2;
        else if (x == -1)
            return -1;
        else /* if(x == -2) */
            *i += 2;
    }
    while(x <= 0);
    return x;
}

int addToFile(char *filename, char *txt, char **strs, int nStringsPtr)
{
    assert(filename != nullptr);
    assert(txt != nullptr);
    assert(strs != nullptr);
    assert(nStringsPtr > 0);
    FILE * ptrFile = fopen(filename, "a");
    if (ptrFile == nullptr)
    {
        printf("# Can't open file with name %s\n", filename);
        perror("# Error: ");
        exit(-1);
    }

    for(int i = 0; i < nStringsPtr; i++)
    {
        int j = 0;
        while(strs[i][j] == 0x9)
            j++;
        if (numFromLetter(&strs[i][j], 0) > 0)
            fputs(&strs[i][j], ptrFile);
    }
    assert(fclose (ptrFile) != EOF);
    return 0;
}

