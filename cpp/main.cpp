#include "reader.cpp"
#include "encode.cpp"
int main() {
    // Initialize objects
    Stream s;
    Reader r;
    // Take input from bluetooth signal
    char input_string[8] = {'h', 'e', 'l', 'l', 'o', '_', '_', '_'};
    // Setup variables
    s.create(input_string, 0);
    r.create(1, 8, 1);
    // Convert input string into data
    int * string_map = encode_string(input_string);
    int finger_data[8][500];
    for (int i = 0; i < 8; i++) {
        int * tmp = gen_data(string_map[i]);
        for (int j = 0; j < 500; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    for (int i = 0; i < 500; i++) {
        cout << finger_data[1][i] << ", ";
        if ((i+1)%125 == 0) {
            cout << endl << endl;
        }
    }
}