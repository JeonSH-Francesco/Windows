#include <Windows.h>
#include <stdio.h>

int main(int argc, const char *argv[],char* env[]) {
	for (int i = 0;; i++) {
		if (env[i] == nullptr) {
			break;
		}
		auto equals = strchr(env[i], '=');

		*equals = '\0';
		printf("%s : %s\n", env[i], equals + 1);

		*equals = '=';

	}
	return 0;

}
