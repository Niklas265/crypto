#include <iostream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
bool PublicKeyAlgorithmBox::EEA(const Integer& a, const Integer& b,
		Integer& d, Integer& x, Integer& y) {

    if (b == 0) {
        d = a;
        x = 1;
        y = 0;
        return true;
    }

    Integer a2 = b;
    Integer b2 = a % b;
    Integer d2, x2, y2;

    EEA(a2, b2, d2, x2, y2);

    d = d2;
    x = y2;
    y = x2 - a / b * y2;

    return d == 1;
} // EEA()

// #modularExponentation()
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer& a,
		const Integer& b, const Integer& n) {

    Integer c = 0;
    Integer d = 1;

    for (int i = b.BitCount()-1; i >= 0; i--) {
        c *= 2;
        d = (d * d) % n;
        if (b.GetBit(i)) {
            c += 1;
            d = (d * a) % n;
        }
    }

    return d;
} // modularExponentation()

// #multInverse()
bool PublicKeyAlgorithmBox::multInverse(const Integer& a, const Integer& n,
		Integer& a_inv) {

    Integer d, x, y;

    EEA(a, n, d, x, y);

    if (d == 1) {
        if (x < 0) x += n;
        a_inv = x;
        return true;
    }
    return false;
} // multInverse()

// #witness()   // TODO: copy pasted from Praktikum-Primzahlen
bool PublicKeyAlgorithmBox::witness(const Integer& a, const Integer& n) {
    //Implementierung des Witness-Algorithmus, der True zurückliefert, wenn n keine Primzahl ist

    //sicherstellen, dass n ungerade Zahl ist
    assert((n%2)==1);

    //Berechne u und r so dass n-1 = u * 2^r
    Integer nMinus1 = n-1;
    Integer u,r;
    Integer powerof2;

    //Für jedes Bit in der Binärdarstellung von n-1
    for(int i = 0; i < nMinus1.BitCount(); i++) {
        //wenn das Bit an der i-ten Position 1 ist, dann Berechne u = (n-1)/ 2^i
        if(nMinus1.GetBit(i)) {
            powerof2 = Integer::Power2(i);
            r = i;
            u = nMinus1 / powerof2;
            //wenn alle Kriterien für u und r erfüllt sind und die Division in der obigen Zeile ohne Rest
            //durchgeführt werden konnte, dann brich die Schleife ab, da r und u gefunden wurde, sodass die Bedingung
            //erfüllt ist
            if(((nMinus1 % powerof2) == 0) && ((u % 2) == 1)) {
                break;
            }
        }
    }

    Integer d = modularExponentation(a,u,n);
    //Führe in jedem Schleifendurchlauf eine Quadratur von d durch, um a^(n-1) (mod n) zu berechnen
    for(Integer i = 0; i < r; i++) {
        Integer x = d;
        d = (d*d) % n;
        //wenn d=1 und x !=1 und x != -1 ist, dann gib True zurück, da n keine Primzahl sein kann, wenn n eine
        //Quadratwurzel in Z Stern n besitzt, die ungleich 1 und -1 (also n-1) ist
        if((d==1) && (x != 1) && (x != (n-1))) {
            return true;
        }
    }
    //wenn das Ergebnis der Berechnung a^(n-1) % n ungleich 1 ist (das Fermat-Kriterium also nicht erfüllt ist), dann gib
    //True zurück, da n dann keine Primzahl ist
    if(d != 1) {
        return true;
    }

    return false;
} // witness()

// randomInteger()
Integer PublicKeyAlgorithmBox::randomInteger(const Integer& n) {
    NonblockingRng nonblockingRng;
    Integer ret;
    do {
        ret.Randomize(nonblockingRng, n.BitCount());
    } while (ret > n);  // TODO: check bounds, according to docu n is included
                        //  in possible values for ret
    return ret;
}

