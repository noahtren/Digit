class BStream { // class representing a text stream that is being fed into the reader class
    public:
        char msg[64];
        int index;
        void create(char [64], int);
        char * get_seg();
        void iterate(int);
};
void BStream::create(char m[64], int idx) { // initialize
    for (int i=0;i<64;i++) {
        msg[i] = m[i];
    }
    index = idx;
}
char * BStream::get_seg() { // return a segment of the string according to index and reader length
    static char segment[8];
    for (int i = 0;i<8;i++) {
        segment[index+i] = msg[index+i];
    }
    return segment;
}
void BStream::iterate(int dir) { // move the index forward according to reader direction
    index = index + dir;
}
 
class Reader { // class that defines the reader for customization with contact points and reading speed
    int r_direction; // how many at a time, which direction we are reading in
    public:
        int r_rate;
        void create(int, int);
        bool read_s(BStream&);
};
void Reader::create(int r, int d) { // initialize
    r_rate = r;
    r_direction = d;
}
bool Reader::read_s(BStream& s) { // the main function
    s.iterate(r_direction); // first it steps forwards in the reading list
    if ((s.index+8) > 64) { // note that it must be initialized at -1
        return false;
    } else {
        return true;
    }
}