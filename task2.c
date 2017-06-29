#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define SUCCESS 0
#define TRUE 1
#define FALSE 0
#define NONE 2
#define NO_MEMORY -3
#define INVALID_ARGUMENT -4
#define NULL_BYTE '\0'

enum lexems {
	PLUS = '+',
	MINUS = '-',
	MULT = '*',
	DIV = '/',
	OPENING_BRACE = '(',
	CLOSING_BRACE = ')',
	ZERO = '0',
	NINE = '9'
};

typedef unsigned long ulong;

int my_errno;
const char zero_number[] = {ZERO, NULL_BYTE};


int isdigit(int c);
int ungetc(int c, FILE *stream);

void print_error()
{
	fprintf(stderr, "%s\n", "[error]\n");
}

// int getint(char **_buf, size_t *_bufsize)
// {
// 	const size_t BUFLEN = 20;

// 	char *buf = NULL;
// 	size_t bufsize = 0;
// 	int code = 0;

// 	buf = (char *)malloc(BUFLEN*sizeof(char));
// 	if(buf == NULL)
// 		return NO_MEMORY;
// 	while((code = getchar()) != EOF)
// 	{
// 		if(!isdigit(code))
// 	}
// }

/*
	Убирает незначащие нули в начале строки.
	Если подают NULL на вход, то INVALID_ARGUMENT
*/

int remove_zeros(char *old_str) {
	if (old_str == NULL) {
		return INVALID_ARGUMENT;
	}
	// сдвигаем строку влево на количество нулей
	int n_zeros = 0;
	char *pointer = old_str;
	while (*(pointer + n_zeros) == ZERO) {
		n_zeros++;
	}
	// двигаем 
	int n_bytes = strlen (old_str) + 1 - n_zeros;
	memmove (old_str, pointer + n_zeros, n_bytes);
	return SUCCESS;
}

/*
На вход подаются однобайтовые строки, содержащие символы от 0 до 9, оканчивающиеся нулевым байтом. 
Строки представляют собой числа, записанные в десятичной системе счисления. Функция сложит два числа,
представленные двумя входными строками, и запишет указатель на строку-результат в выходной параметр.
Пустая строка эквивалентна числу 0. 
Если какое-либо из чисел равно NULL, то возвращает INVALID_ARGUMENT
Возвращаемое значение:
В случае успеха вернет SUCCESS, в параметре _result будет указатель на строку-результат.
Если не удалось выделить память под результат, вернет NO_MEMORY
*/

int add(const char *number1, const char *number2, char **_result)
{
	if (number1 == NULL || number2 == NULL)
		return INVALID_ARGUMENT;
	// Строка, содержащая результат вычисления
	char *number3 = NULL;

	// Длины строк
	ulong number1_size = 0, number2_size = 0, number3_size = 0;
	ulong i = 0;
	// Сначала узнаем длины операндов
	while(number1[i] != NULL_BYTE)
		i++;
	number1_size = i;
	i = 0;
	while(number2[i] != NULL_BYTE)
		i++;
	number2_size = i;
	ulong max = number1_size > number2_size ? number1_size : number2_size;
	// Размер результата это максимум из длин плюс один символ на случай переполнения
	number3_size = max + 1;
	// Размер буфера это длина строки плюс нулевой байт
	ulong bufsize = number3_size + 1;
	number3 = (char *)malloc(bufsize*sizeof(char));
	if(number3 == NULL)
	{
		*_result = NULL;
		return NO_MEMORY;
	}
	number3[bufsize-1] = NULL_BYTE;

	// Указатели на последние символы в строках
	ulong number_1_pos = number1_size > 0 ? number1_size-1 : 0;
	ulong number_2_pos = number2_size > 0 ? number2_size-1 : 0;
	ulong number_3_pos = 					number3_size-1;

	// Carry flag
	int cf = 0;

	// Результат одного отдельного вычисления
	char number1_digit = 0, number2_digit = 0, sum = 0;

	//
	for(i = 0; i < max; i++)
	{
		// Так мы дополняем число нулями спереди
		number1_digit = i < number1_size ? number1[number_1_pos - i] : ZERO;
		number2_digit = i < number2_size ? number2[number_2_pos - i] : ZERO;
		sum = (number1_digit - ZERO) + (number2_digit - ZERO) + cf;
		if(sum >= 10)
		{
			cf = 1;
			sum -= 10;
		}
		else
			cf = 0;
		number3[number_3_pos - i] = sum + ZERO;
	}
	if(cf == 1)
		number3[0] = '1';
	else
		number3[0] = '0';
	remove_zeros(number3);
	*_result = number3;
	return SUCCESS;
}

