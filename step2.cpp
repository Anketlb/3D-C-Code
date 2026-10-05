#include <iostream>

using namespace std;

void add(int& a, int b){
    a += b;
    cout << a << endl;
    return;
}

int main(){
    
    int c = 99;
    int d = 2;

    add(c,d);
    cout << c << endl;

    return 0;
}