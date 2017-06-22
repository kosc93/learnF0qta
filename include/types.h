/*
 * Types.h
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#ifndef TYPES_H_
#define TYPES_H_

#include <cstdint>
#include <string>
#include <vector>
#include <array>
#include <map>

const uint16_t NUM_VOWEL_FEATURES 	= 5;
const uint16_t NUM_CONSONANT_FEATURES= 7;
const uint16_t NUM_PHONETIC_FEATURES = 54;
const uint16_t NUM_POSITION_FEATURES = 8;
const uint16_t NUM_ACCENT_FEATURES 	= 3;
const uint16_t NUM_SYLLABLE_FEATURES = 65;

const uint16_t NUM_PHONEMES_ONSET	= 3;
const uint16_t NUM_PHONEMES_NUCLEUS	= 1;
const uint16_t NUM_PHONEMES_CODA		= 4;
const uint16_t NUM_PHONEMES			= 8;

typedef int16_t 	feature_t;
typedef bool 	modifier_t;

typedef std::array<feature_t,NUM_VOWEL_FEATURES> 		vowelFeatures_t;
typedef std::array<feature_t,NUM_CONSONANT_FEATURES> 	consonantFeatures_t;
typedef std::array<feature_t,NUM_POSITION_FEATURES> 	positionFeatures_t;
typedef std::array<feature_t,NUM_ACCENT_FEATURES> 		accentFeatures_t;
typedef std::array<feature_t,NUM_SYLLABLE_FEATURES> 	syllableFeatures_t;

typedef std::vector<std::string> stringVec_t;

typedef std::map<double,double> signal_t;

#endif /* TYPES_H_ */