char *new_number(const char *number) {
	if (number == NULL) {
		my_errno = INVALID_ARGUMENT;
		return NULL;
	}
	char *new_n = (char *)malloc(strlen(number) + 1);
	strncpy (new_n, number, strlen(number) + 1);
	return new_n;
}
/*
	Сравнивает number1 и number2 как целые неотрицательные числа
	Если какое-либо из чисел NULL, возвращает INVALID_ARGUMENT
	Если первое больше, то TRUE, если второе, то FALSE, если равны, то NONE
*/
int compare(const char *number1, const char *number2) {
	if (number1 == NULL || number2 == NULL)
		return INVALID_ARGUMENT;
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	remove_zeros(new_n1);
	remove_zeros(new_n2);
	
	int n1_len = strlen(new_n1);
	int n2_len = strlen(new_n2);
	int n1_bigger = NONE;
	if (n1_len > n2_len)
		// если в первом числе больше цифр, то оно больше
		n1_bigger = TRUE;
	else if (n1_len < n2_len)
		n1_bigger = FALSE;
	else {
		// если одинаковое число знаков
		int compare = strcmp(new_n1, new_n2);
		if (compare == 1)
			n1_bigger = TRUE;
		else if (compare == -1)
			n1_bigger = FALSE;
		else
			n1_bigger = NONE;
	}
	free (new_n1);
	free (new_n2);
	return n1_bigger;
}

// True, если первым стоит -
int negative(const char *number) {
	if (number == NULL)
		return INVALID_ARGUMENT;
	else if (*number == MINUS)
		return TRUE;
	else
		return FALSE;
}

int negate (char **number_) {
	if (number_ == NULL) {
		return INVALID_ARGUMENT;
	}
	char *number = *number_;
	if (negative (number)) {
		*number = ZERO;
		remove_zeros (number);
	}
	else {
		char *negative_number = (char *)malloc(strlen(number) + 2);
		strncpy(negative_number+1, number, strlen(number) + 1);
		negative_number[0] = MINUS;
		free (number);
		*number_ = negative_number;
	}
	return SUCCESS;
}

// char *negate(char *number) {
// 	if (number == NULL) {
// 		my_errno = INVALID_ARGUMENT;
// 		return NULL;
// 	}
// 	else if (negative(number)) {
// 		*number = ZERO;
// 		return remove_zeros(number);
// 	}
// 	else {
// 		char *negative_number = (char *)malloc(strlen(number) + 2);
// 		strncpy(negative_number+1, number, strlen(number) + 1);
// 		negative_number[0] = MINUS;
// 		return negative_number;
// 	}
// }


int add_zeros (char **number_, int to_len) {
	if (number_ == NULL)
		return INVALID_ARGUMENT;
	char *number = *number_;
	int number_len = strlen(number);
	if (number_len >= to_len)
		return SUCCESS; // уже больше
	else {
		// дополняем нулями до длины to_len
		int number_of_zeros = to_len - number_len;
		char *new_n = (char *)malloc(number_len + number_of_zeros + 1);
		int i;
		for (i = 0; i < number_of_zeros; i++)
			new_n[i] = ZERO;
		strncpy(new_n + number_of_zeros, number, number_len + 1);
		// записываем на место старого числа новое
		free (number);
		*number_ = new_n;
		return SUCCESS;
	}
}

