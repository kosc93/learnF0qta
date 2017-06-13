/*
 * Syllable.cpp
 *
 *  Created on: 15 May 2017
 *      Author: patrick
 */

#include "Syllable.h"
#include "Exception.h"
#include <iostream>
#include <sstream>
#include <regex>

Syllable::Syllable(std::string input)
{
	m_string = input;
	determine_syllable_structure();	// get m_stringVec ("tokenize")
	determine_phonetic_features();	// create Consonant/Vowel objects in onset, nucleus, coda
}

void Syllable::determine_syllable_structure()
{
	// filter '_' at syllable beginning e.g. "t._t"
	if (m_string[0] == '_')
	{
		m_string.erase(0,1);
	}

	// determine onset, nucleus and coda by regex
	std::string tmp_str (m_string);
	std::regex vowels("[aAoeEiIOuUyY269@=](:|~|_\\^){0,3}[aAoeEiIOuUy269@=]?(:|~|_\\^){0,3}[aAoeEiIOuUy269@=]?(:|~|_\\^){0,3}");
	std::regex consonants("((\\?|pf|ps|ts|tS|pS|dZ|p|b|t|d|k|g|f|v|s|z|S|Z|T|D|C|x|h|m|n|N|l|j|R|r)(_0)?)?");

	// tokenize phonemes in onset
	for (uint8_t i=0; i<NUM_PHONEMES_ONSET; ++i)
	{
		std::regex_iterator<std::string::iterator> rit ( tmp_str.begin(), tmp_str.end(), consonants );
		// auto it = boost::find_regex(tmp_str,consonants); ss << it;
		m_stringVec.push_back(rit->str());
		tmp_str.erase(tmp_str.find(m_stringVec[i]),m_stringVec[i].length());
	}

	// tokenize phonemes in nucleus
	for (uint8_t i=0; i<NUM_PHONEMES_NUCLEUS; ++i)
	{
		std::regex_iterator<std::string::iterator> rit ( tmp_str.begin(), tmp_str.end(), vowels );
		m_stringVec.push_back(rit->str());
		tmp_str.erase(tmp_str.find(m_stringVec[i+NUM_PHONEMES_ONSET]),m_stringVec[i+NUM_PHONEMES_ONSET].length());
	}

	// tokenize phonemes in coda
	for (uint8_t i=0; i<NUM_PHONEMES_CODA; ++i)
	{
		std::regex_iterator<std::string::iterator> rit ( tmp_str.begin(), tmp_str.end(), consonants );
		m_stringVec.push_back(rit->str());
		tmp_str.erase(tmp_str.find(m_stringVec[i+NUM_PHONEMES_ONSET+NUM_PHONEMES_NUCLEUS]),m_stringVec[i+NUM_PHONEMES_ONSET+NUM_PHONEMES_NUCLEUS].length());
	}

	// swap onset phonemes if neccessary
	if ( (m_stringVec[0] != "") && (m_stringVec[1] == "") && (m_stringVec[2] == "") )
	{
		m_stringVec[2].swap(m_stringVec[0]);
	}
	else if ( (m_stringVec[0] != "") && (m_stringVec[1] != "") && (m_stringVec[2] == "") )
	{
		m_stringVec[2].swap(m_stringVec[1]);
		m_stringVec[1].swap(m_stringVec[0]);
	}

	// check correct matching
	if ( m_string.length() != (m_stringVec[0].length()+m_stringVec[1].length()+m_stringVec[2].length()+m_stringVec[3].length()+m_stringVec[4].length()+m_stringVec[5].length()+m_stringVec[6].length()+m_stringVec[7].length()) )
	{
		throw Exception("Invalid or unused SAMPA symbols in syllable transcription found: " + m_string);
	}
}

void Syllable::determine_phonetic_features()
{
	// onset
	for (uint8_t i=0; i<NUM_PHONEMES_ONSET; ++i)
	{
		m_onsetVec.push_back(Consonant(m_stringVec[i]));
	}

	// nucleus
	for (uint8_t i=0; i<NUM_PHONEMES_NUCLEUS; ++i)
	{
		m_nucleusVec.push_back(Vowel(m_stringVec[i + NUM_PHONEMES_ONSET]));
	}

	// onset
	for (uint8_t i=0; i<NUM_PHONEMES_CODA; ++i)
	{
		m_codaVec.push_back(Consonant(m_stringVec[i + NUM_PHONEMES_ONSET + NUM_PHONEMES_NUCLEUS]));
	}

	// count number of phonemes in syllable
	for (uint8_t i=0; i<NUM_PHONEMES; ++i)
	{
		if (m_stringVec[i] != "")
		{
			++m_numberPhonemes;
		}
	}

	build_feature_vector();
}

void Syllable::determine_accent_features(uint8_t prev, uint8_t current, uint8_t next)
{
	m_accentVec = {prev, current, next};
	build_feature_vector();
}

