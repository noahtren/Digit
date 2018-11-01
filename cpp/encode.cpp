int code_epochs[] = {32, 40, 48, 52, 36, 56, 60, 44, 24, 60, 34, 42, 50,
54, 38, 58, 62, 46, 26, 30, 17, 41, 31, 49, 53, 37, 0};
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '~'};

int * fill_data(int epoch) {
    static int to_return[20];
    for (int i = 0; i < 20; i++) {
      if (i<7) { // less than 7
        if (epoch == 3) { // high high
            to_return[i] = 255;
        } else if (epoch == 0) { // low low
            to_return[i] = 0;
        } else if (epoch == 2) { // high low
            to_return[i] = 255;
        } else { // low high
            to_return[i] = 0;
        }
      } else if (i < 13) { // less than 13 {
        to_return[i] = 0;
      } else { // 4 or greater
        if (epoch == 3) { // high high
            to_return[i] = 255;
        } else if (epoch == 0) { // low low
            to_return[i] = 0;
        } else if (epoch == 2) { // high low
            to_return[i] = 0;
        } else { // low high
            to_return[i] = 255;
        }
      }
     } return to_return;
}

int get_index(char to_find) {
    int i = 0, index = -1;
    while (code_map[i] != '~') {
        if (code_map[i] == to_find) {
            index = i;
        }
        i++;
    }
    return index;
}

int * encode_string(char * str) {
    static int arr[8];
    for (int i=0;i<8;i++) {
        arr[i] = get_index(str[i]);
    }
    return arr;
}

int * gen_data(int cell_index) {
    static int data[100];
    int * to_fill;
    int epochs[] = {code_epochs[cell_index] / 16, (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4,
    (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16 - ((code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4) * 4)};
    for (int j=0; j<3; j++) {
        to_fill = fill_data(epochs[j]);
        for (int k=j*20; k<(j+1)*20;k++) {
            data[k] = to_fill[k%20];
        }
    }
    return data;
}