# Encrypted Client-Server Communication

This C program demonstrates a client-server communication scenario, where a client connects to a server, sends an encrypted message, receives an encrypted response, decrypts it, and then computes and prints a hash of the decrypted message. The program utilizes the Sodium library for encryption, decryption, and hashing functionalities.

## Dependencies

- Sodium library: A modern, easy-to-use software library for encryption, decryption, signatures, password hashing and more.

```bash
# On Ubuntu, you can install libsodium using the following command:
sudo apt-get install libsodium-dev
```

## Compilation

```bash
gcc -o client client.c -lsodium
```

## Usage

1. Ensure the server is running and listening on IP `192.168.1.77` and port `4000`.
2. Place your encryption key in a file named `key` in the same directory as your compiled program.
3. Run the compiled program.

```bash
./client
```

## Program Flow

1. **Socket Creation and Configuration**: A TCP socket is created and configured for communicating with the server.
2. **Reading Encryption Key from File**: An encryption key is read from a file named "key".
3. **Preparing Message**: An instance of `message` structure is prepared with a `hacker_id`, a randomly generated nonce, and an encrypted version of a request message.
4. **Sending and Receiving Messages**: The prepared message is sent to the server, and a response message is received from the server.
5. **Decrypting Response**: The payload of the received message is decrypted.
6. **Computing Hash**: A hash of the decrypted payload is computed.
7. **Base64 Encoding**: The hash is then base64 encoded and printed to the console.
8. **Closing Socket**: Finally, the socket is closed.

## Troubleshooting

Ensure that the `sodium` library is properly installed and the server is up and running. If you face any issues with decryption, ensure that the encryption key file named `key` is correctly formatted and located in the same directory as your compiled program. 

---

This README provides a high-level overview of the program's functionality and instructions on how to compile and run the program.
