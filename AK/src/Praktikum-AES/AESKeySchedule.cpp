#include <iostream>
#include <iomanip>
#include <sstream>
#include "AESKeySchedule.h"

using namespace std;

AESKeySchedule::AESKeySchedule(const AESMath &aesmath, bool debug_mode)
        : debug_mode(debug_mode), math((const AESMath *) &aesmath), key_schedule(), nk(0),
          nb(4), nr(0), r_con(11,0) {

    /*
     * Aufgabe 10
     */

    r_con[0] = 0;

    // r_con[i] = 2 •(RC[i-1]), gefolgt von 8 Nullbytes / 24 Nullbits.
    word a = 1;
    for (int i = 1; i < 11; i++) {
        // 24 Nullbits werden angehangen
        r_con[i] = a << 24;
        // implementiert die multiplikation mit 2 in 'r_con[i+1] = 02 •(RC[i])'
        // für den nächsten Schleifendurchlauf.
        a = aesmath.mul(a, 2);
    }
}

AESKeySchedule::~AESKeySchedule() {}

size_t AESKeySchedule::getNrOfRounds() const {
    return nr;
}

const word* AESKeySchedule::getRoundKey(size_t round) {
    assert(round<=nr);
    return &key_schedule[4*round];
}

string AESKeySchedule::formatRoundKey(size_t round) {
    assert(round<=nr);
    ostringstream os;

    for (size_t i=0; i<nb; i++) {
        os << hex << setw(8) << setfill('0') << key_schedule[4*round+i];
    }
    return os.str();
}

word AESKeySchedule::rotWord(word w) const {
    /*
     * Aufgabe 11
     */
    // (w << 8) leftshiftet die unteren 24 Bits des word w in die richtige Position.
    // Weiter müssen die oberen 8 Bits des word w zyklisch um 8 Bits nach links
    // geshiftet werden. Das ist das gleiche wie ein 24 Bit Rechtsshift mit dem
    // gesamten wort w, wobei die oberen 8 Bits in w nach dem Shift an den
    // unteren 8 Bits des Ergebnisses sind und die anderen 24 Bits nach dem
    // Rechtsshift aus dem word 'rausgeschoben', also verworfen werden.
    return (w << 8) + (w >> 24);
}

