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

    GEN select_prime()
    {
        GEN prime_candidate = limit;
        while (!isprime(prime_candidate))
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
        // int randomIndex = rand() % primitiveRoots.size();
        return primitiveRoots[0];
    }

    
    GEN select_d()
    {
        GEN res = stoi(5);
        // GEN res = stoi(rand());

        // res = gmod(res, gsub(p, stoi(2))); 
        return res;
    }

    
    
    GEN mod_exp(GEN base, GEN exp, GEN mod)
    {
    
        GEN result = gpow(base, exp, DEFAULTPREC);

    
        result = gmod(result, mod);

        return result;
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
        privateKey = gadd(d, stoi(1));               
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

    
    GEN mod_inv(GEN a, GEN p)
    {
        GEN m = p, n = a;
        GEN x1 = stoi(1), x2 = stoi(0), q, temp;

        while (gcmp(n, stoi(0)) != 0)
        {
            q = gdiv(m, n);
            temp = n;
            n = gmod(m, n);
            m = temp;

            temp = x1;
            x1 = gsub(x2, gmul(q, x1));
            x2 = temp;
        }

        if (gcmp(m, stoi(1)) != 0)
        {
            cout << "Error: No modular inverse exists for " << gtolong(a) << " mod " << gtolong(p) << endl;
            exit(1);
        }

        return gmod(x1, p); 
    }

    GEN decryptMessage(GEN r, GEN t)
    {
        GEN r_d = gpow(r, privateKey, DEFAULTPREC); 
        r_d = gmod(r_d, p);

        GEN r_d_inv = mod_inv(r_d, p);
        GEN message = gmul(t, r_d_inv);
        message = gmod(message, p);

        return message;
    }
};
