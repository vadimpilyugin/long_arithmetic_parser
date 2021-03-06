#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

#define SUCCESS 0
#define TRUE 1
#define FALSE 0
#define NONE 2
#define NULL_BYTE '\0'
#define BASE 10

#define ASSERT_ERROR -1
#define NO_MEMORY -2
#define READ_ERROR -3
#define INVALID_ARGUMENT -4
#define DEBUG 0

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
	if (n_zeros == strlen (old_str))
		// все нули, оставляем 1
		n_zeros--;
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

char *new_string (char ch) {
	char *new_str = (char *) malloc (2);
	new_str[0] = ch;
	new_str[1] = NULL_BYTE;
	return new_str;
}
char *empty_string () {
	char *new_str = (char *) malloc (1);
	new_str[0] = NULL_BYTE;
	return new_str;
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
		// printf ("Compare: %d, Arg1: %s, Arg2: %s\n", compare, new_n1, new_n2);
		if (compare > 0)
			n1_bigger = TRUE;
		else if (compare < 0)
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
	assert ((*number3) != NULL);
	free (*number3);
	int compare_args = compare(number1, number2);
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

char divide_partial (const char *number1, const char *number2) {
	char *result = new_number ("0");
	int i;
	char ans = ZERO;
	for (i = 1; i < BASE; i++) {
		multiply_by_digit (number2, i+ZERO, &result);
		int compare_args = compare (result, number1);
		// если уже больше, то вернуть предыдущюю цифру
		if (compare_args == TRUE) {
			ans = (i-1) + ZERO;
			break;
		}
		// если равно, то вернуть эту цифру
		else if (compare_args == NONE) {
			ans = i + ZERO;
			break;
		}
	}
	if (i == BASE) {
		// так и не накопили
		my_errno = INVALID_ARGUMENT;
		ans = ZERO;
	}
	free (result);
	return ans;
}

// приписывает справа к числу цифру
int add_digit (char **number_, char digit) {
	if (number_ == NULL)
		return INVALID_ARGUMENT;
	shift_left (number_, 1);
	char *number = *number_;
	number[strlen(number)-1] = digit;
	return SUCCESS;
}

int divide (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	free (*number3);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	remove_zeros (new_n1);
	remove_zeros (new_n2);
	// начинаем с нуля
	char *start_from = new_number ("0");
	char *tmp = new_number ("0");
	char *tmp2 = new_number ("0");
	char *result = new_number ("0");
	// идем циклом по цифрам делимого
	int i;
	int n1_len = strlen (new_n1);
	for (i = 0; i < n1_len; i++) {
		// добавляем цифру к стартовому значению
		add_digit (&start_from, new_n1[i]);
		// printf ("Добавляем цифру, теперь %s\n", start_from);
		// проверяем, не делится ли уже стартовое значение на делитель
		char div_value = divide_partial (start_from, new_n2);
		if (div_value == ZERO)
			; // все еще не делится
		else {
			// printf ("Делится %s на %s: %c раз\n", start_from, new_n2, div_value);
			// вычитаем из стартового значения произведения цифры на делитель
			multiply_by_digit (new_n2, div_value, &tmp);
			// printf ("Умножаем %s на %c: %s\n", new_n2, div_value, tmp);
			// результатом будет новое стартовое значение
			subtract (start_from, tmp, &tmp2);
			free (start_from);
			start_from = new_number (tmp2);
			// printf ("Вычитаем, осталось %s\n", start_from);
			// цифра идет в частное
			add_digit (&result, div_value);
		}

	}
	// в результате в start_from будет mod, а в result - div
	remove_zeros (result);
	free (new_n1);
	free (new_n2);
	free (start_from);
	free (tmp);
	free (tmp2);
	*number3 = result;
	return SUCCESS;
}



int add_signed (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	assert (*number3 != NULL);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	// printf ("%s, %s\n", new_n1, new_n2);
	// сложение с учетом знаков
	if (negative (new_n1) && negative (new_n2)) {
		// оба отрицательные, то же самое, что сложить модули и negate
		negate (&new_n1);
		negate (&new_n2);
		add (new_n1, new_n2, number3);
		negate (number3);
	}
	else if (negative (new_n1) && !negative (new_n2)) {
		// вычесть из 2 1
		negate (&new_n1);
		subtract (new_n2, new_n1, number3);
	}
	else if (!negative (new_n1) && negative (new_n2)) {
		// вычесть из 1 2
		negate (&new_n2);
		subtract (new_n1, new_n2, number3);
	}
	else {
		// оба положительные, сумма
		add (new_n1, new_n2, number3);
	}
	free (new_n1);
	free (new_n2);
	return SUCCESS;
}
int subtract_signed (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	assert (*number3 != NULL);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	// вычитание с учетом знаков
	if (negative (new_n1) && negative (new_n2)) {
		// оба отрицательные, то же самое, что сложить модули и negate
		negate (&new_n1);
		negate (&new_n2);
		// вычесть из 2 1
		// printf ("Arg1: %s, Arg2: %s\n", new_n1, new_n2);
		subtract (new_n2, new_n1, number3);
		// printf ("Result: %s\n", *number3);
	}
	else if (negative (new_n1) && !negative (new_n2)) {
		negate (&new_n1);
		add (new_n1, new_n2, number3);
		negate (number3);
	}
	else if (!negative (new_n1) && negative (new_n2)) {
		negate (&new_n2);
		// оба положительные, сумма
		add (new_n1, new_n2, number3);
		// вычесть из 1 2
	}
	else {
		subtract (new_n1, new_n2, number3);
	}
	free (new_n1);
	free (new_n2);
	return SUCCESS;
}
int mult_signed (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	assert (*number3 != NULL);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);
	if (negative (new_n1) && negative (new_n2)) {
		// оба отрицательные, то же самое, что положительные
		negate (&new_n1);
		negate (&new_n2);
		multiply (new_n1, new_n2, number3);
		
	}
	else if (negative (new_n1) && !negative (new_n2)) {
		negate (&new_n1);
		multiply (new_n1, new_n2, number3);
		negate (number3);
	}
	else if (!negative (new_n1) && negative (new_n2)) {
		negate (&new_n2);
		multiply (new_n1, new_n2, number3);
		negate (number3);
	}
	else {
		multiply (new_n1, new_n2, number3);
	}
	free (new_n1);
	free (new_n2);
	return SUCCESS;
}

