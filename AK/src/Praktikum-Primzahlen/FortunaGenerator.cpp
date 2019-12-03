/*
 * FortunaGenerator.cpp
 *
 *  Created on: 20.04.2011
 *      Author: chkarg
 */

#include <iostream>
#include <fstream>
#include "filters.h"
#include "sha.h"
#include "aes.h"
#include "modes.h"
#include "hex.h"
#include "files.h"
#include "FortunaGenerator.h"

using namespace std;

FortunaGenerator::FortunaGenerator() {
	for (int i = 0; i < sizeof(key); i++) {
		key[i] = 0;
	}

	for (int i = 0; i < sizeof(cntr); i++) {
		cntr[i] = 0;
	}

	seeded = false;
	last_reseed=0;
}

bool FortunaGenerator::incCntr() {
}

void FortunaGenerator::reseed(byte* seed, unsigned int size) {
}

bool FortunaGenerator::getBit() {
}

byte FortunaGenerator::getByte() {
}

void FortunaGenerator::generateBlocks(byte* buffer, unsigned int n) {
}

void FortunaGenerator::getBlock(byte* buffer, unsigned int n) {
}

uint32_t FortunaGenerator::getTimeStamp() const {
	struct timeval  tv;

	gettimeofday(&tv,NULL);
	return tv.tv_sec*1000 + tv.tv_usec/1000;
}

