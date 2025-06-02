#include <stdio.h>

void guessNumber(int guess) {
    if(guess < 555) {
        printf("You guess is too low.\n");
    }else if (guess > 555){
        printf ("your guess is too high.\n");
    }else{
        printf("correct. you guessed it!\n");
    }

}
int main(){
    guessNumber(500);
    guessNumber(600);
    guessNumber(555);

}
