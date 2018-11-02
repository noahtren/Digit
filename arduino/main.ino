class BStream {
    public:
        char msg[64];
        int index;
        void create(char [64], int);
        char * get_seg(int);
        void iterate(int);
};
void BStream::create(char m[64], int i) {
    for (int i=0;i<64;i++) {
        msg[i] = m[i];
    }
    index = i;
}
char * BStream::get_seg(int r_length) {
    static char segment[8];
    for (int i = 0;i<r_length;i++) {
        segment[index+i] = msg[index+i];
    }
    return segment;
}
void BStream::iterate(int d) {
    index = index + d;
}
 
class Reader {
    int r_length, r_direction;
    public:
        int r_rate;
        void create(int, int, int);
        bool read_s(BStream&);
};
void Reader::create(int r, int l, int d) {
    r_rate = r;
    r_length = l;
    r_direction = d;
}
bool Reader::read_s(BStream& s) {
    s.iterate(r_direction);
    if ((s.index+r_length) > 64) {
        return false;
    } else {
        return true;
    }
}
 
int code_epochs[] = {32, 40, 48, 52, 36, 56, 60, 44, 24, 60, 34, 42, 50,
54, 38, 58, 62, 46, 26, 30, 17, 41, 31, 49, 53, 37, 0}; // binary representation of braille characters
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '~'};

byte * fill_data(byte epoch) { // FUNCTION TO MANIPULATE FOR DIFFERENT INPUT PERCEPTIONS
    static byte to_return[20];
    for (byte i = 0; i < 20; i++) {
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
    byte i = 0, index = -1;
    while (code_map[i] != '~') {
        if (code_map[i] == to_find) {
            index = i;
        }
        i++;
    }
    return index;
}

byte * encode_string(char * str) {
    static byte arr[8];
    for (int i=0;i<8;i++) {
        arr[i] = get_index(str[i]);
    }
    return arr;
}

byte * gen_data(int cell_index) {
    static byte data[80];
    byte * to_fill;
    byte epochs[] = {code_epochs[cell_index] / 16, (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4,
    (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16 - ((code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4) * 4)};
    for (byte j=0; j<3; j++) {
        to_fill = fill_data(epochs[j]);
        for (byte k=j*20; k<(j+1)*20;k++) {
            data[k] = to_fill[k%20];
        }
    }
    return data;
}

byte * string_map;
char * eight_char;
// Input variables
int len;
int rate;

void update_data(byte finger_data[8][80], Reader r, BStream s) {
    if (r.read_s(s)) {
    eight_char = s.get_seg(len);
    string_map = encode_string(eight_char);
    for (byte i = 0; i < 8; i++) {
        byte * tmp = gen_data(string_map[i]);
        for (byte j = 0; j < 80; j++) {
            finger_data[i][j] = tmp[j];
        }
    }
    } else {
        Serial.print("we have reached the end of the msg");
    }
}

void write_to_pins(byte finger_data[8][80], Reader r) {
  for (int i = 0; i<20; i++) {
    analogWrite(A0, (finger_data[0][i]));
    analogWrite(A1, (finger_data[1][i]));
    analogWrite(A2, (finger_data[2][i]));
    delay(r.r_rate*1000); // 1s / 20
  }
  analogWrite(A0, 0);
  analogWrite(A1, 0);
  analogWrite(A2, 0);
}

void setup() {
    BStream s;
    Reader r;
    Serial.begin(9600);
    byte finger_data[8][80];
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
    r.create(rate, len, 8);
    update_data(finger_data, r, s);
    for (int i = 0; i < 80; i++) {
      Serial.print(finger_data[0][i]);
    }
    Serial.print("Data updated!");
    Serial.print("\n");
    write_to_pins(finger_data, r);
}

void loop() {
  // put your main code here, to run repeatedly:
}