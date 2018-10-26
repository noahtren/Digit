#include <string>
#include <iostream>
using namespace std;
char code_epochs[][3] = {
    {'F', 'L', 'L'}, // H high
    {'F', 'F', 'L'}, // L low
    {'H', 'L','L'}, // R rising
    {'H', 'R', 'L'}, // F falling
    {'F', 'R', 'L'},
    {'H', 'F', 'L'},
    {'H', 'H', 'L'},
    {'F', 'H', 'L'},
    {'R', 'F', 'L'},
    {'H', 'H', 'L'},
    {'F', 'L', 'F'},
    {'F', 'F', 'F'},
    {'H', 'L','F'},
    {'H', 'R', 'F'},
    {'F', 'R', 'F'},
    {'H', 'F', 'F'},
    {'H', 'H', 'F'},
    {'F', 'H', 'F'},
    {'R', 'F', 'F'},
    {'R', 'H', 'F'},
    {'R', 'L', 'H'},
    {'F', 'F', 'H'},
    {'R', 'H', 'R'},
    {'H', 'L', 'H'},
    {'H', 'R', 'H'},
    {'F', 'R', 'H'},
    {'L', 'L', 'L'}
};
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '~'};

int * fill_data(char epoch) {
    static int to_return[125];
    if (epoch == 'H') {
        for (int i=0;i<125;i++) {
            to_return[i] = 150;
        }
    } else if (epoch == 'L') {
        for (int i=0;i<125;i++) {
            to_return[i] = 40;
        }
    } else if (epoch == 'R') {
        for (int i=0; i<125; i++) {
            to_return[i] = 40 + (110*i/125);
        }
    } else {
        for (int i=0; i<125; i++) {
            to_return[i] = 150 - (110*i/125);

        }
    }
    return to_return;
}

int get_index(char * arr, char to_find) {
    int i = 0, index = -1;
    while (arr[i] != '~') {
        if (arr[i] == to_find) {
            index = i;
        }
        i++;
    }
    return index;
}

int * encode_string(char * str) {
    static int arr[8];
    for (int i=0;i<8;i++) {
        arr[i] = get_index(code_map, str[i]);
    }
    return arr;
}

int * gen_data(int cell_index) {
    static int data[500]; // Hz can update 500 times per second, 2ms resolution
    // each epoch will be 125 data points long, or max .25 seconds
    int * to_fill;
    for (int j=0; j<3; j++) {
        to_fill = fill_data(code_epochs[cell_index][j]);
        for (int k=j*125; k<(j+1)*125;k++) {
            data[k] = to_fill[k%125];
        }
    }
    return data;
}