int divide_signed (const char *number1, const char *number2, char **number3) {
	if (number1 == NULL || number2 == NULL || number3 == NULL)
		return INVALID_ARGUMENT;
	assert (*number3 != NULL);
	char *new_n1 = new_number (number1);
	char *new_n2 = new_number (number2);

	int n1_neg = negative (new_n1);
	int n2_neg = negative (new_n2);

	if (n1_neg) 
		negate (&new_n1);
	
	if (n2_neg)
		negate (&new_n2);
	divide (new_n1, new_n2, number3);

	if (n1_neg && n2_neg)
		; // уже правильно
	else if (n1_neg || n2_neg)
		negate (number3);

	free (new_n1);
	free (new_n2);
	return SUCCESS;
}

char *input = NULL;


/*
	Если произошла ошибка парсинга, то вернется NULL
	Если дошли до конца строки, то вернется пустая строка
*/
char *parse_tokens () {
	// Пропускаем пробелы перед токеном
	// printf ("Состояние строки: %s, strlen = %d\n", input, strlen (input));
	while (isspace (*input) && (*input) != NULL_BYTE) input++;
	// Если input начинается с цифры, то парсим число
	if (isdigit (*input)) {
		char *number = new_number (zero_number);
		while (isdigit (*input)) add_digit(&number, *(input++));
		remove_zeros (number);
		// printf ("Input: %s\n", number);
		return number;
	}
	// printf ("Привет: %s\n", input);
	// Проходимся по всем известным программе токенам
	// Они все однобайтовые
	switch (*(input++)) {
		case PLUS: 
			// printf ("Plus\n");
			return new_string (PLUS);
			break;
		case MINUS:
			return new_string (MINUS);
			break;
		case MULT:
			return new_string (MULT);
			break;
		case DIV:
			return new_string (DIV);
			break;
		case OPENING_BRACE:
			return new_string (OPENING_BRACE);
			break;
		case CLOSING_BRACE:
			return new_string (CLOSING_BRACE);
			break;
		case NULL_BYTE:
			// printf ("Empty\n");
			input --; // вернуть нулевой байт
			return empty_string();
			break;
		default:
			// несуществующая лексема
			return NULL;
	}
}
struct Expression {
	char *token;
	struct Expression *left;
	struct Expression *right;
};