// #millerRabinTest()   // TODO: copy pasted too
bool PublicKeyAlgorithmBox::millerRabinTest(Integer& n, unsigned int s) {
    //Impementiert den Miller-Rabin Primzahltest, der feststellen kann, ob es sich bei n um eine Primzahl oder eine
    //Zusammengesetze Zahl handelt

    //Sicherstellen, dass die Zahl n > 2 und der Qualitätsparameter s größer gleich 0 ist
    assert(n > 2);
    assert(s >= 0);

    //Wenn die Zahl gerade ist (also n % 2 = 0), dann gib False zurück. Die Zahl 2 ist die einzige gerade Primzahl.
    //Da aber sichergestellt wird, dass die zu testende Zahl größer als 2 ist, kann geschlussfolgert werden,
    //dass es sich sicher um keine Primzahl handelt, wenn n gerade ist
    if((n % 2) == 0) {
        return false;
    }
    //Durchlaufe die Schleife s-mal
    for(int i = 0; i < s; i++) {
        //Die zufällig erzeugte Zahl a muss in range 1 bis n-1 liegen. Da getInteger() eine Zahl im Raum von 0 bis n-1
        //liefert, wird der Raum der erzeugten Zahlen um 1 reduziert und am Ende eins aufaddiert. So ist
        //die kleinstmöglich erzeugbare Zahl 0+1 = 1 und die größtmöglich erzeugbare Zahl n-2+1 = n-1. Somit befindet
        //sich die zufällig erzeugte Zahl in dem vom Algorithmus benötigten Bereich von {1,...,n-1}
        Integer a = randomInteger(n-1)+1;

        //Wenn der größte Gemeinsame Teiler der zufällig erzeugten Zahl und n größer als 1 ist, dann ist n
        //keine Primzahl, da für eine Primzahl p gilt, dass der ggT aller Zahlen in Z Stern p (also alle Zahlen
        //zwischen 1 und p-1 (inklusiv)) gleich 1 ist
        if(Integer::Gcd(a,n) > 1) {
            return false;
        }
        //wenn die Funktion Witness true zurückgibt -> Gib false zurück, da n dann keine Primzahl ist, da hierfür ein
        //solider Beweis gefunden wurde
        if(witness(a,n)) {
            return false;
        }
    }

    return true;
} // millerRabinTest()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {
    NonblockingRng nonblockingRng;

    do {
        p.Randomize(nonblockingRng, bitlen);
    } while (!millerRabinTest(p, s));
    return 0;
} // randomPrime()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {

    NonblockingRng nonblockingRng;

    do {
        p.Randomize(nonblockingRng, bitlen);
    } while (p % 4 != 3 || !millerRabinTest(p, s));
    return 0;
} // randomRabinPrime()

// #modPrimeSqrt()
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer& y, const Integer& p,
		vector<Integer>& v) {

    if (p % 4 != 3) return false;

    Integer x = modularExponentation(y, (p+1)/4, p);
    v.clear();
    v.push_back(x);
    v.push_back(-x + p);

    return true;
}

Integer PublicKeyAlgorithmBox::euklid(const Integer& a, const Integer& b,
		vector<Integer>& q) {

    vector<Integer> r = {a, b};
    int m = 1;

    while (r[m] != 0) {
        q.push_back(r[m-1] / r[m]);
        r.push_back(r[m-1] - q[m-1]*r[m]);
        m += 1;
    }
    m -= 1;

    return r[m];
}

unsigned int PublicKeyAlgorithmBox::computeConvergents(const Integer& a,
		const Integer& b, vector<Integer>& c, vector<Integer>& d) {

    vector<Integer> q;
    euklid(a, b, q);

    c.push_back(Integer(1));
    c.push_back(q[0]);
    for (int i = 2; i <= q.size(); i++) {
        c.push_back(q[i-1] * c[i-1] + c[i-2]);
    }

    d.push_back(Integer("0"));
    d.push_back(Integer(1));
    for (int i = 2; i <= q.size(); i++) {
        d.push_back(q[i-1] * d[i-1] + d[i-2]);
    }

    return 1;
}

// #sqrt()
bool PublicKeyAlgorithmBox::sqrt(const Integer& x, Integer& s) const {

    Integer min = 1;
    Integer max = x;    // probably x/2

    while (min != max) {
        Integer mid = (min + max) / 2;
        Integer midSqrt = mid * mid;
        if (midSqrt == x) {
            s = mid;
            return true;
        }
        // can probably optimize this out
        if (max * max == x) {
            s = max;
            return true;
        }
        if (max-min == 1 && min * min != x) return false;

        if (midSqrt > x) {
            max = mid - 1;
        } else {
            min = mid;
        }
    }

    return false;
}

void PublicKeyAlgorithmBox::generateRSAParams(Integer& p, Integer& q,
		Integer& e, Integer& d, unsigned int bitlen, unsigned int s) {
    randomPrime(p, bitlen, s);
    randomPrime(q, bitlen, s);
    Integer n = p * q;
    Integer phiN = (p-1) * (q-1);
    do {
        e = randomInteger(n-2) + 1;
    } while (Integer::Gcd(e, phiN) != 1);
    multInverse(e, phiN, d);
}

