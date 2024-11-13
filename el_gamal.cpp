#include <iostream>
#include <pari/pari.h>
#include <cmath>
#include <vector>
#include <cstdlib>

using namespace std;

class ElGamalCryptosystem
{
public:
    GEN p, alpha, beta, limit, d, privateKey;

    bool isPrime(GEN number)
    {
        long num = gtolong(number);
        if (num <= 1)
            return false;
        if (num == 2 || num == 3)
            return true;
        if (num % 2 == 0 || num % 3 == 0)
            return false;
        long sqrt_num = (long)sqrt(num);
        for (long i = 5; i <= sqrt_num; i += 6)
        {
            if (num % i == 0 || num % (i + 2) == 0)
                return false;
        }
        return true;
    }

    GEN select_prime()
    {
        GEN prime_candidate = limit;
        while (!isPrime(prime_candidate))
        {
            prime_candidate = gsub(prime_candidate, stoi(1));
        }
        return prime_candidate;
    }

    bool isPrimitiveRoot(GEN alpha, GEN p)
    {
        long n = gtolong(p) - 1;
        for (long i = 1; i <= sqrt(n); i++)
        {
            if (n % i == 0)
            {
                GEN pow1 = gpow(alpha, stoi(n / i), DEFAULTPREC);
                if (gcmp(pow1, stoi(1)) == 0)
                {
                    return false;
                }
            }
        }
        return true;
    }

    GEN select_alpha()
    {
        vector<GEN> primitiveRoots;
        GEN alpha_candidate = stoi(2);

        while (gcmp(alpha_candidate, p) < 0)
        {
            if (isPrimitiveRoot(alpha_candidate, p))
            {
                primitiveRoots.push_back(alpha_candidate);
            }
            alpha_candidate = gadd(alpha_candidate, stoi(1));
        }

        if (primitiveRoots.empty())
        {
            cout << "Error: No primitive root found!" << endl;
            exit(1);
        }
        srand(time(NULL));
        int randomIndex = rand() % primitiveRoots.size();
        return primitiveRoots[randomIndex];
    }

    GEN select_d()
    {
        GEN res = stoi(rand());
        res = gmod(res, gsub(p, stoi(2)));
        return res;
    }

    GEN mod_exp(GEN a, GEN b, GEN n)
    {
        GEN d = stoi(1);
        GEN base = a;
        GEN exp = b;

        vector<GEN> bin;

        if (gcmp(exp, stoi(0)) == 0)
        {
            return d;
        }

        while (gcmp(exp, stoi(0)) != 0)
        {
            if (gcmp(exp, stoi(0)) == 0)
            {
                break;
            }
            bin.push_back(gmod(exp, stoi(2)));
            exp = gdiv(exp, stoi(2));
        }

        for (int i = bin.size() - 1; i >= 0; i--)
        {
            d = gmul(d, d);
            d = gmod(d, n);

            if (gcmp(bin[i], stoi(1)) == 0)
            {
                d = gmul(d, base);
                d = gmod(d, n);
            }
        }

        return d;
    }

    ElGamalCryptosystem(int power)
    {
        pari_init(10000000000, 2);

        GEN two = stoi(2);
        GEN exp = stoi(power);
        limit = gpow(two, exp, DEFAULTPREC);

        p = select_prime();
        alpha = select_alpha();

        d = select_d();

        privateKey = gadd(d, two);
        beta = gpow(alpha, privateKey, DEFAULTPREC);
        beta = gmod(beta, p);
    }

    void printKeys()
    {
        cout << "Public Key: (p = " << gtolong(p) << ", alpha = " << gtolong(alpha)
             << ", beta = " << gtolong(beta) << ")\n";
        cout << "Private Key: d = " << gtolong(privateKey) << "\n";
    }

    pair<GEN, GEN> encryptMessage(GEN message)
    {
        GEN k = select_d(); 
        
        GEN r = gpow(alpha, k, DEFAULTPREC);
        r = gmod(r, p);

        GEN beta_k = gpow(beta, k, DEFAULTPREC);
        beta_k = gmod(beta_k, p);
        GEN t = gmul(beta_k, message);
        t = gmod(t, p);

        return make_pair(r, t);
    }
};
