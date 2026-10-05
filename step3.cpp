#include <iostream>
#include <string>

using namespace std;

int main(){
    
    string line = "V 0 0 0";

    int space_index = line.find(" ");
    if(space_index != string::npos){
        cout << "First Space at index " << space_index << endl;
    }

    return 0;
}