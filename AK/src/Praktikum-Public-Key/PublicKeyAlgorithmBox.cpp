#include <iostream>
#include "nbtheory.h"
#include "osrng.h"
#include "PublicKeyAlgorithmBox.h"

using namespace CryptoPP;
using namespace std;

// #EEA()
bool PublicKeyAlgorithmBox::EEA(const Integer& a, const Integer& b,
		Integer& d, Integer& x, Integer& y) {

    // Es gilt gcd(a,0) = |a|. Da a laut Vorgabe in Doxygen eine positive Zahl
    // ist, ist gcd(a,0) = a.
    if (b == 0) {
        d = a;
        // Für {ax + by | x, y ∈ Z} ist a = gcd(a,b) und somit x = 1 und y = 0.
        x = 1;
        y = 0;
        // Wenn (d =) gcd(a,b) == 1, dann wird True zurückgeliefert.
        return d == 1;
    }

    // Berechnen der Argumente für den rekursiven Aufruf von EEA.
    Integer a2 = b;
    Integer b2 = a % b;
    Integer d2, x2, y2;

    EEA(a2, b2, d2, x2, y2);

    // Werte des rekursiven Aufruf von EEA werden ausgelesen und
    // in den Zielvariablen gespeichert.
    d = d2;
    x = y2;
    y = x2 - a / b * y2;

    // Wenn (d =) gcd(a,b) == 1, dann wird True zurückgeliefert.
    return d == 1;
} // EEA()

// #modularExponentation()
Integer PublicKeyAlgorithmBox::modularExponentation(const Integer& a,
		const Integer& b, const Integer& n) {

    Integer d = 1;

    // Bei der Berechnung der modularen Exponentation wird die Binärdarstellung
    // des Exponenten von b verwendet. Die Binärdarstellung kann über die
    // GetBit() Funktion der Integer Klasse verwendet werden. Über alle Bits
    // kann mit eine Schleife von Bit b.BitCount()-1 bis 0 iteriert werden.
    for (int i = b.BitCount()-1; i >= 0; i--) {
        // In jedem Schleifendurchlauf wird d quadriert. Die Multiplikation
        // modulo n ist assoziativ. Dadurch kann der Zwischenwert nach jedem
        // Zwischenschritt mod n gerechnet werden. Dadurch werden die Zwischen-
        // werte nicht zu groß und die Berechung wird dadurch effizienter.
        d = (d * d) % n;
        // Wenn ein Bit in b gleich 1 ist, dann wird d ebenfalls mit a
        // multipliziert.
        if (b.GetBit(i)) {
            d = (d * a) % n;
        }
    }

    // Mittels der in nbtheory.h deklarierten Methode a_exp_b_mod_c(a,b,n) kann
    // überprüft werden, ob die implementierung des Algorithmus korrekt
    // implementiert ist. Dieser Check kann für bessere Performance entfernt
    // werden.
    assert(d == a_exp_b_mod_c(a, b, n));

    return d;
} // modularExponentation()

// #multInverse()
bool PublicKeyAlgorithmBox::multInverse(const Integer& a, const Integer& n,
		Integer& a_inv) {

    Integer d, x, y;

    // a ist invertierbar modulo n, falls gcd(a,n) = 1. gcd(a,n) kann auch mit
    // dem Erweiterten Algorithmus von Euklid berechnet werden.
    // Wegen ZTK Satz 3.4 gibt es x,y ∈ Z, so dass gcd(a, n) = 1 = ax + ny.
    // Somit gilt 1 ist kongruent zu ax+ny (mod n). Wegen mod n kürzt sich das
    // ny raus und ist somit das gleiche wie 1 ist kongruent zu ax (mod n).
    // Dadurch ist x das multiplikative Inverse von a mod n und dieses x kann
    // ebenfalls durch den EEA berechnet werden.
    EEA(a, n, d, x, y);

    // Bei EEA wird gcd(a,n) in d gespeichert.
    if (d == 1) {
        // Laut Aufgabenstellung sollen keine negativen inversen zurückgegeben
        // werden. Da -1 (mod n) = n-1 (mod n), kann ein berechnetes negatives
        // Inverses zu einer positiven Inversen umgewandelt werden.
        if (x < 0) x += n;
        // Das multiplikative Inverse von a wird über a_inv zurückgegeben.
        a_inv = x;
        // True wird als Rückgabewert zurückgegeben, da ein multiplikatives
        // Inverses existiert.
        return true;
    }
    // gcd(a,n) != 1, das heißt, dass es kein inverses Element gibt.
    // Dadurch wird false zurückgegeben.
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
    // Ein nicht blockierender Pseudozufallszahlengenerator wird für das erzeugen
    // von n.BitCount() Bit großen Zahlen verwendet.
    NonblockingRng nonblockingRng;
    Integer ret;
    do {
        // Erzeugt einen zufälligen Integer im Bereich 0,1,n-1. Wenn ein Integer
        // > n-1 gewürfelt worden ist, dann muss nochmal gewürfelt werden.
        ret.Randomize(nonblockingRng, n.BitCount());
    } while (ret >= n);
    return ret;
}

