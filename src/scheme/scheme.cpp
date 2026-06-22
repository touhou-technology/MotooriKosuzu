extern "C" {
#include <chibi/eval.h>
}

int main(int argc, char *argv[]) {
	sexp ctx = sexp_make_eval_context(nullptr, nullptr, nullptr, 0, 0);

	sexp_load_standard_env(ctx, nullptr, SEXP_SEVEN);
	sexp_load_standard_ports(ctx, nullptr, stdin, stdout, stderr, 0);

	sexp_eval_string(ctx, "(display \"Hello World\") (newline)", -1, nullptr);

	sexp_destroy_context(ctx);

	return 0;
}
