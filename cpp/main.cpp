#include "reader.cpp"
#include "encode.cpp"
int finger_data[8][500];
int * string_map;
char * eight_char;
char master_string[64];
// Initialize objects
BStream s;
Reader r;
// Input variables
int length;
int rate;

void update_data() {
    // Iterate the message stream, break it into 8 characters
    // Convert these characters into finger_data
    if (r.read_s(s)) {
    // Convert input string into data
    eight_char = s.get_seg(length);
    string_map = encode_string(eight_char);
    // Create the data
    for (int i = 0; i < 8; i++) {
        int * tmp = gen_data(string_map[i]);
        for (int j = 0; j < 500; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    } else {
        cout << "we have reached the end of the msg";
    }

}

int main() {
    // Take input from bluetooth signal
    char input_string[64] = {'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_',
                             'h', 'e', 'l', 'l', 'o', '_', '_', '_'};
    for (int i = 0; i < 64; i++) {
        master_string[i] = input_string[i];
    }
    rate = 1;
    length = 8;
    // Setup variables according to input
    s.create(master_string, -1);
    r.create(rate, length, 1);

    update_data();

    // Display the data
    for (int i = 0; i < 500; i++) {
        cout << finger_data[0][i] << ", ";
        if ((i+1)%125 == 0) {
            cout << endl << endl;
        }
    }
}