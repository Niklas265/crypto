#include <sstream>
#include <iostream>
#include <iomanip>
#include "AESMath.h"

using namespace std;

AESMath::AESMath() : exp_table(), log_table(256, 0), sbox(), inv_sbox(256, 0) {

    /*
     * Aufgabe 5a
     */

    // 3 ist das erzeugende Element

    byte a = 1;
    byte b;

    exp_table.push_back(a);

    // 254 because 256-2
    for (int i = 1; i < 256; i++) {
        // b = g^i
        b = rpmul(a,3);
        exp_table.push_back(b);
        a = b;
    }

    /*
     * Aufgabe 5b
     */

    // inverse = log
    for (int i = 0; i < 256; i++) {
        log_table[exp_table[i]] = i;
    }


    /*
     * Aufgabe 9a
     */

    // i steht für den key der sbox. Die value der sbox für key i ist eine
    // Komposition aus zwei bijektiven Abbildungen.
    // Als erstes wird dabei i auf sein multiplikatives Inverses abgebildet.
    // Das wird von inv(i) implementiert. Zweitens wird das im ersten Schritt
    // berechnete multiplikative Inverse von i mit einer in atrans berechneten
    // affinen Abbildung über GF(2) abgebildet.
    // Da der key im Wertebereich 0 bis 255 liegt, können die 256 Ergebnisse
    // der 256 möglichen Eingaben an die sbox im vorfeld berechnet, in einer
    // Lookup Tabelle sbox gespeichert und über die sBox() Funktion im weiteren
    // Programmverlauf abgerufen werden.
    // Die sbox wird als Substitution in ByteSub von AES bei der Verschlüsselung
    // verwendet.
    for(int i = 0; i < 256; i++) {
        sbox.push_back(atrans(inv(i)));
    }

    /*
     * Aufgabe 9b
     */

    // inv_sbox macht die Substitution der sbox rückgängig. inv_sbox wird
    // bei der Entschlüsselung verwendet.
    // Zum Beispiel: Angenommen sbox[1] = 10, dann ist inv_sbox[10] = 1.
    // Wie bei der sbox können die 256 Ergebnisse
    // der 256 möglichen Eingaben an die inv_sbox im vorfeld berechnet, in einer
    // Lookup Tabelle inv_sbox gespeichert und über die invSBox() Funktion im weiteren
    // Programmverlauf abgerufen werden.
    for (int i = 0; i < 256; i++) {
        inv_sbox[sbox[i]] = i;
    }
}

byte AESMath::add(byte a, byte b) {
    /*
     * Aufgabe 2
     */
    // Für jedes i = 0,1,..,7: i-te Bit in a plus i-te Bit in b
    // mod 2 ergibt das i-te Bit des Rückgabebytes.
    // Diese Rechnung wird als a XOR b implementiert.
    return a ^ b;
}

/*byte AESMath::atrans(byte x) {
    /*
     * Aufgabe 8
     */

    // 8x8 * 8x1 = 1x8

    // 1 1 1 0 1 0 1 0 = 234
    //x = 234;

    // 1 1 1 1 1 0 0 0 = 4 = 0 + 0 = 0
    // 0 1 1 1 1 1 0 0 = 3 = 1 + 1 = 0
    // 0 0 1 1 1 1 1 0 = 3 = 1 + 1 = 0
    // 0 0 0 1 1 1 1 1 = 2 = 0 + 0 = 0
    // 1 0 0 0 1 1 1 1 = 3 = 1 + 0 = 1
    // 1 1 0 0 0 1 1 1 = 3 = 1 + 0 = 1
    // 1 1 1 0 0 0 1 1 = 4 = 0 + 1 = 1
    // 1 1 1 1 0 0 0 1 = 3 = 1 + 1 = 0

    // i and AND and then parity*/