// char *add_zeros(const char *number, int to_len) {
// 	int number_len = strlen(number);
// 	if (number_len >= to_len)
// 		return new_number(number); // ничего не надо делать
// 	else {
// 		// дополняем нулями до длины to_len
// 		int number_of_zeros = to_len - number_len;
// 		char *new_n = (char *)malloc(number_len + number_of_zeros + 1);
// 		int i;
// 		for (i = 0; i < number_of_zeros; i++)
// 			new_n[i] = ZERO;
// 		strncpy(new_n + number_of_zeros, number, number_len + 1);
// 		return new_n;
// 	}
// }

/*
	Вычитает из number1 number2. Оба числа записаны без
	знака. Результат может быть со знаком.
	Если какое-либо из чисел NULL, то результат INVALID_ARGUMENT
*/

int subtract(const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	int compare_args = compare(number1, number2);
	if (compare_args == NONE) {
		*number3 = new_number (zero_number);
	}
	else if (compare_args == FALSE) {
		char *result = NULL;
		subtract(number2, number1, &result);
		negate(&result);
		*number3 = result;
	}
	else {
		// основные вычисления
		// первое число больше второго
		int number_size = strlen(number1);
		char *new_n2 = new_number (number2);
		add_zeros(&new_n2, number_size);
		char *result = new_number(zero_number);
		add_zeros (&result, number_size);
		printf ("Первое число: %s\n", number1);
		printf ("Второе число: %s\n", new_n2);
		int carry_flag = 0;
		int i;
		for (i = number_size - 1; i >= 0; i--) {
			// вычитаем из i-ой цифры 1 числа i-ой цифру 2 числа с учетом carry flag
			int digit_result = number1[i] - new_n2[i] - carry_flag;
			if (digit_result < 0) {
				carry_flag = 1;
				digit_result += 10;
				printf ("Carry, result = %d\n", digit_result);
			}
			else {
				carry_flag = 0;
				printf ("No carry, result = %d\n", digit_result);
			}
			result[i] = digit_result + ZERO;
		}
		free (new_n2);
		remove_zeros (result);
		*number3 = result;
	}
	return SUCCESS;
}

// int add_signed(const char *signed_number1, const char *signed_number2, char **_result)
// {
// 	char *number1 = NULL, *number2 = NULL;
// 	char number1_sign = 0, number2_sign = 0, result_sign = 0;
// 	int code = 0;

// 	// Разделяем число и знак
// 	divide_signed_number(signed_number1, &number1, &number1_sign);
// 	divide_signed_number(signed_number2, &number2, &number2_sign);

// 	// Четыре случая: ++, +-, -+, --
// 	if(number1_sign == PLUS && number2_sign == PLUS)
// 		add(number1, number2, _result);
// 	else if(number1_sign == MINUS && number2_sign == MINUS)
// 	{
// 		add(number1, number2, _result);
// 		result_sign = PLUS;
// 		concat_number_with_sign()
// 	}
// }

int main()
{
	const char *number1 = "92758295792685692957927597297593";
	const char *number2 = "72985370692067259302572307";
	char *add_result = NULL;
	char *add_result_ = "92758368778056385025186899869900";
	char *subtract_result = NULL;
	char *subtract_result_= "-92758222807315000890668294725286";
	int code = add(number1, number2, &add_result);
	int code2 = subtract(number2, number1, &subtract_result);
	if(code != SUCCESS || code2 != SUCCESS)
		print_error();
	else
	{
		printf("%s + %s = %s\n", number1, number2, add_result);
		if (compare (add_result, add_result_) == NONE)
			printf ("Correct\n");
		printf("%s - %s = %s\n", number2, number1, subtract_result);
		if (compare (subtract_result, subtract_result_) == NONE)
			printf ("Correct\n");
		free(add_result);
		free(subtract_result);
	}
	return 0;
}