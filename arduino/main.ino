#define data_len 60
#define parallel_cells 4
using namespace std;
int * string_map;
char * parallel_array;
// Input variables
int read_speed;

class BStream { // class representing a text stream that is being fed into the reader class
    public:
        char msg[64];
        int index;
        void create(char [64], int);
        char * get_seg();
        void update_index(int);
};
void BStream::create(char m[64], int idx) { // initialize
    for (int i=0;i<64;i++) {
        msg[i] = m[i];
    }
    index = idx;
}
char * BStream::get_seg() { // return a segment of the string according to index and reader length
    static char segment[parallel_cells];
    for (int i = 0;i<parallel_cells;i++) {
        segment[index+i] = msg[index+i];
    }
    return segment;
}
void BStream::update_index(int dir) { // move the index forward according to reader direction
    index = index + dir;
}
 
class Reader { // class that defines the reader for customization with contact points and reading speed
    int dir; // how many at a time, which direction we are reading in
    public:
        int rate;
        void create(int, int);
        bool can_read(BStream&);
        void iterate(BStream&);
};
void Reader::create(int r, int d) { // initialize
    rate = r;
    dir = d;
}
bool Reader::can_read(BStream& s) { // the main function
    if ((s.index+parallel_cells+dir) > 64) { // note that it must be initialized at -1
        return false;
    } else {
        return true;
    }
}
void Reader::iterate(BStream& s) {
    s.update_index(dir);
}
 
int code_epochs[] = {32, 40, 48, 52, 36, 56, 60, 44, 24, 60, 34, 42, 50,
54, 38, 58, 62, 46, 26, 30, 17, 41, 31, 49, 53, 37, 0, 1}; // binary representation of braille characters
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '.', '~'}; // language character associated with braille character code

int * fill_data(int epoch) { // takes a 2-bit input (0-3) and converts it into a vibration pattern
    static int to_return[data_len/3];
    for (int i = 0; i < data_len/3; i++) {
      if (i<data_len/9+1) { // less than 7
        if (epoch == 3) { // high high
            to_return[i] = 1;
        } else if (epoch == 0) { // low low
            to_return[i] = 0;
        } else if (epoch == 2) { // high low
            to_return[i] = 1;
        } else { // low high
            to_return[i] = 0;
        }
      } else if (i < (data_len/9)*2+1) { // less than 13 {
        to_return[i] = 0;
      } else { // 4 or greater
        if (epoch == 3) { // high high
            to_return[i] = 1;
        } else if (epoch == 0) { // low low
            to_return[i] = 0;
        } else if (epoch == 2) { // high low
            to_return[i] = 0;
        } else { // low high
            to_return[i] = 1;
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
    static int arr[parallel_cells];
    for (int i=0;i<parallel_cells;i++) {
        arr[i] = get_index(str[i]);
    }
    return arr;
}

int * gen_data(int cell_index) {
    static int data[data_len];
    int * to_fill;
    int epochs[] = {code_epochs[cell_index] / 16, (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4,
    (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16 - ((code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4) * 4)};
    for (int j=0; j<3; j++) {
        to_fill = fill_data(epochs[j]);
        for (int k=j*(data_len/3); k<(j+1)*(data_len/3);k++) {
            data[k] = to_fill[k%(data_len/3)];
        }
    }
    return data;
}

void update_data(int finger_data[8][data_len], Reader r, BStream s) {
    if (r.can_read(s)) {
    r.iterate(s);
    parallel_array = s.get_seg();
    string_map = encode_string(parallel_array);
    for (int i = 0; i < 8; i++) {
        int * tmp = gen_data(string_map[i]);
        for (int j = 0; j < data_len; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    } else {
        Serial.print("we have reached the end of the msg \n");
    }
}

void write_to_pins(byte finger_data[8][data_len], Reader r) {
  for (int i = 0; i<(data_len/3); i++) {
    analogWrite(A0, (finger_data[0][i])*255);
    analogWrite(A1, (finger_data[0][i+(data_len/3)])*255);
    analogWrite(A2, (finger_data[0][i+(data_len/3)*2])*255);
    Serial.print("1: ");
    Serial.print(finger_data[0][i]);
    Serial.print("\n");
    Serial.print("2: ");
    Serial.print(finger_data[0][i+(data_len/3)]);
    Serial.print("\n");
    Serial.print("3: ");
    Serial.print(finger_data[0][i+(data_len/3)*2]);
    Serial.print("\n");
    delay(r.rate*1000/20); // 1s / 20
  }
  analogWrite(A0, 0);
  analogWrite(A1, 0);
  analogWrite(A2, 0);
}

void setup() {
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
    read_speed = 1;
    // Setup variables according to input
    s.create(input_string, parallel_cells*-1);
    r.create(read_speed, parallel_cells);
    update_data(finger_data, r, s);
    for (int h = 0; h < 8; h++) {
        Serial.print("Displaying the letter: "); Serial.print(s.get_seg()[h]); Serial.print("\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < (data_len/3)-1; j++) {
                Serial.print(finger_data[h][j+i*(data_len/3)]); Serial.print(",");
            }
            Serial.print(finger_data[h][(data_len/3)-1+i*(data_len/3)]);
            Serial.print("\n");
        }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
}