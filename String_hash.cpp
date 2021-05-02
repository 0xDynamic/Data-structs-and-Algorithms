#include <cassert>
#include <string>
#include <tuple>

const int MAX_N = 10;
const int NUM_H = 2;                                    // Number of hashes generated
																		// per string.
typedef long long ll;

ll base[NUM_H] = {131, 137};                            // Each string is hashed
ll mod         = 1e9 + 7;                               			// against NUM_H primes.

ll P[NUM_H][MAX_N];                                     // Polynomial powers.
ll H[NUM_H][MAX_N];                                     // Prefix hashes.

ll n;                                                   // Length of current string
														// being hashed.
bool init_is_complete = false;

ll modulo(ll value)
{
    return (value + mod) % mod;
}

ll extEuclid(ll a, ll b, ll &x, ll &y) {                // Returns gcd(a, b).
    ll xx = y = 0;
    ll yy = x = 1;

    while (b != 0)
    {
      ll q = a/b;
      std::tie(a, b) = std::tuple<ll,ll>(b, a%b);
      std::tie(x, xx) = std::tuple<ll,ll>(xx, x-q*xx);
      std::tie(y, yy) = std::tuple<ll,ll>(yy, y-q*yy);
    }

    return a;
}

ll mod_inverse(ll b)                                    // Returns b^(-1) under mod.
{
    ll x, y;

    ll d = extEuclid(b, mod, x, y);                  		   // b*x + m*y == d

    if (d != 1) assert(false);                         			 // No multiplicative inverse.

    return modulo(x);                                		   // b*x == 1 under mod.
}

ll get_phash(ll p[], ll h[], int l, int r)             	 // Returns the hash of the
{                                                       						// substring S[l, r].
    if (l == 0) return h[r];

    ll partial_hash = modulo(h[r] - h[l-1]);
    partial_hash *= mod_inverse(p[l]);

    assert(0 <= partial_hash);                          	// Overflow detection.

    return modulo(partial_hash);
}

ll get_hash(int L = 0, int R = n - 1)
{
    assert(0 <= L && L <= R && R < n);                  // At any moment, only hashes
																					// up till H[n-1] are valid.
    ll complete_hash = 0;

    for (int i = 0; i < NUM_H; i += 1)                  			// The actual hash is a
    {                                                   								// superposition of NUM_H
        ll partial_hash = get_phash(P[i], H[i], L, R);  // partial hashes.

        complete_hash <<= (30 / (NUM_H - 1));
        complete_hash |=  partial_hash;
    }

    return complete_hash;
}

void init()
{
    for (int i = 0; i < NUM_H; i += 1)
        for (int j = 0; j < MAX_N; j += 1)
            if (j == 0) P[i][j] = 1;
            else P[i][j] = modulo(P[i][j-1] * base[i]);

    init_is_complete = true;
}

ll Hash(const std::string &S)
{
    n = (int)S.length();

    if (init_is_complete != true) init();               // Hashing can only be
																				// performed after powers
    for (int i = 0; i < NUM_H; i += 1)                  	// have been initialized.
        for (int j = 0; j < n; j += 1)
        {
            H[i][j] =  (j == 0) ? 0 : H[i][j-1];
            H[i][j] += (ll)+S[j] * P[i][j];
            H[i][j] =  modulo(H[i][j]);
        }

    return get_hash();
}