bool AESKeySchedule::setKey(const vector<byte>& key) {
    /*
     * Aufgabe 13
     */

    // nk, also die Größe des (Cipher Key) Schlüssels (in 4-Byte-Wörtern)
    // wird aus dem Übergabeargument key berechnet.
    // key ist ein vector aus byte werten. Da nk in 32 Bit (4-Byte) Wörtern
    // angegeben ist, werden die Byte in Bits umgerechnet und durch 32 geteilt.
    nk = key.size() * 8 / 32;

    // KA-04 Folie 29
    // In unserer Implementation wird nb, also Größe des Blocks
    // (in 4-Byte-Wörtern) auf 4 festgelegt.
    assert(nb == 4);
    // nr, also die Anzahl der Runden hängt von nk ab.
    // Die spezielle Anzahl der Runden je nk wird im Standard festgelegt.
    // In unserer Implementation werden nur Schlüssellängen 128, 192 und 256
    // unterstützt. Ist die Schlüssellänge davon unterschiedlich, wird false
    // zurückgegeben, was auf einen Fehler hinweist.
    if (nk == 4) nr = 10;
    else if (nk == 6) nr = 12;
    else if (nk == 8) nr = 14;
    else return false; // Schlüssellänge != 128, 192, 256

    // Die ersten nk words des key_schedule bestehen aus den ersten nk nk*4
    // Byte des key.
    for (int i = 0; i < nk; i++) {
        word w = 0;
        // Da key_schedule die Elemente als word verpackt erwartet,
        // werden je 4 Byte des key als word in w verpackt und an key_schedule
        // angehängt.
        for (int j = 0; j < 4; j++) {
            w <<= 8;
            w += key[4*i+j];
        }
        key_schedule.push_back(w);
    }

    // Im debug_mode werden Zwischenergebnisse über stdout ausgegeben.
    // Die Ausgabe orientiert sich an der des FIPS-Standards (Seite 27).
    if(debug_mode) {
        // Ausgabe der Header
        cout << " i|" << setw(15) << setfill(' ')
             << "temp|" << setw(15) << setfill(' ')
             << "rotWord|" << setw(15) << setfill(' ')
             << "subWord|" << setw(15) << setfill(' ')
             << "Rcon[i/Nk]|" << setw(15) << setfill(' ')
             << "Xor with Rcon|" << setw(15) << setfill(' ')
             << "w[i-Nk]|" << setw(15) << setfill(' ')
             << "w[i]" << endl;
    }

    // In jedem Schleifendurchlauf wird das i-te word in key_schedule gesetzt.
    // Für AES werden die benötigten Schlüssel im Vorfeld berechnet. Insgesamt
    // werden (nb * (nr+1)) word Daten für die Schlüssel benötigt.
    for (int i = nk; i < nb * (nr+1); i++) {
        // Alle Zwischenergebnisse werden separat nach jedem Zwischenschritt
        // in eine varible gespeichert, damit sie später, wenn debug_mode
        // aktiviert wurde, ausgegeben werden können. Die Variablen werden
        // alle bereits hier deklariert, damit sie auch später abgerufen
        // werden können.
        word temp, firsttemp, afterRotWord, afterSubWord, afterRconiDivnk,
             afterXORwithRcon, afterwIminusNk, aftertempXORwIminusNk;
        // Der aktuell zu berechnende Wert key_schedule[i] basiert auf
        // key_schedule[i-1], also dem im letzten Schleifendurchlauf
        // berechneten Schlüsselteil.
        firsttemp = key_schedule[i-1];
        temp = firsttemp;
        // TODO: genauer sagen was abläuft, on a higher level
        if (i % nk == 0) {
            // In jedem nk-ten Schleifendurchlauf wird auf temp rotWord und
            // subWord angewendet. Das Ergebnis dieser Berechnungen wird
            // mit der nächsten, noch nicht in der Schleife verwendeten,
            // Rundenkonstanten XOR'd.
            afterRotWord = rotWord(temp);
            afterSubWord = subWord(afterRotWord);
            afterRconiDivnk = r_con[i / nk];
            afterXORwithRcon = afterSubWord ^ afterRconiDivnk;
            temp = afterXORwithRcon;
        } else if (nk > 6 && i % nk == 4) {
            // Alternativ, wenn nk > 6 ist und i-4 ein vielfaches von nk ist,
            // dann wird nur subWord auf temp ausgeführt.
            afterSubWord = subWord(temp);
            temp = afterSubWord;
        }
        // Danach wird das (i-nk)-te key_schedule, also das im nk-t letzten
        // Schleifendurchlauf berechnete Ergebnis für key_schedule, mit
        // dem bisherigen Zwischenergebnis temp XOR'd. Dieses Endergebnis wird
        // dann mit push_back an die i-te Stelle in key_schedule gesetzt.
        afterwIminusNk = key_schedule[i-nk];
        aftertempXORwIminusNk = afterwIminusNk ^ temp;
        key_schedule.push_back(aftertempXORwIminusNk);

        if(debug_mode) {
            cout << dec << setw(2) << setfill(' ') << i << "|      ";
            cout << hex << setw(8) << setfill('0') << firsttemp << "|      ";
            if (i % nk == 0) {
                // In jedem nk-ten Schleifendurchlauf sind die folgenden 4 Werte
                // gesetzt und können ausgegeben werden.
                cout << hex << setw(8) << setfill('0') << afterRotWord << "|      ";
                cout << hex << setw(8) << setfill('0') << afterSubWord << "|      ";
                cout << hex << setw(8) << setfill('0') << afterRconiDivnk << "|      ";
                cout << hex << setw(8) << setfill('0') << afterXORwithRcon << "|      ";
            } else if (nk > 6 && i % nk == 4) {
                // Alternativ, wenn nk > 6 ist und i-4 ein vielfaches von nk ist
                // kann das Zwischenergebnis nach afterSubWord ausgegeben werden.
                // Die anderen Zwischenergebnisse wie afterRconiDivnk etc.
                // wurden in diesem Schleifendurchlauf nicht gesetzt und werden
                // deshalb auch nicht ausgegeben.
                cout << string(15, ' ') << hex << setw(8) << setfill('0') << afterSubWord
                     << string(30, ' ') << "|      ";
            } else {
                // ist keines davon der fall, dann wurden diese Zwischenergebnisse
                // in diesem Schleifendurchlauf nicht verwendet und werden nicht
                // ausgegeben.
                cout << string(60, ' ');
            }
            // Das Zwischenergebnis afterwIminusNk = key_schedule[i-nk] wird
            // in jedem Schleifendurchlauf ausgegeben, genauso wie das für
            // aftertempXORwIminusNk = afterwIminusNk ^ temp.
            cout << hex << setw(8) << setfill('0') << afterwIminusNk << "|      ";
            cout << hex << setw(8) << setfill('0') << aftertempXORwIminusNk << endl;
        }

    }

    return true;
}

word AESKeySchedule::subWord(word w) const {
    /*
     * Aufgabe 12
     */
    // ret speichert das Ergebnis
    word ret = 0;

    // Für jedes Byte in w, also 4 mal, wird:
    for (int i = 3; i >= 0; i--) {
        // Das bisherige Ergebnis ret um 1 Byte nach links geshiftet.
        ret <<= 8;
        // und das nächste Ergebnis der sBox an die unteren 8 Bits von ret
        // geschrieben.
        // w >> i*8 shiftet die als nächsts zu bearbeitenden 8 Bits an die
        // untersten 8 Bits. Die Funktion sBox erhält einen byte als
        // Eingabewert. Da wir ein word, also 4 Byte an die subBox Funktion
        // liefern, werden die oberen 3 Byte im word verworfen und müssen
        // hier nicht manuell gelöscht werden.
        ret += math->sBox(w >> i*8);
    }
    // Nach den 4 Schleifendurchläufen sind die 4 Bytes in ret mit den 4 Werten
    // aus sBox belegt und können zurückgegeben werden.
    return ret;
}
