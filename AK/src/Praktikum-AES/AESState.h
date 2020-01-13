#ifndef AES_AESSTATE_H
#define AES_AESSTATE_H

#include <cassert>
#include <string>
#include "datatypes.h"
#include "AESMath.h"

using namespace std;

/**
 * AESState stellt den internen Zustand von AES dar.
 * Weiter werden Funktionen implementiert, welche den State
 * verändern/transformieren.
 *
 * Wie in den anderen Klassen für AES dieser Implementierung
 * wird davon ausgegangen, dass nb = 4 ist.
 */
class AESState {
private:
    /**
     * Flag zum Aktivieren von Debug-Ausgaben
     */
    bool debug_mode;
    /**
     * Zeiger auf ein Objekt der Klasse AESMath
     */
    const AESMath *math;

protected:
    /**
     * Interner Zustand (Array mit 16 Byte)
     *
     * Die state kann als rechteckiges array von bytes interpretiert werden.
     * Dieses Array hat 4 Reihen und weil nb auf 4 gesetzt ist auch 4 Zeilen.
     */
    byte state[16];

    byte getCell(size_t row, size_t col);
    void setCell(size_t row, size_t col, byte b);

    /**
     * shiftRow führt eine zyklische Rechtsverschiebung der Zeile row der state
     * um shift Positionen durch.
     *
     * @param row Zeile der State, die verschoben werden soll.
     * @param shift Anzahl der Positionen, um die die row nach rechts
     * zyklisch verschoben werden wird.
     */
    void shiftRow(size_t row, size_t shift);

public:
    AESState(const AESMath& aesmath, bool debug_mode=false);

    /**
     * Setzt die 16 Byte in state auf die 16 Bytes ab *in.
     *
     * @param in Zeiger auf einen mindestens 16 Byte großen Speicherbereich.
     * Diese 16 Bytes werden nach state kopiert.
     */
    void set(const byte* in);

    /**
     * Setzt die 16 Bytes ab *out auf die 16 Bytes von state.
     *
     * @param out Zeiger auf einen mindestens 16 Byte großen Speicherbereich,
     * an welchen das 16 Byte große state Array kopiert wird.
     */
    void get(byte* out);

    /**
     *  Führt die Basisoperation ShiftRows auf den internen Zustand aus.
     *  ShiftRows shiftet die Reihen der state zyklisch um verschiedene offsets.
     *  Reihe 0 wird nicht verschoben. Reihe 1 wird um 1 Byte, Reihe 2 um 2 und
     *  Reihe 3 um 3 Byte nach rechts zyklisch verschiebt.
     */
    void shiftRows();
    void subBytes();
    void mixColumns();

    /**
     * invShiftRows macht die shiftRows Funktion wieder rückgänig.
     * Dafür wird die Reihe 1 wird um 3 Byte, Reihe 2 um 2 und
     * Reihe 3 um 1 Byte nach rechts zyklisch verschoben. Wird für eine State
     * shiftRows und danach invShiftRows aufgerufen, dann ist darauf
     * resulierende State die gleiche wie vor dem Aufruf von shiftRows().
     */
    void invShiftRows();
    void invSubBytes();
    void invMixColumns();
    void addKey(const word* key);

    string format() const;
};


#endif //AES_AESSTATE_H
