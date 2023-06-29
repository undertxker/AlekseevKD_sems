#include "csv.h"
#include <stdlib.h>
#include <errno.h>

// Определение констант
#define END_OF_LINE '\0'  // Конец строки
#define SEPARATOR ';'     // Разделитель
#define QUOTES '"'        // Кавычки для обрамления записей

// Функция для определения символа новой строки
int is_new_line(char c)
{
  if (c == '\r' || c == '\n')
    return 1;

  return 0;
}

// Функция для чтения следующей записи в CSV-файле
dynamic_string csv_read_next(char** input)
{
  dynamic_string output;
  dynamic_string_init(&output);
  char* i = *input;

  if (i[0] == END_OF_LINE || is_new_line(i[0]))
    return output;

  if (i[0] == QUOTES)
  {
    i++;

    while (i[0] != END_OF_LINE)
    {
      if (i[0] == QUOTES)
      {
        if (i[1] == QUOTES)
        {
          dynamic_string_push_back(&output, QUOTES);
          i += 2;
        }
        else
        {
          i++;
          break;
        }
      }
      else
      {
        dynamic_string_push_back(&output, i[0]);
        i++;
      }
    }
  }
  else
  {
    while (i[0] != END_OF_LINE && !is_new_line(i[0]) && i[0] != SEPARATOR)
    {
      dynamic_string_push_back(&output, i[0]);
      i++;
    }
  }

  if (i[0] == SEPARATOR)
    i++;

  (*input) = i;
  return output;
}

// Функция для записи записи в CSV-файл
int csv_write_entry(FILE* file, dynamic_string* s)
{
  int escape = 0;

  for (size_t i = 0; i < s->length; ++i)
  {
    if (s->buffer[i] == SEPARATOR || s->buffer[i] == QUOTES)
    {
      escape = 1;
      break;
    }
  }

  if (!escape)
  {
    size_t written = fwrite(s->buffer, sizeof(char), s->length, file);
    if (written != s->length)
      return errno;
    return 0;
  }

  if (putc('"', file) == EOF)
    return errno;

  for (size_t i = 0; i < s->length; ++i)
  {
    if (putc(s->buffer[i], file) == EOF)
      return errno;

    if (s->buffer[i] == QUOTES)
    {
      if (putc('"', file) == EOF)
        return errno;
    }
  }

  if (putc('"', file) == EOF)
    return errno;

  return 0;
}

// Функция для записи разделителя в CSV-файл
int csv_write_separator(FILE* file)
{
  if (putc(SEPARATOR, file) == EOF)
    return errno;

  return 0;
}

// Функция для записи новой строки в CSV-файл
int csv_write_new_line(FILE* file)
{
  if (putc('\r', file) == EOF)
    return errno;
  if (putc('\n', file) == EOF)
    return errno;
  
  return 0;
}
