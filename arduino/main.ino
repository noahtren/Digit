#define data_len 60
#define parallel_cells 8
byte pins [12] = {4, 3, 2, 8, 5, 6, 12, 7, 10, 11, 12, 9}; // the first six pins are correct, the rest not so much
byte * string_map;
char * parallel_array;
// Input variables
float read_speed;

class BStream { // class representing a text stream that is being fed into the reader class
    public:
        char msg[64];
        int index;
        void create(char [64], int);
        char * get_seg();
        void update_index(byte);
};
void BStream::create(char m[64], int idx) { // initialize
    for (byte i=0;i<64;i++) {
        msg[i] = m[i];
    }
    index = idx;
}
char * BStream::get_seg() { // return a segment of the string according to index and reader length
    static char segment[parallel_cells];
    for (byte i = 0;i<parallel_cells;i++) {
        segment[i] = msg[index+i];
    }
    return segment;
}
void BStream::update_index(byte dir) { // move the index forward according to reader direction
    index = index + dir;
}
 
class Reader { // class that defines the reader for customization with contact points and reading speed
    byte dir; // how many at a time, which direction we are reading in
    public:
        float rate;
        void create(float, byte);
        bool can_read(BStream&);
        void iterate(BStream&);
};
void Reader::create(float r, byte d) { // initialize
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

BStream s;
Reader r;

byte code_epochs[] = {32, 40, 48, 52, 36, 56, 60, 44, 24, 60, 34, 42, 50,
54, 38, 58, 62, 46, 26, 30, 17, 41, 31, 49, 53, 37, 0, 1}; // binary representation of braille characters
char code_map[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '_', '.', '~'}; // language character associated with braille character code

byte * fill_data(byte epoch) { // takes a 2-bit input (0-3) and converts it into a vibration pattern
    static byte to_return[data_len/3];
    for (byte i = 0; i < data_len/3; i++) {
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

byte get_index(char to_find) {
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
    static byte arr[parallel_cells];
    for (byte i=0;i<parallel_cells;i++) {
        arr[i] = get_index(str[i]);
    }
    return arr;
}

byte * gen_data(byte cell_index) { // this works
    static byte data[data_len];
    byte * to_fill;
    byte epochs[] = {code_epochs[cell_index] / 16, (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4,
    (code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16 - ((code_epochs[cell_index] - (code_epochs[cell_index] / 16) * 16)/4) * 4)};
    for (byte j=0; j<3; j++) {
        to_fill = fill_data(epochs[j]);
        for (byte k=j*(data_len/3); k<(j+1)*(data_len/3);k++) {
            data[k] = to_fill[k%(data_len/3)];
        }
    }
    return data;
}

void update_data(byte finger_data[8][data_len], Reader r, BStream s) {
    if (r.can_read(s)) {
      r.iterate(s);
      parallel_array = s.get_seg();
      Serial.print("first element of list");
      Serial.print(parallel_array[0]);
      Serial.print("\n");
      string_map = encode_string(parallel_array);
      for (byte i = 0; i < 8; i++) {
          byte * tmp = gen_data(string_map[i]);
          for (byte j = 0; j < data_len; j++) {
              finger_data[i][j] = tmp[j];
          }
      }
    } else {
        Serial.print("we have reached the end of the msg \n");
    }
}

void write_to_pins(byte finger_data[parallel_cells][data_len], Reader r) {
  for (byte i = 0; i<(data_len/3); i++) {
    for (byte pin = 0; pin < 6; pin++) {
      digitalWrite(pins[pin], (finger_data[pin/3][i+(pin%3)*(data_len/3)]));
    }
    delay(r.rate*1000/20); // 1s / 20
  }
  for (byte pin = 0; pin < 6; pin++) {
    digitalWrite(pins[pin], 0);
  }
}

void setup() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
    byte finger_data[8][data_len];
    // ##################################################
    char input_string[64] = {'a', 'n', 't', 's', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_',
                             '_', '_', '_', '_', '_', '_', '_', '_'};
    read_speed = 0.5;
    
    // Setup variables according to input. THIS IS WHERE USER CUSTOMIZATION COMES INTO PLAY
    s.create(input_string, parallel_cells*-1);
    r.create(read_speed, parallel_cells);
    // ##################################################
    update_data(finger_data, r, s);
    for (byte i = 0; i < 100; i++) {
      write_to_pins(finger_data, r);
      delay(read_speed*1000);
    }
}

void loop() {
  // put your main code here, to run repeatedly:
}