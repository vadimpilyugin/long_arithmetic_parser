
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
	add (n1, n2, &n1);
	char *true_result = new_number ("92758368778056385025186899869900");
	assert (compare(n1, true_result) == NONE);
	free (n1);
	free (n2);
	free (true_result);
	// free (add_result);
	printf ("test_same_addition: Correct\n");
}

void test_subtraction () {
	char *n1 = new_number ("67");
	char *n2 = new_number ("17");
	char *n3 = new_number ("0102");
	char *n4 = new_number ("96");
	char *subtract_result = new_number (zero_number);
	subtract (n1, n2, &subtract_result);
	assert (compare (subtract_result, "50") == NONE);
	subtract (n3, n4, &subtract_result);
	assert (compare (subtract_result, "6") == NONE);
	free (n1);
	free (n2);
	free (n3);
	free (n4);
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

void tests_multiplication () {
	char *result = new_number (zero_number);
	multiply ("20", "30", &result);
	assert (compare (result, "600") == NONE);
	multiply ("174", "35", &result);
	assert (compare (result, "6090") == NONE);
	multiply ("2", "1", &result);
	assert (compare (result, "2") == NONE);
	multiply ("0", "233", &result);
	assert (compare (result, "0") == NONE);
	multiply ("323", "000", &result);
	assert (compare (result, "0") == NONE);
	free (result);
}

void test_compare () {
	assert(compare ("127", "016") == TRUE);
	assert(compare ("005", "016") == FALSE);
	assert(compare ("0997", "00997") == NONE);
	assert(compare ("67", "17") == TRUE);
	printf ("test_compare: Correct\n");
}

void test_divide_partial () {
	assert(divide_partial ("100", "16") == ZERO+6);
	assert(divide_partial ("16", "16") == ZERO+1);
	assert(divide_partial ("15", "16") == ZERO);
	assert(divide_partial ("1000", "100") == ZERO);
}

void test_add_digit () {
	char *number = new_number ("999");
	add_digit (&number, '0');
	assert(compare (number, "9990") == NONE);
	free (number);
}

void test_division () {
	char *result = new_number ("0");

	divide ("9", "9", &result);
	assert (compare(result, "1") == NONE);

	divide ("100", "16", &result);
	assert (compare(result, "6") == NONE);

	divide ("555", "5", &result);
	assert (compare(result, "111") == NONE);

	divide ("5", "555", &result);
	assert (compare(result, "0") == NONE);

	divide ("1024", "16", &result);
	assert (compare(result, "64") == NONE);

	divide ("0", "16", &result);
	assert (compare(result, "0") == NONE);

	divide ("16", "0", &result);
	assert (compare(result, "0") == NONE);

	free (result);
}

void test_add_signed () {
	char *n1 = new_number ("-273");
	char *n2 = new_number ("2344");
	char *n3 = new_number ("6654");
	char *n4 = new_number ("-4839");
	char *add_result = new_number (zero_number);
	add_signed (n2, n3, &add_result);
	assert (compare(add_result, "8998") == NONE);
	add_signed (n2, n4, &add_result);
	assert (compare(add_result, "-2495") == NONE);
	add_signed (n4, n3, &add_result);
	assert (compare(add_result, "1815") == NONE);
	add_signed (n1, n4, &add_result);
	assert (compare(add_result, "-5112") == NONE);
	free (n1);
	free (n2);
	free (n3);
	free (n4);
	free (add_result);
	printf ("test_addition_signed: Correct\n");
}
void test_subtract_signed () {
	char *n1 = new_number ("-17");
	char *n2 = new_number ("35");
	char *n3 = new_number ("23");
	char *n4 = new_number ("-67");
	char *add_result = new_number (zero_number);
	subtract_signed (n2, n3, &add_result);
	assert (compare(add_result, "12") == NONE);
	subtract_signed (n2, n4, &add_result);
	assert (compare(add_result, "102") == NONE);
	subtract_signed (n4, n3, &add_result);
	assert (compare(add_result, "-90") == NONE);
	subtract_signed (n1, n4, &add_result);
	assert (compare(add_result, "50") == NONE);
	free (n1);
	free (n2);
	free (n3);
	free (n4);
	free (add_result);
	printf ("test_subtraction_signed: Correct\n");
}

void test_mult_signed () {
	char *result = new_number (zero_number);
	
	mult_signed ("1", "2", &result);
	assert (compare (result, "2") == NONE);
	
	mult_signed ("-270", "10", &result);
	assert (compare (result, "-2700") == NONE);
	
	mult_signed ("4799", "-1727", &result);
	assert (compare (result, "-8287873") == NONE);
	
	mult_signed ("-122", "-20", &result);
	assert (compare (result, "2440") == NONE);
	
	free (result);
	printf ("test_mult_signed: Correct\n");
}
void test_div_signed () {
	char *result = new_number (zero_number);
	
	divide_signed ("1", "2", &result);
	assert (compare (result, "0") == NONE);
	
	divide_signed ("-270", "10", &result);
	assert (compare (result, "-27") == NONE);
	
	divide_signed ("4799", "-1727", &result);
	assert (compare (result, "-2") == NONE);
	
	divide_signed ("-122", "-20", &result);
	assert (compare (result, "6") == NONE);
	
	free (result);
	printf ("test_div_signed: Correct\n");
}

void test_parse () {
	Expression *expr;
	char *result = NULL;

	input = "0";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("0", result) == NONE);
	free_expression (expr);
	free (result);
	input = "1";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("1", result) == NONE);
	free_expression (expr);
	free (result);
	input = "2";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("2", result) == NONE);
	free_expression (expr);
	free (result);
	input = "9";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("9", result) == NONE);
	free_expression (expr);
	free (result);
	input = "10";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("10", result) == NONE);
	free_expression (expr);
	free (result);
	input = "+1";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("1", result) == NONE);
	free_expression (expr);
	free (result);
	input = "-1";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("-1", result) == NONE);
	free_expression (expr);
	free (result);
	input = "(1)";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("1", result) == NONE);
	free_expression (expr);
	free (result);
	input = "(-1)";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("-1", result) == NONE);
	free_expression (expr);
	free (result);
	input = "1+20";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	input = "1 +  20";
	expr = parse ();
	// printf ("Result: %s\n", expr -> token);
	result = eval (expr);
	// printf ("Answer = %s\n", result);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	// printf ("DELIM___\n");

	
	input = "1+20+300";
	expr = parse ();
	result = eval (expr);
	assert (compare ("321", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "1+20+300+4000";
	expr = parse ();
	result = eval (expr);
	assert (compare ("4321", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "-1+20";
	expr = parse ();
	result = eval (expr);
	assert (compare ("19", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "--1+20";
	expr = parse ();
	result = eval (expr);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "---1+20";
	expr = parse ();
	result = eval (expr);
	assert (compare ("19", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "(1+20)";
	expr = parse ();
	result = eval (expr);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "-2*3";
	expr = parse ();
	result = eval (expr);
	assert (compare ("-6", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "2*-3";
	expr = parse ();
	result = eval (expr);
	assert (compare ("-6", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "1++2";
	expr = parse ();
	result = eval (expr);
	assert (compare ("3", result) == NONE);
	free_expression (expr);
	free (result);

	input = "1+10*2";
	expr = parse ();
	result = eval (expr);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "10*2+1";
	expr = parse ();
	result = eval (expr);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "(1+20)*2";
	expr = parse ();
	result = eval (expr);
	assert (compare ("42", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "2*(1+20)";
	expr = parse ();
	result = eval (expr);
	assert (compare ("42", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "(1+2)*(3+4)";
	expr = parse ();
	result = eval (expr);
	assert (compare ("21", result) == NONE);
	free_expression (expr);
	free (result);

	input = "2*3+4*5";
	expr = parse ();
	result = eval (expr);
	assert (compare ("26", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "100+2*10+3";
	expr = parse ();
	result = eval (expr);
	assert (compare ("123", result) == NONE);
	free_expression (expr);
	free (result);
	
	input = "5 * 4 + 3 * 2 + 1";
	expr = parse ();
	result = eval (expr);
	assert (compare ("27", result) == NONE);
	free_expression (expr);
	free (result);
}

void test_big_parse () {
	Expression *expr = NULL;
	char *result = NULL;

	input = "(1+2)*3353454445445345434545435647/6";
	expr = parse ();
	result = eval (expr);
	// assert (compare ("1676727222722672717272717823", result) == NONE);
	free_expression (expr);
	free (result);
	// printf ("Прошел главный тест!\n");
}