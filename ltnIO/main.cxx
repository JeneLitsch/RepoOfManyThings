#include "io.hxx"

void write() {
	auto line = ltn::readln();
	auto x = ltn::readfloat();
	auto file = ltn::fout("tmp/foo.txt");
	ltn::println(file, "Hello World");
	ltn::println(file, x);
	ltn::println(file, line);
}

void read() {
	auto file = ltn::fin("tmp/foo.txt");
	while(!ltn::isEof(file)) {
		ltn::println(ltn::readln(file));
	}
}

int main() {
	write();
	read();
	return 0;
}