typedef struct Expression Expression;


void free_expression (Expression *expr) {
	if (expr == NULL)
		return;
	if (expr -> token != NULL) {
		free (expr -> token);
		expr -> token = NULL;
	}
	if (expr -> left != NULL) {
		free_expression (expr -> left);
		expr -> left = NULL;
	}
	if (expr -> right != NULL) {
		free_expression (expr -> right);
		expr -> right = NULL;
	}
	free (expr);
}

Expression *parse ();

/*
	Парсит простое выражение и записывает его в структуру
	NULL в случае ошибки

*/

Expression *parse_simple_expression() {
	// Парсим первый токен
	char *token = parse_tokens ();
	// Неожиданный конец строки, или неизвестный токен
	if (token == NULL)
		return NULL;
	else if (strlen (token) == 0)
		return NULL;
	else {
		// Если это цифра, возвращаем выражение без аргументов
		if (isdigit (token[0])) {
			Expression *result = (Expression *)malloc (sizeof (Expression));
			result -> token = token;
			result -> left = NULL;
			result -> right = NULL;
			// printf ("Foo: %s\n", token);
			return result;
		}
		else if (token[0] == OPENING_BRACE) {
			free (token); // скобка нам не нужна
			// Если это скобки, парсим и возвращаем выражение в скобках
			Expression *next_token = parse ();
			if (next_token == NULL)
				// ошибка при парсинге
				return NULL;
			// Проверяем, что закрывающая скобка на месте
			char *closing_brace = parse_tokens ();
			if (*closing_brace != CLOSING_BRACE) {
				free (closing_brace);
				free_expression (next_token);
				return NULL;
			}
			else {
				
				free (closing_brace);
				return next_token;
			}
		}
		else {
			// Иначе, это унарная операция ("-")
			Expression *unary_operand = parse_simple_expression (); // Парсим ее аргумент
			if (unary_operand == NULL) {
				free (token);
				return NULL;
			}
			// Заводим новое выражение
			Expression *unary_operation = (Expression *) malloc (sizeof(Expression));
			unary_operation -> token = token;
			unary_operation -> left = unary_operand;
			unary_operation -> right = NULL;
			return unary_operation;
		}
	}
}

int get_priority(const char *token) {
    if (token[0] == PLUS) return 1;
    if (token[0] == MINUS) return 1;
    if (token[0] == MULT) return 2;
    if (token[0] == DIV) return 2;
    return 0; // Возвращаем 0 если токен - это не бинарная операция (например ")")
}


// NULL, если ошибка
Expression *parse_binary_expression(int min_priority) {
	Expression *left_expr = parse_simple_expression(); // Парсим простое выражение
	if (left_expr == NULL)
		return NULL;
	for (;;) {
		char *op = parse_tokens (); // Пробуем парсить бинарную операцию
		if (op == NULL) {
			free_expression (left_expr);
			return NULL;
		}
		int priority = get_priority(op);
		// printf ("Operation: %s, priority: %d, min: %d\n", op, priority, min_priority);
        
		// Выходим из цикла если ее приоритет слишком низок (или это не бинарная операция)
		if (priority <= min_priority) {
            input -= strlen (op); // Отдаем токен обратно
            free (op);
		// printf ("Hello, %s\n", left_expr -> token);
            return left_expr; // возвращаем выражение слева.
        }
        // Парсим выражение справа. Текущая операция задает минимальный приоритет.
        Expression *right_expr = parse_binary_expression(priority);
        if (right_expr == NULL) {
        	free_expression (left_expr);
        	free (op);
        	return NULL;
        }
        Expression *new_expr = (Expression *)malloc(sizeof(Expression));
        new_expr -> token = op;
        new_expr -> left = left_expr;
        new_expr -> right = right_expr;
        left_expr = new_expr; // Обновляем выражение слева
        // printf ("Parsed binary expression\n");
    } // Повторяем цикл: парсинг операции, и проверка ее приоритета.
}

