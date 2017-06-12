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
#include <array>
#include <map>

const uint8_t NUM_VOWEL_FEATURES 	= 8;
const uint8_t NUM_CONSONANT_FEATURES= 7;
const uint8_t NUM_POSITION_FEATURES = 6;
const uint8_t NUM_ACCENT_FEATURES 	= 3;
const uint8_t NUM_SYLLABLE_FEATURES = 66;

const uint8_t NUM_PHONEMES_ONSET	= 3;
const uint8_t NUM_PHONEMES_NUCLEUS	= 1;
const uint8_t NUM_PHONEMES_CODA		= 4;
const uint8_t NUM_PHONEMES			= 8;

typedef signed int 	feature_t;
typedef bool 		modifier_t;

typedef std::array<feature_t,NUM_VOWEL_FEATURES> 		vowel_feature_vec;
typedef std::array<feature_t,NUM_CONSONANT_FEATURES> 	consonant_feature_vec;
typedef std::array<feature_t,NUM_POSITION_FEATURES> 	position_feature_vec;
typedef std::array<feature_t,NUM_ACCENT_FEATURES> 		accent_feature_vec;
typedef std::array<feature_t,NUM_SYLLABLE_FEATURES> 	syllable_feature_vec;

#endif /* TYPES_H_ */