// #millerRabinTest()   // TODO: copy pasted too
bool PublicKeyAlgorithmBox::millerRabinTest(Integer& n, unsigned int s) {
    //Impementiert den Miller-Rabin Primzahltest, der feststellen kann, ob es sich bei n um eine Primzahl oder eine
    //Zusammengesetze Zahl handelt

    //Sicherstellen, dass der Qualitätsparameter s größer gleich 0 ist
    assert(s >= 0);

    //Wenn die Zahl gerade ist (also n % 2 = 0), dann gib False zurück. Die Zahl 2 ist die einzige gerade Primzahl.
    // Auf 2 wird deshalt speziell tetestet.
    if (n == 2) return true;
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
    // Verwendung eines nicht blockierenden Pseudozufallszahlengenerators
    // zum erzeugen von Zufallszahlen.
    NonblockingRng nonblockingRng;

    // anzahlVersuche gibt die Anzahl der bisher generierten Zufallszahlen an,
    // bis eine Zufallszahl gefunden wurde, die mit Wahrscheinlichkeit 1-2^-s
    // eine Primzahl ist.
    int anzahlVersuche = 0;

    // Es wird so lange eine Zufallszahl p in {0,1,2^bitlen-1} generiert,
    // bis p >= 2 und mit Wahrscheinlichkeit 1-2^-s eine Primzahl ist.
    do {
        // Randomize erzeugt eine bitlen Bits große Zufallszahl mit dem
        // Pseudozufallszahlengenerator nonblockingRng und speichert die
        // erzeugte Zahl in p ab.
        p.Randomize(nonblockingRng, bitlen);
        // Für jeden Versuch wird anzahlVersuche um 1 erhöht.
        anzahlVersuche++;
    } while (p < 2 || !millerRabinTest(p, s));
    // Die Anzahl der bisher generierten Zufallszahlen an,
    // bis eine Zufallszahl gefunden wurde, die mit Wahrscheinlichkeit 1-2^-s
    // eine Primzahl ist, wird zurückgegeben.
    return anzahlVersuche;
} // randomPrime()

// #randomPrime()
unsigned int PublicKeyAlgorithmBox::randomRabinPrime(Integer &p,
		unsigned int bitlen, unsigned int s) {

    // Ein nicht blockierender Pseudozufallszahlengenerator wird für das
    // erzeugen von Zufallszahlen verwendet.
    NonblockingRng nonblockingRng;

    // In jedem Schleifendurchlauf wird eine Zufallszahl mit maximaler Bitlänge
    // bitlen erzeugt. Wenn gilt, dass p % 4 != 3 und
    // laut dem Rabin Miller Test mit einer Wahrscheinlichkeit von 1-2^-s
    // p eine Primzahl ist, dann wird die Schleife und die Funktion verlassen.
    // Ist eine der beiden Bedingungen nicht erfüllt, dann wird der nächste
    // Schleifendurchlauf ausgeführt, die Schleife wird also nicht verlassen.
    do {
        p.Randomize(nonblockingRng, bitlen);
    } while (p % 4 != 3 || !millerRabinTest(p, s));

    return 0;
} // randomRabinPrime()

