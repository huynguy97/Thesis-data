#include <iostream>
using namespace std;
struct Length
{
	int minutes;
	int seconds;
};

void one(){
    cout << b; // b is not declared here
}
void two(){
    one() // missing ;
    one();
}
void three(){
    if (1>0){
        cout << "error" // missing ; , but this time before a } token
    }
}
void four(Length a, Length b){ 
    if (a==b){ // == is not defined 
        cout << "error"; 
    }
}
bool five(bool x){
    bool y = bool x; // should remove bool in bool x;
    return y;
}

void six(){
    cout <<"error";
    int x[] = {1};
    int y = x[]; // should be x[0];
}

void seven(); 
{ // there should be no ; on the previous line
    cout <<"error";
}

void seven(){
    if (1>0){
    cout << "error";
    }
    else {
        cout <<"error";
}

int main()
{ 
    cout << "Hello world!" << endl;
    return 0;
} // missing a closing } in the previous function
