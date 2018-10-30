#include "reader.cpp"
#include "encode.cpp"
#include <iostream>
using namespace std;
int * string_map;
char * eight_char;
// Input variables
int len;
int rate;

void update_data(int finger_data[8][100], Reader r, BStream s) {
    // Iterate the message stream, break it into 8 characters
    // Convert these characters into finger_data
    if (r.read_s(s)) {
    // Convert input string into data
    eight_char = s.get_seg(len);
    string_map = encode_string(eight_char);
    // Create the data
    for (int i = 0; i < 8; i++) {
        int * tmp = gen_data(string_map[i]);
        for (int j = 0; j < 100; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    } else {
        cout << "we have reached the end of the msg";
    }

}

int main() {
    // Initialize objects
    BStream s;
    Reader r;
    int finger_data[8][100];
    // Take input from bluetooth signal
    char input_string[64] = {'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_'};
    rate = 1;
    len = 8;
    // Setup variables according to input
    s.create(input_string, -1);
    r.create(rate, len, 1);

    update_data(finger_data, r, s);

    // Display the data
    for (int i = 0; i < 100; i++) {
        cout << finger_data[0][i] << ", ";
        if ((i+1)%20 == 0) {
            cout << endl << endl;
        }
    }
}