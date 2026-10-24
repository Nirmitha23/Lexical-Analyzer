    // Single line comment
   #include <stdio.h>

    /* Multi-line
    comment */

    int main() {
        int a = 10;
        float b = 20.5;
        double c = -30;
        char ch = 'A';

        if (a >= b && c != 0) {
            a++;
            b = b + 2;
        } else {
            a = a - 1;
        }

        for (int i = 0; i < 5; i++) {
            printf("Hello World\n");
        }

        return 0;
    }