void Syllable::determine_position_features(uint8_t numberWord, uint8_t currWord,uint8_t numberSyl, uint8_t currSyl, uint8_t numberPhonPrev, uint8_t numberPhonNext)
{
	// count number of phonemes
	uint8_t cntOnset = 0, cntCoda = 0;
	for (uint8_t i=0; i<NUM_PHONEMES_ONSET; ++i)
	{
		if (m_stringVec[i] != "")
		{
			++cntOnset;
		}
	}

	for (uint8_t i=0; i<NUM_PHONEMES_CODA; ++i)
	{
		if (m_stringVec[i + NUM_PHONEMES_ONSET + NUM_PHONEMES_NUCLEUS] != "")
		{
			++cntCoda;
		}
	}

	m_positionVec = {numberWord, currWord, numberSyl, currSyl, cntOnset, cntCoda, numberPhonPrev, numberPhonNext};

	// words beginning with vowels become glottal onset
	if (cntOnset == 0 && currWord == 1 && currSyl == 1)
	{
		m_onsetVec[2] = Consonant("?");
	}

	build_feature_vector();
}

void Syllable::build_feature_vector()
{
	// get phonetical features
	consonant_feature_vec p1 = m_onsetVec[0].get_features();
	consonant_feature_vec p2 = m_onsetVec[1].get_features();
	consonant_feature_vec p3 = m_onsetVec[2].get_features();
	vowel_feature_vec 	  p4 = m_nucleusVec[0].get_features();
	consonant_feature_vec p5 = m_codaVec[0].get_features();
	consonant_feature_vec p6 = m_codaVec[1].get_features();
	consonant_feature_vec p7 = m_codaVec[2].get_features();
	consonant_feature_vec p8 = m_codaVec[3].get_features();

	// get accent features
	accent_feature_vec a0 = m_accentVec;

	// get position features
	position_feature_vec p0 = m_positionVec;

	// write to syllable feature vector
	m_features = {
			p1[0], p1[1], p1[2], p1[3], p1[4], p1[5], p1[6],
			p2[0], p2[1], p2[2], p2[3], p2[4], p2[5], p2[6],
			p3[0], p3[1], p3[2], p3[3], p3[4], p3[5], p3[6],
			p4[0], p4[1], p4[2], p4[3], p4[4],
			p5[0], p5[1], p5[2], p5[3], p5[4], p5[5], p5[6],
			p6[0], p6[1], p6[2], p6[3], p6[4], p6[5], p6[6],
			p7[0], p7[1], p7[2], p7[3], p7[4], p7[5], p7[6],
			p8[0], p8[1], p8[2], p8[3], p8[4], p8[5], p8[6],
			a0[0], a0[1], a0[2],
			p0[0], p0[1], p0[2], p0[3], p0[4], p0[5] , p0[6] , p0[7]
	};

	// build output string
	m_outputString = "";
	for (uint8_t i=0; i<NUM_SYLLABLE_FEATURES; ++i)
	{
		m_outputString += (std::to_string(m_features[i]) + ",");
	}
}

void Syllable::print()
{
	for (uint8_t i=0; i<NUM_SYLLABLE_FEATURES; ++i)
	{
		std::cout << m_features[i] << ",";
	}
}

void Syllable::print_debug()
{
	// print tokenized string
	std::cout << "syllable features:\t[";
	if (m_onsetVec[0].get_string() != "")
		std::cout << m_onsetVec[0].get_string() << "-";
	if (m_onsetVec[1].get_string() != "")
		std::cout << m_onsetVec[1].get_string() << "-";
	if (m_onsetVec[2].get_string() != "")
		std::cout << m_onsetVec[2].get_string();
	std::cout << "][" << m_nucleusVec[0].get_string() << "][";
	if (m_codaVec[0].get_string() != "")
		std::cout << m_codaVec[0].get_string();
	if (m_codaVec[1].get_string() != "")
		std::cout << "-" << m_codaVec[1].get_string();
	if (m_codaVec[2].get_string() != "")
		std::cout << "-" << m_codaVec[2].get_string();
	if (m_codaVec[3].get_string() != "")
		std::cout << "-" << m_codaVec[3].get_string();
	std::cout << "] \t-->\t|";

	// print phonetical features
	for (uint8_t i=0; i<NUM_PHONEMES_ONSET; ++i)
	{
		for (uint8_t j=0; j<NUM_CONSONANT_FEATURES; ++j)
		{
			std::cout << m_features[j+i*NUM_CONSONANT_FEATURES];
		}
		std::cout << "|";
	}
	for (uint8_t i=NUM_PHONEMES_ONSET; i<NUM_PHONEMES_ONSET+NUM_PHONEMES_NUCLEUS; ++i)
	{
		for (uint8_t j=0; j<NUM_VOWEL_FEATURES; ++j)
		{
			std::cout << m_features[j+i*NUM_CONSONANT_FEATURES];
		}
		std::cout << "|";
	}
	for (uint8_t i=NUM_PHONEMES_ONSET+NUM_PHONEMES_NUCLEUS; i<NUM_PHONEMES; ++i)
	{
		for (uint8_t j=0; j<NUM_CONSONANT_FEATURES; ++j)
		{
			std::cout << m_features[j+i*NUM_CONSONANT_FEATURES-(NUM_CONSONANT_FEATURES-NUM_VOWEL_FEATURES)];
		}
		std::cout << "|";
	}

	// print accent features
	for (uint8_t i=NUM_PHONETIC_FEATURES; i<NUM_PHONETIC_FEATURES+NUM_ACCENT_FEATURES; ++i)
	{
		std::cout << m_features[i];
	}
	std::cout << "|";

	// print location features
	for (uint8_t i=NUM_PHONETIC_FEATURES+NUM_ACCENT_FEATURES; i<NUM_SYLLABLE_FEATURES; ++i)
	{
		std::cout << m_features[i];
	}
	std::cout << "|";
	std::cout << std::endl;

}





