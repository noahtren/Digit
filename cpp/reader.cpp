#include <iostream>
#include <string>
using namespace std;
class Stream {
    public:
        string msg;
        int index;
        void create(string, int);
        string get_seg(int);
        void iterate(int);
};
void Stream::create(string m, int i) {
    msg = m;
    index = i;
}
string Stream::get_seg(int length) {
    string segment = msg.substr(index, length);
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
    cout << s.get_seg(length);
    s.iterate(direction);
    if ((s.index+length) > s.msg.length()) {
        return false;
    } else {
        return true;
    }
}
 