Expression *parse () {
	return parse_binary_expression (0);
}

// NULL при ошибке
char *eval(Expression *e) {
	if (e == NULL)
		return NULL;
	if (e -> left != NULL && e -> right != NULL) {
		char *a = eval (e -> left);
		char *b = eval (e -> right);
		char *result = new_number (zero_number);
		if (e -> token[0] == PLUS) {
			add_signed (a,b,&result);
			free (a);
			free (b);
			return result;
		}
		if (e -> token[0] == MINUS) {
			subtract_signed (a,b,&result);
			free (a);
			free (b);
			return result;
		}
		if (e -> token[0] == MULT) {
			mult_signed (a,b,&result);
			free (a);
			free (b);
			return result;
		}
		if (e -> token[0] == DIV) {
			divide_signed (a,b,&result);
			free (a);
			free (b);
			return result;
		}
		// неопознанный оператор
		free (result);
		return NULL;
	}
	else if ((e -> left != NULL && e -> right == NULL) || (e -> left == NULL && e -> right != NULL)) {
		char *a = eval (e -> left);
		if (e -> token[0] == PLUS) {
			// вернуть само число
			return a;
		}
		if (e -> token[0] == MINUS) {
			// вернуть негатив числа
			negate (&a);
			return a;
		}
		return NULL;
	}
	else {
		// вернуть токен
		// printf ("Bar: %s\n", e->token);
		return new_number(e -> token);
	}
	return NULL;
}

/*
	Функция возвращает массив строк, прочитанных из ввода
*/

