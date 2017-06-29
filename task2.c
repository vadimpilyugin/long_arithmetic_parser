#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define SUCCESS 0
#define TRUE 1
#define FALSE 0
#define NONE 2
#define NO_MEMORY -3
#define INVALID_ARGUMENT -4
#define NULL_BYTE '\0'
#define BASE 10

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
		if(sum >= BASE)
		{
			cf = 1;
			sum -= BASE;
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
	assert ((*_result) != NULL);
	free (*_result);
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
		assert ((*number_) != NULL);
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
		assert ((*number_) != NULL);
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
	assert ((*number3) != NULL);
	free (*number3);
	if (compare_args == NONE) {
		*number3 = new_number (zero_number);
	}
	else if (compare_args == FALSE) {
		char *result = new_number (zero_number);
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
		// printf ("Первое число: %s\n", number1);
		// printf ("Второе число: %s\n", new_n2);
		int carry_flag = 0;
		int i;
		for (i = number_size - 1; i >= 0; i--) {
			// вычитаем из i-ой цифры 1 числа i-ой цифру 2 числа с учетом carry flag
			int digit_result = number1[i] - new_n2[i] - carry_flag;
			if (digit_result < 0) {
				carry_flag = 1;
				digit_result += BASE;
				// printf ("Carry, result = %d\n", digit_result);
			}
			else {
				carry_flag = 0;
				// printf ("No carry, result = %d\n", digit_result);
			}
			result[i] = digit_result + ZERO;
		}
		free (new_n2);
		// printf ("Результат с нулями: %s\n", result);
		remove_zeros (result);
		// printf ("Результат: %s\n", result);
		*number3 = result;
	}
	return SUCCESS;
}

int shift_left (char **number_, int times) {
	if (number_ == NULL)
		return INVALID_ARGUMENT;
	if (times == 0)
		return SUCCESS;
	char *number = *number_;
	int number_len = strlen (number);
	// Новое число будет иметь дополнительно times нулей
	char *new_n = (char *) malloc (number_len + 1 + times);
	strncpy (new_n, number, number_len + 1); // нулевой байт
	char *zeros = new_number(zero_number);
	add_zeros (&zeros, times);
	strncat (new_n, zeros, times + 1); // нулевой байт
	free (zeros);
	assert ((*number_) != NULL);
	free (*number_);
	*number_ = new_n;
	return SUCCESS;
}

int multiply_by_digit (const char *number, const char digit, char **number3) {
	if (number == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	// убираем лишние нули
	char *new_n = new_number (number);
	remove_zeros (new_n);
	int number_size = strlen(new_n);
	// в результате будет не больше number_size + 1 цифры
	char *result = new_number (zero_number);
	add_zeros (&result, number_size + 1);
	// умножаем
	int carry = 0;
	int digit_value = digit - ZERO;
	int i;
	int sum;
	for (i = number_size - 1; i >= 0; i--) {
		sum = (new_n[i] - ZERO) * digit_value + carry;
		result[i+1] = sum % BASE + ZERO;
		carry = sum / BASE;
	}
	result[0] = carry + ZERO;
	remove_zeros (result);
	assert ((*number3) != NULL);
	free (*number3);
	free (new_n);
	*number3 = result;
	return SUCCESS;
}

int multiply (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	assert (*number3 != NULL);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	remove_zeros (new_n1);
	remove_zeros (new_n2);
	// Для каждой цифры второго числа вычисляем произведение
	int n_size = strlen (new_n2);
	int i;
	// Сумму собираем в аккумуляторе
	char *accumulator = new_number (zero_number);
	char *sum = new_number (zero_number);
	for (i = n_size-1; i >= 0; i--) {
		multiply_by_digit (new_n1, new_n2[i], &sum);
		// результат умножения нужно сдвинуть
		shift_left (&sum, (n_size-1) - i);
		add (sum, accumulator, &accumulator);
	}
	remove_zeros (accumulator);
	free (new_n1);
	free (new_n2);
	free (sum);
	free (*number3);
	*number3 = accumulator;
	return SUCCESS;
}

void test_addition () {
	char *n1 = new_number ("0092758295792685692957927597297593");
	char *n2 = new_number ("000072985370692067259302572307");
	char *add_result = new_number (zero_number);
	add (n1, n2, &add_result);
	char *true_result = new_number ("92758368778056385025186899869900");
	assert (compare(add_result, true_result) == NONE);
	free (n1);
	free (n2);
	free (true_result);
	free (add_result);
	printf ("test_addition: Correct\n");
}


void test_same_addition () {
	char *n1 = new_number ("0092758295792685692957927597297593");
	char *n2 = new_number ("000072985370692067259302572307");
	// char *add_result = new_number (zero_number);
	add (n1, n2, &n2);
	char *true_result = new_number ("92758368778056385025186899869900");
	assert (compare(n2, true_result) == NONE);
	free (n1);
	free (n2);
	free (true_result);
	// free (add_result);
	printf ("test_same_addition: Correct\n");
}

void test_subtraction () {
	char *n1 = new_number ("0092758295792685692957927597297593");
	char *n2 = new_number ("000072985370692067259302572307");
	char *subtract_result = new_number (zero_number);
	subtract (n2, n1, &subtract_result);
	char *true_result = new_number ("-92758222807315000890668294725286");
	assert (compare (subtract_result, true_result) == NONE);
	free (n1);
	free (n2);
	free (true_result);
	free (subtract_result);
	printf ("test_subtraction: Correct\n");
}

void test_shift_left () {
	char *result = new_number ("95");
	shift_left (&result, 3);
	assert (compare (result, "95000") == NONE);
	printf ("test_shift_left: Correct\n");
	free (result);
}

void test_multiply_by_digit () {
	char *result = new_number (zero_number);
	multiply_by_digit ("7257483957492758", '7', &result);
	assert (compare (result, "50802387702449306") == NONE);
	printf ("test_multiply_by_digit: Correct\n");
	free (result);
}

void test_multiplication () {
	char *n1 = new_number ("0092758295792685692957927597297593");
	char *n2 = new_number ("000072985370692067259302572307");
	char *result = new_number ("0");
	char *true_result = new_number ("6769998603193588141062051447265683640784801860886679557051");
	multiply (n1,n2,&result);
	assert (compare (result, true_result) == NONE);
	printf ("test_multiplication: Correct\n");
	free (n1);
	free (n2);
	free (result);
	free (true_result);
}

int main()
{
	test_addition ();
	test_same_addition ();
	test_subtraction ();
	test_shift_left ();
	test_multiply_by_digit ();
	test_multiplication ();
	return 0;
}