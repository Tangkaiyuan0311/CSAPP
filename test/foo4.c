#include <stdio.h>
void f();

int x; //weak

int main(){
        x = 15213;
        f();
        printf("x = %d\n", x);
        return 0;
}