int get_lines(char ***_lines, size_t *_lines_num, size_t **_lines_len)
{
	size_t MAX_LINES = 20;
	size_t MULTIPLE = 2;

	// Array of lines
	char **lines = *_lines;
	// Size of that array
	size_t total_lines = *_lines_num;
	// Array containing number of characters in each line
	size_t *lines_len = lines_len;
	// Counter
	size_t i = 0;
	// Number of lines read from stdin
	size_t lines_read = 0;
	// Temporary buffer for a line
	char *line = NULL;
	// Size of that buffer
	size_t line_len = 0;
	// Number of characters or error code retreived via getline() call
	ssize_t chars_read = 0;
	// Temporary buffer for realloc()
	void *buf = NULL;

	// Allocate buffer for all lines
	lines = (char **)malloc(MAX_LINES*sizeof(char *));
	if(lines == NULL) {
		*_lines = NULL;
		*_lines_num = 0;
		*_lines_len = NULL;
		return NO_MEMORY;
	}
	total_lines = MAX_LINES;
	for(i = 0; i < total_lines; i++)
		lines[i] = NULL;
	// Allocate buffer for the number of characters in each line
	lines_len = (size_t *)malloc(MAX_LINES*sizeof(size_t));
	if(lines_len == NULL) {
		free(lines);
		*_lines = NULL;
		*_lines_num = 0;
		*_lines_len = NULL;
		return NO_MEMORY;
	}
	for(i = 0; i < MAX_LINES; i++)
		lines_len[i] = 0;
	while((chars_read = getline(&line,&line_len,stdin)) != -1)
	{
		// read next line
		lines[lines_read] = line;
		// write the number of read chars to array
		lines_len[lines_read] = chars_read;
		line = NULL;
		line_len = 0;
		lines_read++;
		if(lines_read == total_lines) {
			// reallocate lines buffer
			// printf("Hello\n");
			buf = (char **)realloc(lines,total_lines*MULTIPLE*sizeof(char *));
			// printf("==Hello==\n");
			if(buf == NULL) {
				// realloc failed, free all read lines
				for(i = 0; i < lines_read; i++)
					free(lines[i]);
				free(lines);
				free(lines_len);
				*_lines = NULL;
				*_lines_num = 0;
				*_lines_len = NULL;
				return NO_MEMORY;
			}
			lines = buf;
			// realloc does not initialize allocated memory
			for(i = lines_read; i < total_lines*MULTIPLE; i++)
				lines[i] = NULL;
			// reallocate lines length buffer
			buf = (size_t *)realloc(lines_len,total_lines*MULTIPLE*sizeof(size_t));
			if(buf == NULL) {
				// realloc failed, free all read lines
				for(i = 0; i < lines_read; i++)
					free(lines[i]);
				free(lines);
				free(lines_len);
				*_lines = NULL;
				*_lines_num = 0;
				*_lines_len = NULL;
				return NO_MEMORY;
			}
			lines_len = buf;		
			total_lines *= MULTIPLE;
		}
	}
	// getline failed, free allocated buffer
	free(line);
	if(errno == 0)
	{
		// We have read the entire file without errors
		// write NULL pointer to last line
		lines[lines_read] = NULL;
		*_lines = lines;
		*_lines_num = lines_read;
		*_lines_len = lines_len;
		return SUCCESS;
	}
	else if(errno != 0)
	{
		// Error happened while reading the file
		// free all read lines
		for(i = 0; i < lines_read; i++)
			free(lines[i]);
		free(lines);
		free(lines_len);
		*_lines = NULL;
		*_lines_num = 0;
		*_lines_len = NULL;
		return READ_ERROR;
	}
	return ASSERT_ERROR;
}

/* Объединяет весь массив строк в одну строку */
char *my_getline () {
	char **lines = NULL; // переменная для массива строк
	size_t lines_num = 0; // для числа прочитанных строк
	size_t *lines_len = NULL; // для длины каждой строки
	int code = get_lines (&lines, &lines_num, &lines_len);
	if (code != SUCCESS)
		return NULL;
	else {
		size_t i;
		size_t sum = 0;
		for (i = 0; i < lines_num; i++)
			sum += lines_len[i];
		// Теперь мы знаем длину итоговой строки
		char *result = (char *) malloc (sum + 1);
		result[0] = NULL_BYTE; // изначально строка нулевой длины
		for (i = 0; i < lines_num; i++)
			strncat (result, lines[i], lines_len[i] + 1);
		for (i = 0; i < lines_num; i++) {
			free (lines[i]);
		}
		free (lines);
		free (lines_len);
		return result;
	}
}

#if DEBUG
#include "tests.h"
#endif
int main()
{
	#if DEBUG
	test_compare ();
	test_addition ();
	test_same_addition ();
	test_subtraction ();
	test_shift_left ();
	test_multiply_by_digit ();
	test_multiplication ();
	tests_multiplication ();
	test_divide_partial ();
	test_add_digit ();
	test_division ();
	test_add_signed ();
	test_subtract_signed ();
	test_mult_signed ();
	test_div_signed ();
	test_parse ();
	test_big_parse ();
	#endif

	input = my_getline ();
	char *start_input_pointer = input; // для free
	if (input == NULL) {
		// getline failed
		print_error();
	}
	else {
		// в input теперь находится все выражение
		Expression *expr = parse ();
		// printf ("Hello");
		char *answer = eval (expr);
		if (expr == NULL || answer == NULL)
			print_error ();
		else
			printf ("%s\n", answer);
		if (expr)
			free_expression (expr);
		if (answer)
			free (answer);
		// printf ("Состояние строки: %s\n", start_input_pointer);
		free (start_input_pointer);
	}
	return 0;
}