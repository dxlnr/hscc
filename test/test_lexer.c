typedef struct {
    char* input;
    TokenList expected_output;
} TestCase;


int main() {
  TestCase tests[] = {
    { "int main() { return 0; }", /*expected output here*/ },
    { "/* Comment */ int x;", /*expected output here*/ },
  };

  for (int i = 0; i < sizeof(tests) / sizeof(TestCase); i++) {
    TokenList result = lexer(tests[i].input);
    if (!compareTokenLists(result, tests[i].expected_output)) {
        printf("Test %d failed!\n", i);
    }
  }

  return 0;
}
