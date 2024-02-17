#include <stdio.h>
#include <string.h>
const char * const GREEN = "\x1b[32m";
const char * const YELLOW = "\x1b[33m";
const char * const WHITE = "\x1b[0m";
const char * current = WHITE;

void setColour(const char *colour) {
  if (current == colour) return;
  printf("%s", colour);
  current = colour;
}

int strcount(char *sourcestr, char tar) {
  int count = 0;
  // simple string count function
  for (size_t i = 0; i <= strlen(sourcestr); ++i) {
    if (sourcestr[i] == tar) ++count;
  }
  return count;
}

void checkGuess(char *guess, char *tar, int *scores) {
  //assuming guess, target, and scores are of same length are of same length.
  //checks for exact chars and like chars 2 if exact 1 if alike (alike means char in guess is in target, exact means they're in the same spot)
  int size = strlen(tar) + 1;
  for (size_t i = 0; i < size; ++i) {
    if (guess[i] == tar[i]) scores[i] = 2;
    else {
      scores[i] = 0;
      for (size_t j = 0; j < size; ++j) {
        if (guess[i] == tar[j]) {
          scores[i] = 1;
          break;
        }
      }
    }
  }
}

void fixScore(char *tar, int *scores, char *guess, char c){
  int size = strlen(tar) + 1;
  int cCount = strcount(tar, c);
  //checkGuess doesn't account for priority and/or amount of chars so this function fixes the score array
  //for instance if the word to guess was "apple" and a user typed "ppppp", with just checkGuess the score array would look like [1,2,2,1,1] which is wrong
  //so this function fixes that by comparing the character of an index with the target word and the guess word fixing the score array at the same time.
  for (size_t j = 0; j < size - 1; ++j) {
    char check = tar[j];
    if (scores[j] == 2 && check == c && cCount > 0) --cCount;
  }
  for (size_t k = 0; k < size - 1; ++k) {
    char check = guess[k];
    if (scores[k] == 1 && check == c && cCount > 0) --cCount;
    else {
      if (scores[k] == 1 && check == c && cCount == 0) scores[k] = 0;
    }
  }
}

void printWord(char *guess, int *score) {
  for (size_t i = 0; i < strlen(guess); ++i){
    switch (score[i]) {
      case 0: //char is completely wrong
        setColour(WHITE);
        printf("%c",guess[i]);
        break;
      case 1: //char is in the word but in the wrong spot
        setColour(YELLOW);
        printf("%c",guess[i]);
        break;
      case 2: //char is in the right spot
        setColour(GREEN);
        printf("%c",guess[i]);
        break;
      default: //this should straight up never happen since the scoreArray is initialized and populated before this function is called
        printf("This should not be happenening");
        break;
    }
  }
  
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Invalid number of arguments\n");
    return 1;
  }
  char uStr[13]; //one of the flags doesn't let me do strlen(argv[1]) + 1 so we just set it to 13 (max word length is 12 + null terminator)
  int count = 6;
  while (!feof(stdin) && count > 0) {
    printf("Enter guess: ");
    scanf("%s", uStr);
    if (strlen(uStr) != strlen(argv[1])) {
      printf("Invalid string, not the same length as argument\n");
    return 2;
    }
    else {
      int scoreArr[13]; //really this could be 12 since the 13th spot shouldn't get filled but lets just keep it 13
      checkGuess(uStr,argv[1], scoreArr);
      for (size_t i = 0; i < strlen(argv[1]) + 1; ++i) fixScore(argv[1],scoreArr,uStr, uStr[i]);
      printWord(uStr,scoreArr);
      printf("\n");
      setColour(WHITE);
      int cumScore = 0;
      for (size_t i = 0; i < strlen(argv[1]); ++i) cumScore = cumScore + scoreArr[i];
      --count;
      if (cumScore == strlen(argv[1])*2) {
        printf("Finished in %d guesses\n", (6-count));
        return 0;
      }
    }
  }
  
  printf("Failed to guess the word: %s\n", argv[1]);
  return 0;
}
