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
54, 38, 58, 62, 46, 26, 30, 17, 41, 31, 49, 53, 37, 0};
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '~'};

byte * fill_data(byte epoch) {
    static byte to_return[20];
    for (byte i = 0; i < 20; i++) {
        if (epoch == 3) {
            to_return[i] = 150;
        } else if (epoch == 0) {
            to_return[i] = 40;
        } else if (epoch == 2) {
            to_return[i] = 150 - (110*i/20);
        } else {
            to_return[i] = 40 + (110*i/20);
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
  for (int i = 0; i<80; i++) {
    analogWrite(A1, finger_data[0][i]);
    delay((r.r_rate*1000)/80); // one UOM is 80 iterations
  }
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
    while (true) {
      update_data(finger_data, r, s);
      Serial.print("Data updated!");
      Serial.print("\n");
      // Display the data
      for (byte j=0;j<8;j++) {
        for (int i = 0; i < 80; i++) {
          Serial.print(finger_data[j][i]);
          Serial.print(", ");
          if ((i+1)%20 == 0) {
              Serial.print("\n");
          }
      }
      Serial.print("===========================");
      Serial.print("\n");
    }
    }
    write_to_pins(finger_data, r);
}

void loop() {
  // put your main code here, to run repeatedly:
}