#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sodium.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAX 80
#define PORT 4000
#define SA struct sockaddr
#define TOKEN_SIZE 128
#define PAYLOAD_SIZE (crypto_secretbox_MACBYTES + TOKEN_SIZE)
#define MSG_ASK "Can I get the solution to the challenge, please?"
#define STATUS_BAD 0
#define STATUS_GOOD 1

// Define the message structure
struct message {
    int hacker_id;
    int status;
    unsigned char nonce[crypto_secretbox_NONCEBYTES];
    unsigned char payload[PAYLOAD_SIZE];
};

// Function to read the encryption key from a file
void read_key(char* key, const char* filename) {
    FILE *file_ptr = fopen(filename, "r");
    if (file_ptr == NULL) {
        fprintf(stderr, "Failed to open key file.\n");
        exit(1);
    }
    
    for (int i = 0; i < crypto_secretbox_KEYBYTES; i++) {
        int ch = fgetc(file_ptr);
        if (ch == EOF) {
            fprintf(stderr, "Key file too short.\n");
            fclose(file_ptr);
            exit(1);
        }
        key[i] = (char) ch;
    }

    fclose(file_ptr);
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cli;

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Socket creation failed...\n");
        exit(0);
    } else {
        printf("Socket successfully created..\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    // Assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.77");
    servaddr.sin_port = htons(PORT);

    // Connect to server
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    } else {
        printf("Connected to the server..\n");
    }

    char key[crypto_secretbox_KEYBYTES];
    read_key(key, "key");  // Read encryption key

    struct message M1, M2;
    struct message *ptr1 = &M1;
    struct message *ptr2 = &M2;

    char tbuff[TOKEN_SIZE] = MSG_ASK;

    M1.hacker_id = 23;
    randombytes_buf(M1.nonce, crypto_secretbox_NONCEBYTES);

    crypto_secretbox_easy(M1.payload, tbuff, TOKEN_SIZE, M1.nonce, key);
    printf("\n%s\n", M1.payload);

    send(sockfd, ptr1, sizeof(struct message), 0);
    recv(sockfd, ptr2, sizeof(struct message), 0);
    printf("%d\n", M2.status);

    unsigned char decrypted[TOKEN_SIZE];
    if (crypto_secretbox_open_easy(decrypted, M2.payload, PAYLOAD_SIZE, M2.nonce, key) != 0) {
        printf("\nMessage forged!\n");
    } else {
        printf("\nSuccess Code\n");
    }

    unsigned char hashing[crypto_generichash_BYTES];
    crypto_generichash(hashing, crypto_generichash_BYTES, decrypted, TOKEN_SIZE, NULL, 0);

    // Base64 encoding requires the correct function to compute encoded length
    // Ensure you have the right function or replace with a correct value
    int binary_length = TODO_compute_base64_encoded_length();
    char b64[binary_length];
    // Ensure you have the right variant or replace with a correct value
    sodium_bin2base64(b64, binary_length, hashing, crypto_generichash_BYTES, TODO_variant);
    printf("%s", b64);

    close(sockfd);  // Close the socket
}
