#include "reader.cpp"
int main() {
    Stream s;
    Reader r;
    s.create("Hello there how are you", 0);
    r.create(1, 8, 1);
    while (r.read(s)) {
        cout << "\n";
    }
}