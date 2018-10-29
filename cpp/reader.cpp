#include <iostream>
#include <string>
using namespace std;
class Stream {
    public:
        string msg;
        int index;
        void create(char [64], int);
        char * get_seg(int);
        void iterate(int);
};
void Stream::create(char m[64], int i) {
    msg = m;
    index = i;
}
char * Stream::get_seg(int length) {
    static char segment[8];
    for (int i = 0;i<length;i++) {
        cout << msg[index + i];
        segment[index+i] = msg[index+i];
    }
    return segment;
}
void Stream::iterate(int d) {
    index = index + d;
}

class Reader {
    int rate, length, direction;
    public:
        void create(int, int, int);
        bool read(Stream&);
};
void Reader::create(int r, int l, int d) {
    rate = r; // the time in seconds between
    length = l;
    direction = d;
}
bool Reader::read(Stream& s) {
    s.iterate(direction);
    if ((s.index+length) > s.msg.length()) {
        return false;
    } else {
        return true;
    }
}
 