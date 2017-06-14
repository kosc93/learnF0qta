/*
 * Vowel.h
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#ifndef VOWEL_H_
#define VOWEL_H_

#include "types.h"
#include <string>
#include <map>

class Vowel {
public:
	Vowel(std::string);
	vowelFeatures_t get_features() { return m_features; };
	std::string get_string() { return m_string; };

private:
	// member functions
	void determine_features();

	// members: vowel features
	vowelFeatures_t m_features;

	// members: vowel modifiers
	modifier_t m_long = false;
	modifier_t m_nasal = false;
	modifier_t m_diphthong = false;
	modifier_t m_diphthong2 = false;

	// members: vowel string
	std::string m_string;

	// features: {backness, height, roundedness, length, nasal, glottal, diphthon, syllabic}
	static const std::map<std::string, vowelFeatures_t> VOWELS;
};

#endif /* VOWEL_H_ */
