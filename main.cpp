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
//------------------------------------------------------------------------------------------------

char *scanFile(char *filename, int *nStringsPtr);
char **strs_from_txt(char* input_txt, int n);
char **sortText(char *input_txt, int n);
int addToFile(char *filename, char *input_txt, char **strings_sorted);

int main(int argv, char* argc[])
{
    puts("# PoemSort v1.0 (c) Philipp Khristolyubov");
    if (argv < 2)
    {
        puts("# Please, launch this programm with the name of file to sort as first argument");
        return -1;
    }
    assert(argc[1] != nullptr);

    int nStrings;
    char *text = scanFile(argc[1], &nStrings);
    assert(text != nullptr);
    assert(nStrings > 0);
    printf("%d\n", nStrings);

    /*char **strings = strs_from_txt(text, nStrings);
    assert(strings != nullptr);

    char **strings_sorted = sortText(text, nStrings);
    assert(strings_sorted != nullptr);

    assert(addToFile(argc[1], text, strings_sorted) == 0);*/

    return 0;
}

char *scanFile(char *filename, int *nStringsPtr)
{
    assert(filename != nullptr);
    assert(nStringsPtr != nullptr);
    FILE * ptrFile = fopen(filename, "r+");
    if (ptrFile == nullptr)
    {
        printf("# Can't open file with name %s\n", filename);
        perror("# Error: ");
        exit(-1);
    }

    fseek(ptrFile, 0, SEEK_END);                            // устанавливаем позицию в конец файла
    long lSize = ftell(ptrFile);                            // получаем размер в байтах
    rewind (ptrFile);                                       // устанавливаем указатель в конец файла

    char* buffer = (char*) calloc(lSize, 1);                // выделить память для хранения содержимого файла
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
