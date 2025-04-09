#include "SpeakingRecursian.h"
using namespace std;

void generateWordsRecursive(int numSyllables, string currentWord, Vector<string>& words, bool firstSyllable, int& index) {
    // Base case: if no more syllables to add, add the current word to the vector
    if (numSyllables == 0) {
        words[index] = currentWord; // Assign the current word to the index in the vector
        index++;  // Increment the index for the next word
        return;
    }

    string vowels = "eiu";
    string consonants = "bknrs'";

    // First syllable can be just a vowel
    if (firstSyllable) {
        for (char vowel : vowels) {
            generateWordsRecursive(numSyllables - 1, currentWord + vowel, words, false, index);
        }
    } else {
        // Subsequent syllables must be consonant + vowel
        for (char consonant : consonants) {
            for (char vowel : vowels) {
                generateWordsRecursive(numSyllables - 1, currentWord + consonant + vowel, words, false, index);
            }
        }
    }
}

Vector<string> allRecursianWords(int numSyllables) {
    // Handle invalid input
    if (numSyllables < 0) {
        error("Number of syllables cannot be negative.");
    }

    // Special case for 0 syllables (empty word)
    if (numSyllables == 0) {
        return {""};
    }

    // Calculate the total number of words based on the number of syllables
    int totalWords = 0;
    for (int i = 1; i <= numSyllables; i++) {
        totalWords += (i == 1) ? 21 : 21 * 6 * 3 * (i - 1);
    }

    // Allocate the vector to hold all the words
    Vector<string> words(totalWords);
    int index = 0; // Start from the first position in the vector
    generateWordsRecursive(numSyllables, "", words, true, index);

    return words;
}


