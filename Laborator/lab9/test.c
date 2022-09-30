#include <stdio.h>
#include <unistd.h>

#include "huffman.h"

#define ASSERT(cond, msg) if (!(cond)) { failed(msg); } else { passed(msg); }
void passed(char* s) {
    printf("Testul %s a fost trecut cu succes!\t\t \n", s);
    usleep(500000);
}

void failed(char* s) {
    printf("Testul %s NU a fost trecut!\n", s);
    exit(0);
}


char ref1[5][200] = {"0", "100011101111", "000110", "11100001010101010011", 
                    "011101000010111110000111101100101011001110110110010100011110101110001011101100111010110101110100111010101"};
char ref2[5][200] = {"1", "100010", "1011011100110", "101110111011100100101001011000101010010", 
                    "111101111000111100010010101111010110000011101000001110001011000110110011101010010011101101000100"};


void runSimpleTest() {
    int freqs[256] = {0};
    char **codes = (char **)calloc(256, sizeof(char *));

    char *huffmanText = "ana are mere";

    computeFreqs(huffmanText, freqs);
    ASSERT(freqs[97] == 3 && freqs[101] == 3 
        && freqs[114] == 2 && freqs[32] == 2
        && freqs[109] == 1 && freqs[110] == 1
        && freqs[111] == 0 && freqs[116] == 0, "Simple test - computeFreqs");

    PHuffmanNode root = makeTree(freqs);
    ASSERT(root && root->left && root->right 
        && root->left->right && root->left->right->value == 'e'
        && root->right->left && root->right->left->value == 'a'
        && root->right->right->right->left && root->right->right->right->left->value == 'm', "Simple test - makeTree");

    makeCodes(root, codes);
    ASSERT(codes[97] && strcmp(codes[97], "10") == 0
        && codes[32] && strcmp(codes[32], "110") == 0
        && codes[114] && strcmp(codes[114], "00") == 0
        && codes[110] && strcmp(codes[110], "1111") == 0
        && !codes[111] && !codes[0] && !codes[65], "Simple test - makeCodes");
}


void runTest(char *huffmanText, char *textToEncode, char *textToDecode, char *testName, char* ref1, char* ref2) {
    int freqs[256] = {0};
    char **codes = (char **)calloc(256, sizeof(char *));

    computeFreqs(huffmanText, freqs);
    PHuffmanNode root = makeTree(freqs);
    makeCodes(root, codes);
    char *compressed1 = compress(textToEncode, codes);
    char *decompressed1 = decompress(compressed1, root);
    char *compressed2 = compress(textToDecode, codes);
    char *decompressed2 = decompress(compressed2, root);
    printf("\n%s: %s\n\tSent: %s (%s)\n\tReceived: %s (%s)\n", testName, huffmanText, 
                textToEncode, compressed1, decompressed2, compressed2);
    ASSERT(strcmp(compressed1, ref1) == 0 && strcmp(compressed2, ref2) == 0 
        && strcmp(textToDecode, decompressed2) == 0 && strcmp(textToEncode, decompressed1) == 0, testName);
}

int main() {
    runSimpleTest();
    runTest("ababab", "a", "b", "Test-01", ref1[0], ref2[0]);
    runTest("hello world", "lord", "lol", "Test-02", ref1[1], ref2[1]);
    runTest("ana are mere", "rea", "a mea", "Test-03", ref1[2], ref2[2]);
    runTest("ala bala portocala?!", "acolo?", "la laborator", "Test-04", ref1[3], ref2[3]);
    runTest("this is a clear and obvious example of a huffman tree!", 
            "am luat nota mare la examen", "ce bine! am avut emotii", "Test-05", ref1[4], ref2[4]);

    return 0;
}

