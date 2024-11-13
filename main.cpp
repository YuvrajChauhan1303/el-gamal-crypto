#include <iostream>
#include <pari/pari.h>
#include <cstdlib>
#include <stdexcept>

#include "el_gamal.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Error: Missing argument for power.\n";
        cerr << "Usage: " << argv[0] << " <power>\n";
        return 1;
    }

    int power = stoi(argv[1]);

    ElGamalCryptosystem obj(power);

    obj.printKeys();
    GEN message = stoi(513); 

    pair<GEN, GEN> encrypted = obj.encryptMessage(message);

    cout << "Encrypted Message: (r = " << gtolong(encrypted.first) << ", t = " << gtolong(encrypted.second) << ")\n";

    return 0;
}