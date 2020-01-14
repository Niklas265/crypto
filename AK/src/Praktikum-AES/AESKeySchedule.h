#ifndef AES_AESKEYSCHEDULE_H
#define AES_AESKEYSCHEDULE_H

#include <string>
#include <vector>
#include "datatypes.h"
#include "AESMath.h"
#include "AESState.h"

using namespace std;

/**
 * AESKeySchedule ist eine Klasse, um aus einem initialen
 * Cipher Key eine key_schedule für AES zu erzeugen. key_schedule wird
 * bereits beim setzen des Keys in setKey() erzeugt und kann dann von AES
 * in der addKey() Funktion von AES verwendet werden. key_schedule
 * enthält einen verschiedenen Rundenschlüssel für jede von AES durchlaufene Runde.
 */
class AESKeySchedule {
private:
    /**
     * Flag zum Aktivieren von Debug-Ausgaben
     */
    bool debug_mode;
    /**
     * Zeiger auf ein Objekt der Klasse AESMath
     */
    const AESMath *math;
    //word *key_schedule;
    /**
     * Vektor zum Speichern der Rundenschlüssel
     */
    vector<word> key_schedule;
    /**
     * Größe des (Cipher Key) Schlüssels (in 4-Byte-Wörtern)
     * Die erlaubten Werte für nk sind: 4,6,8
     */
    size_t nk;
    /**
     * Anzahl der Runden. nr ist abhängig von nk, also der Größe des Schlüssels.
     */
    size_t nr;
    /**
     * Größe des Blocks (in 4-Byte-Wörtern). In dieser Implementation wird
     * nb im Konstruktor auf 4 gesetzt.
     */
    size_t nb;
    /**
     * Vektor zum Speichen der Rundenkonstanten.
     */
    vector<word> r_con;
public:
    /**
     * rotWord ist eine Hilfsfunktion für setKey und führt eine zyklische
     * Linksverschiebung um 8 Bit auf ein aus 4 Byte bestehendes Wort w aus.
     *
     * @param w Eingabewort, auf welches der zyklische 8 Bit Linksshift
     * ausgeführt wird.
     * @return Das Ergebnis nach dem Shift.
     */
    word rotWord(word w) const;

    /**
     * subWord wendet auf jedes Byte des Eingabeword die sBox Funktion an
     * und liefert das Ergebnis als word zurück.
     * @param w Ein aus 4 Byte bestehendes word, auf welches die 4 sBox
     * Funktionsaufrufe ausgeführt werden.
     * @return Die 4 Byte Ergebnisse aus der sBox werden zusammengefügt
     * und als Word zurückgegeben.
     */
    word subWord(word w) const;

public:
    /**
     * Konstruktor der AESKeySchedule Klasse, in welchem r_con,
     * also der Vektor zum Speichen der Rundenkonstanten berechnet wird.
     *
     * @param aesmath Zeiger auf ein Objekt der Klasse AESMath
     * @param debug_mode Flag zum Aktivieren von Debug-Ausgaben.
     * Wenn True, dann werden Debug Ausgaben über stdout ausgegeben.
     */
    AESKeySchedule(const AESMath &aesmath, bool debug_mode=false);
    virtual ~AESKeySchedule();

    /**
     * setKey verwendet den Übergabeparameter key und führt eine
     * Schlüsselerweiterung durch, um den key_schedule zu berechnen.
     * Danach ist key_schedule ein nb(nr+1) word großes Array. Da
     * nr indirekt von der Größe des keys abhängt, ist key_schedule
     * für größere Keys größer als für kleinere Keys. Der key muss
     * eine Schlüssellänge von 128, 192 oder 256 Bits haben.
     *
     * @param key ein vector Array von bytes mit einer Größe von
     * 128, 192 oder 256 Bits. key ist der Cipher Key, mit welchem
     * die key_schedule berechnet wird.
     *
     * @return True, wenn kein Fehler aufgetreten ist. False, falls
     * ein Fehler aufgereten ist. Zum Beispiel führt ein Schlüssel
     * mit einer nicht unterstützten Größe zu einem Fehler.
     */
    bool setKey(const vector<byte>& key);

    const word* getRoundKey(size_t round);

    string formatRoundKey(size_t round);

    size_t getNrOfRounds() const;

};

#endif //AES_AESKEYSCHEDULE_H