byte AESMath::atrans(byte x) {
    /*
     * Aufgabe 8
     */

    /*
     * atrans berechnet folgende affine transformation über GF(2):
     *  [y0]   [1 1 1 1 1 0 0 0]   [x0]   [1]
     *  [y1]   [0 1 1 1 1 1 0 0]   [x1]   [1]
     *  [y2]   [0 0 1 1 1 1 1 0]   [x2]   [0]
     *  [y3] = [0 0 0 1 1 1 1 1] ° [x3] ⊕ [0]
     *  [y4]   [1 0 0 0 1 1 1 1]   [x4]   [0]
     *  [y5]   [1 1 0 0 0 1 1 1]   [x5]   [1]
     *  [y6]   [1 1 1 0 0 0 1 1]   [x6]   [1]
     *  [y7]   [1 1 1 1 0 0 0 1]   [x7]   [0]
     *
     *  wobei x0 bis x7 die 8 Bits der Parameters x sind und y0 bis y7 die
     *  8 Bits des Byte Rückgabewerts sind.
     */

    // v ist eine Version der 8x8 Matrix:
    // 1 1 1 1 1 0 0 0
    // 0 1 1 1 1 1 0 0
    // 0 0 1 1 1 1 1 0
    // 0 0 0 1 1 1 1 1
    // 1 0 0 0 1 1 1 1
    // 1 1 0 0 0 1 1 1
    // 1 1 1 0 0 0 1 1
    // 1 1 1 1 0 0 0 1
    //, wobei das erste Element in v die erste Zeile darstellt. Dabei werden
    // die Spalten dieser Zeile als Bits interpretiert. Die Spalten einer Zeile
    // zusammen ergeben ein Byte. Für jede Zeile i in 0,1,2,..,7 steht v[i]
    // für dieses Byte.
    // Diese Matrix ist im Standard vorgegeben.
    vector<byte> v = { 248, 124, 62, 31, 143, 199, 227, 241 };

    // v2 ist der 8 Elemente großer Vektor, welcher auf das Ergebnis der
    // Matrixmultiplikation von x und v addiert wird und
    // danach zurückgegeben wird.
    // Dieser Vektor ist im Standard vorgegeben.
    vector<byte> v2 = { 0, 1, 1, 0, 0, 0, 1, 1 };
    // In ret wird das byte gespeichert, welches zurückgegeben wird.
    byte ret = 0;

    // Die nachfolgenden Schleifen implementieren die Matrixmultiplikation von
    // x mit v und die darauffolgende Addition, wobei diese Berechnungen über
    // GF(2) sind.

    // Für jede i-te Zeile in i = 7,6,5,...,0
    for (int i = 7; i >= 0; i--) {
        // parity(x & v[i]) implementiert einen Teil der Matrixmultiplikation,
        // einer Zeile über GF(2). Speziell wird in jedem Schleifendurchlauf
        // ein Bit des Outputbytes berechnet.
        // Bei der Matrixmultiplikation einer Zeilemit x wird jedes Bit der
        // Zeile mit jedem Bit in x multipliziert. Das wird von x & v[i]
        // implementiert. Weiter werden diese 8 Zwischenergebnisse über GF(2)
        // addiert. Dabei wird der Reihe nach jedes Bit addiert und mod 2 wegen
        // GF(2) gerechnet. Das ist das selbe wie zu überprüfen, ob die Anzahl
        // der Eines im Ergebnis gerade oder ungerade ist.
        // Das wird von parity() implementiert.
        // Dieses (Ergebnis + v2[i]) % 2 implementiert die Addition über GF(2)
        if ( (parity(x & v[i]) + v2[i]) % 2 ) {
            // Das jeweilige Bit des Outputbytes wird je nach i, also je nachdem
            // welche Zeile aktuell in der Berechnung bearbeitet wird, gesetzt,
            // wenn die Matrixmultiplikation mit x und Addition mit v2[i] der
            // aktuellen Zeile gleich 1 ist.
            ret += 1 << (7-i);
        }
    }

    return ret;
}

byte AESMath::exp(byte i) const {
    /*
     * Aufgabe 5c
     */
    return exp_table[i];
}

byte AESMath::inv(byte b) const {
    /*
     * Aufgabe 6
     */
    if (b == 0) return 0;

    // Gesucht ist das ? in: g^i * ? = g^1, wobei b=g^i.
    // multiplikative inverse: g^i * ? = g^1
    // m-1-i
    // 1) aus b i bekommen. 2)  g^m-1-i berechnen
    return exp(256-1-log(b));
}

byte AESMath::log(byte b) const {
    /*
     * Aufgabe 5d
     */
  return log_table[b];
}

byte AESMath::rpmul(byte a, byte b) {
    /*
     * Aufgabe 4
     */
    // TODO
    byte p = 0;
    while (a > 0) {
        if (a % 2 == 1) {
            p = p ^ b;
        }
        b = xtime(b);
        a = a >> 1;
    }
    return p;
}

