#define parallel_cells 4
class BStream { // class representing a text stream that is being fed into the reader class
    public:
        char msg[64];
        int index;
        void create(char [64], int);
        char * get_seg();
        void step(int);
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
void BStream::step(int dir) { // move the index forward according to reader direction
    index = index + dir;
}
 
class Reader { // class that defines the reader for customization with contact points and reading speed
    int dir; // how many at a time, which direction we are reading in
    public:
        int speed;
        void create(int, int);
        bool can_read(BStream&);
        void iterate(BStream&);
};
void Reader::create(int s, int d) { // initialize
    speed = s;
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
    s.step(dir);
}