// #modPrimeSqrt()
bool PublicKeyAlgorithmBox::modPrimeSqrt(const Integer& y, const Integer& p,
		vector<Integer>& v) {

    // Diese Methode berechnet nur die Quadratwurzeln, wenn p kongruent zu 3
    // modulo 4 ist. Ist das nicht der Fall, dann wird false zurückgegeben.
    if (p % 4 != 3) return false;

    // Laut Vorgabe wird angenommen, dass p eine Primzahl ist.
    // Es gilt, dass wenn p eine Primzahl mit der Eigenschaft ist, dass
    // p kongruent zu 3 modulo 4 ist, dann sind x1,2 = +/- a^((p+1)/4) die
    // beiden Lösungen für die Gleichung x² ist kongruent zu a (mod p). Dabei
    // ist a ein quadratischer Rest modulo p und in unserem Fall ist a der
    // Übergabeparameter y und x bzw die 2 x's sind die zu suchenden
    // Quadratwurzeln.
    // p kongruent zu 3 modulo 4 ist durch obige p % 4 == 3 abfrage gegeben.
    // x1,2 = +/- a^((p+1)/4)
    Integer x = modularExponentation(y, (p+1)/4, p);
    v.clear();
    // Der plus Fall
    v.push_back(x);
    // Der minus Fall. Es gilt -x (mod p) = -x + p (mod p)
    // Durch die Testwerte aus der Aufgabenstellung wurde angenommen, dass hier
    // die zwei kleinsten POSITIVE Quadratwurzeln zurückgegeben werden soll.
    v.push_back(-x + p);

    // Die Quadratwurzeln konnten berechnet werden. Dadurch wird True
    // zurückgegeben.
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

    // Gesucht ist eine nicht-negativen ganzzahligen Quadratwurzel s von x.
    // Mit anderen worten wird ein s gesucht, wobei s² = x ist, wobei s >= 0.
    // Hier kann die Binären Suche eingesetzt werden, weil die möglichen
    // Kandidaten von s = 0,1,2,...,x/2 auch quadriert sortiert sind.
    // z.b. ist 2² < 3² < 4² < ....
    // Angenommen x = 100, dann wird min auf 0 und max auf 50 gesetzt.
    // Weiter gilt, dass 0² <= x <= 50²
    // Für s gilt: 0 <= s <= 50. Eine Möglichkeit
    // wäre, alle s von 0 bis 50 durchzuprobieren. Dieser Alogorithmus hätte
    // allerdings lineare Laufzeit. Da die Kandidaten sortiert sind, kann
    // auch die Binäre Suche als Algorithmus mit Logarithmischer Laufzeit
    // verwendet werden.
    Integer min = 0;
    Integer max = x / 2;

    while (min < max) {
        // In jedem Schleifendurchlauf wird die Mitte von min und max bestimmt.
        // Der Suchraum wird dadurch in der Mitte halbiert.
        Integer mid = (min + max) / 2;
        // Zuerst wird überprüft, ob diese Zahl die gesuchte Zahl s ist.
        Integer midSqrt = mid * mid;
        if (midSqrt == x) {
            // und wenn sie es ist, dann wird s auf mid gesetzt und true
            // zurückgegeben.
            s = mid;
            return true;
        }
        // Spezieller Fall wenn der Suchraum nur noch 2 Elemente groß ist.
        if (max-min == 1) {
            // Wenn dieser Basic Block ausgeführt wird, dann ist
            // min+1 = max und mid ist immer min, weil bei (min + max) / 2
            // abgerundet wird. Da max auf mid - 1 gesetzt wird, wurde max² == x
            // bisher noch nicht getestet.
            if (max * max == x) {
                s = max;
                return true;
            }
            // Wenn dieser Basic Block ausgeführt wird, dann ist
            // min² < x und max² > x
            // Da allerdings min+1 = max ist, ist das gesuchte mid, wobei
            // mid²=x ist, eine Gleitkommazahl. Laut der Aufgabenstellung
            // soll nur nach Ganzzahligen Quadratwurzeln gesucht werden.
            // Das ist also nicht der Fall und es wird false zurückgegeben.
            return false;
        }

        // Im Beispiel oben galt Für s gilt: 0 <= s <= 50.
        // Da min=0 und max=50, ist mid = 25 und 25² = 2500.
        // 2500 ist größer als das gesuchte 100. Weil sortiert ist, ist
        // für alle s >= 25 s² > x. Dadurch fallen alle Kandidaten >= 25 weg.
        // Das Setzten von max auf mid-1, also 24, stellt das dar.
        // Übrig bleiben die Kandidaten 0 <= s <= 24. In jedem Schleifendurch-
        // lauf werden die Möglichkeiten für s halbiert, wodurch logarithmische
        // Laufzeit erreicht wird.
        // Wäre mid² < x, dann wäre das gesuchte s im Bereich mid, mid+1,..,max,
        // was durch das Setzten der Grenze min auf mid markiert wird.
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
    // Zwei bitlen-Bit große Zahlen, die jeweils mit Wahrscheinlichkeit
    // 1-2^-s eine Primzahl ist, werden erzeugt und in p und q abgespeichert.
    // Wenn p und q gleich sind, dann könnte N sehr einfach faktorisiert
    // werden, indem die Wurzeln von N berechnet wird. Für die Sicherheit
    // von N darf N nicht einfach faktorisierbar sein.
    // Deshalb wird sichergestellt, dass p != q.
    do {
        randomPrime(p, bitlen, s);
        randomPrime(q, bitlen, s);
    } while(p == q);
    Integer n = p * q;
    // phiN gibt die Anzahl der Zahlen an, die Teilerfremd zu n sind.
    Integer phiN = (p-1) * (q-1);
    do {
        e = randomInteger(phiN-2) + 1;
    } while (Integer::Gcd(e, phiN) != 1);
    multInverse(e, phiN, d);
}