byte AESMath::mul(byte a, byte b) const {
    /*
     * Aufgabe 5e
     */
    // Die Polynommultiplikation mit 0 ergibt ebenfalls 0.
    if (a == 0 || b == 0) return 0;

    /**
     * Um a°b = g^i ° g^j = g^((i+j) mod 255) zu berechnen, muss als erstes
     * das i in g^i für a und das j in g^j für b berechnet werden. Dafür wird
     * die Funktionn log() verwendet, welche die im Konstruktor berechnete
     * Lookup Tabelle log verwendet. log liefert das i für log(a) und das j
     * für log(b) zurück. Danach kann g^((i+j) mod 255) berechnet werden.
     * Sowohl g, als auch das Ergebnis von ((i+j) mod 255) ist bekannt. Jetzt
     * muss nur noch die "Polynomexponentation" angewendet werden, welches von
     * der Funktion exp() implementiert wird. exp() liefert das Ergebnis von
     * g^((i+j) mod 255) zurück und verwendet die im Konstruktor berechnete
     * LookupTabelle exp.
     */
    return exp( (log(a) + log(b)) % 255 );
}

byte AESMath::parity(byte b) {
    /*
     * Aufgabe 7
     */
    // cnt zählt die Anzahl der bisher gefundenen Einsen im Byte
    byte cnt = 0;
    // In jedem Schleifendurchlauf wird 1 Bit des Bytes überprüft
    for (int i = 0; i < 8; i++) {
        // b wird mit einer Bitmask nach != 0 überprüft. Die Bitmask hat 1 Bit
        // auf 1 gesetzt, die anderen Bits des Bytes sind 0. Wenn das bit in b
        // an der gleichen Position wie in der Bitmask auf 1 ist, dann ist
        // != 0 Wahr und der counter cnt wird um 1 erhöht.
        if ((b & (1 << i)) != 0) cnt++;
    }
    // cnt % 2 liefert 0 zurück, wenn die Anzahl der Eines in b ungerade ist.
    // Und 0 wenn die Anzahl gerade ist.
    return cnt % 2;
}

void AESMath::printTable(const vector<byte> &table) {
    cout << "         +----------------------------------------";
    cout << "---------------------------------------+" << endl;
    cout << "         |                                       y";
    cout << "                                       |" << endl;
    cout << "         +----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    cout << "        ";
    for (size_t i = 0; i < 16; i++) {
        cout << " | " << setw(2) << setfill(' ') << hex << i;
    }
    cout << " |" << endl;
    cout << "+---+----+----+----+----+----+----+----+----+----+";
    cout << "----+----+----+----+----+----+----+----+" << endl;
    for (size_t i = 0; i < 16; i++) {
        cout << "|   | " << setw(2) << setfill(' ') << hex << i;
        for (size_t j = 0; j < 16; j++) {
            cout << " | " << setw(2) << setfill('0') << (unsigned) table[(i << 4) + j];
        }
        cout << " |" << endl;
        if (i < 15) {
            if (i == 7) {
                cout << "| x ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;

            } else {
                cout << "|   ";
                cout << "+----+----+----+----+----+----+----+----+----+";
                cout << "----+----+----+----+----+----+----+----+" << endl;
            }
        } else {
            cout << "|---";
            cout << "+----+----+----+----+----+----+----+----+----+";
            cout << "----+----+----+----+----+----+----+----+" << endl;
        }
    }
}

byte AESMath::sBox(byte b) const {
    /*
     * Aufgabe 9c
     */
    return sbox[b];
}

byte AESMath::invSBox(byte b) const {
    /*
     * Aufgabe 9d
     */
    return inv_sbox[b];
}

byte AESMath::xtime(byte b) {
    /*
     * Aufgabe 3
     */
    // wenn höchstwertigste Bit gesetzt ist, ann muss durch das irreduzible
    // Polynom m(x) = 0x11b subtrahiert werden.
    byte r = b << 1;
    if ((b & 0x80) == 0x80) {
        r ^= 0x1b;
    }
    return r;
}

string AESMath::format(byte b) {
    ostringstream os;
    os << "{" << hex << setfill('0') << setw(2) << (unsigned)b <<"}";
    return os.str();
}
