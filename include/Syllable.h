/*
 * Syllable.h
 *
 *  Created on: 15 May 2017
 *      Author: patrick
 */

#ifndef SYLLABLE_H_
#define SYLLABLE_H_

#include "Types.h"
#include "Vowel.h"
#include "Consonant.h"
#include <string>
#include <vector>

class Syllable {
public:
	Syllable(std::string);

	// set additional syllable information
	void determine_position_features(uint8_t numberSyl, uint8_t positionCur, uint8_t numberPhonPrev, uint8_t numberPhonNext);
	void determine_accent_features(uint8_t prev, uint8_t current, uint8_t next);

	// get functions
	syllable_feature_vec get_features() { build_feature_vector(); return m_features; };
	uint8_t get_number_phonemes() {return m_numberPhonemes; };
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
	std::vector<std::string> m_stringVec;

	// members: phonetic features
	std::vector<Consonant>	m_onsetVec;
	std::vector<Vowel> 		m_nucleusVec;
	std::vector<Consonant>	m_codaVec;
	uint8_t m_numberPhonemes = 0;

	// members: accent features (zero initialized)
	accent_feature_vec m_accentVec;

	// members: position features (zero initialized)
	position_feature_vec m_positionVec;

	// members: syllable feature vector
	syllable_feature_vec m_features;

	// member: glottal start syllable
	bool m_glottal = false;
};

#endif /* SYLLABLE_H_ */
