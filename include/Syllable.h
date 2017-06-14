/*
 * Syllable.h
 *
 *  Created on: 15 May 2017
 *      Author: patrick
 */

#ifndef SYLLABLE_H_
#define SYLLABLE_H_

#include "types.h"
#include "Vowel.h"
#include "Consonant.h"
#include <string>
#include <vector>

class Syllable {
public:
	Syllable(std::string);

	// set additional syllable information
	void determine_position_features(int16_t numberWord, int16_t currWord,int16_t numberSyl, int16_t currSyl, int16_t numberPhonPrev, int16_t numberPhonNext);
	void determine_accent_features(int16_t prev, int16_t current, int16_t next);

	// get functions
	syllableFeatures_t get_features() { build_feature_vector(); return m_features; };
	uint16_t get_number_phonemes() {return m_numberPhonemes; };
	std::string get_output_string () { return m_outputString; };

	// print feature vector
	void print();

	// debugging
	void print_debug();

private:
	// member functions
	void determine_syllable_structure();
	void determine_phonetic_features();
	void build_feature_vector();

	// members: syllable string
	std::string m_string;
	std::string m_outputString;
	stringVec_t m_stringVec;

	// members: phonetic features
	std::vector<Consonant>	m_onsetVec;
	std::vector<Vowel> 		m_nucleusVec;
	std::vector<Consonant>	m_codaVec;
	uint16_t m_numberPhonemes = 0;

	// members: accent features (zero initialized)
	accentFeatures_t m_accentVec;

	// members: position features (zero initialized)
	positionFeatures_t m_positionVec;

	// members: syllable feature vector
	syllableFeatures_t m_features;
};

#endif /* SYLLABLE_H_ */
