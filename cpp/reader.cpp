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
char * BStream::get_seg(int length) {
    static char segment[8];
    for (int i = 0;i<length;i++) {
        segment[index+i] = msg[index+i];
    }
    return segment;
}
void BStream::iterate(int d) {
    index = index + d;
}

class Reader {
    int r_rate, r_length, r_direction;
    public:
        void create(int, int, int);
        bool read_s(BStream&);
};
void Reader::create(int r, int l, int d) {
    r_rate = r; // the time in seconds between
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
 