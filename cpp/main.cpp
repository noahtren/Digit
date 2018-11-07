#include "reader.cpp"
#include "encode.cpp" 
#include <iostream>
#define data_len 60
using namespace std;
int * string_map;
char * eight_char;
// Input variables
int rate;

void update_data(int finger_data[8][data_len], Reader r, BStream s) {
    if (r.read_s(s)) {
    eight_char = s.get_seg();
    string_map = encode_string(eight_char);
    for (int i = 0; i < 8; i++) {
        int * tmp = gen_data(string_map[i]);
        for (int j = 0; j < data_len; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    } else {
        cout << "we have reached the end of the msg" << endl;
    }
}

int main() {
    BStream s;
    Reader r;
    int finger_data[8][data_len];
    char input_string[64] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_'};
    rate = 1;
    // Setup variables according to input
    s.create(input_string, -8);
    r.create(rate, 8);
    update_data(finger_data, r, s);
    for (int h = 0; h < 8; h++) {
        cout << "Displaying the letter: " << s.get_seg()[h] << endl;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < (data_len/3)-1; j++) {
                cout << finger_data[h][j+i*(data_len/3)] << ",";
            }
            cout << finger_data[h][(data_len/3)-1+i*(data_len/3)];
            cout << endl;
        }
    }
}