#include <iostream>
#include <pari/pari.h>
#include <cstdlib>
#include <stdexcept>

#include "el_gamal.cpp"

using namespace std;


int main()
{
    int power = 10;  // Define the power of 2 to generate prime.
    ElGamalCryptosystem elGamal(power);

    elGamal.printKeys();  // Display the generated keys.

    // Encrypt a sample message.
    GEN message = stoi(42);  // Some sample message (as a number).
    pair<GEN, GEN> encryptedMessage = elGamal.encryptMessage(message);

    cout << "Encrypted Message: (r = " << gtolong(encryptedMessage.first)
         << ", t = " << gtolong(encryptedMessage.second) << ")\n";

    // Decrypt the message.
    GEN decryptedMessage = elGamal.decryptMessage(encryptedMessage.first, encryptedMessage.second);
    cout << "Decrypted Message: " << gtolong(decryptedMessage) << endl;

    return 0;
}