/*
 * Word.cpp
 *
 *  Created on: 16 May 2017
 *      Author: patrick
 */

#include "StringOps.h"
#include "Word.h"
#include "Exception.h"
#include <algorithm>
#include <iostream>

Word::Word(std::string input, std::string accentPattern)
{
	m_string = input;
	determine_accent_pattern(accentPattern);
	filter_accent();
	determine_syllable_vec();
}

void Word::determine_accent_pattern(std::string accentPattern)
{
	for (uint8_t i=0; i<accentPattern.size(); ++i)
	{
		m_accentPattern.push_back(accentPattern[i] - '0'); // char to int
	}

	m_numberSyllables = accentPattern.size();
}

void Word::determine_syllable_vec()
{
	// split words by vertical line
	uint8_t cntWord (0); uint8_t cntSylAbs (0);
	std::vector<std::string> word_str;
	split(word_str, m_string, "|");

	for (auto w = word_str.begin(); w != word_str.end(); ++w)
	{
		cntWord++;

		// split syllables by dot
		uint8_t cntSylRel = 0;
		std::vector<std::string> syllable_str;
		split(syllable_str,*w,".");

		// create Syllable objects
		for (auto s = syllable_str.begin(); s != syllable_str.end(); ++s)
		{
			cntSylRel++; cntSylAbs++;
			m_syllable_vec.push_back(Syllable(*s));
		}

		// additional information on accent and position
		if (m_numberSyllables == 1) // words with only one syllable
		{
			m_syllable_vec[0].determine_accent_features(0, m_accentPattern[0], 0);
			m_syllable_vec[0].determine_position_features(word_str.size(),cntWord,syllable_str.size(),1,0,0);
		}
		else // longer words
		{
			// first syllable
			m_syllable_vec[0].determine_accent_features(0, m_accentPattern[0], m_accentPattern[1]);
			m_syllable_vec[0].determine_position_features(word_str.size(),cntWord,syllable_str.size(),1,0,m_syllable_vec[1].get_number_phonemes());

			// following syllables
			for (uint8_t i=1; i<m_numberSyllables-1; ++i)
			{
				m_syllable_vec[i].determine_accent_features(m_accentPattern[i-1], m_accentPattern[i], m_accentPattern[i+1]);
				m_syllable_vec[i].determine_position_features(word_str.size(),cntWord,syllable_str.size(),i+1,m_syllable_vec[i-1].get_number_phonemes(),m_syllable_vec[i+1].get_number_phonemes());
			}

			// last syllable
			m_syllable_vec[m_numberSyllables-1].determine_accent_features(m_accentPattern[m_numberSyllables-2], m_accentPattern[m_numberSyllables-1], 0);
			m_syllable_vec[m_numberSyllables-1].determine_position_features(word_str.size(),cntWord,syllable_str.size(),syllable_str.size(),m_syllable_vec[m_numberSyllables-2].get_number_phonemes(),0);
		}
	}

	// validity check
	if (cntSylAbs != m_numberSyllables)
	{
		throw Exception("ERROR: Accent pattern doesn't match SAMPA string!");
	}
}

void Word::print()
{
	for ( auto i = m_syllable_vec.begin(); i != m_syllable_vec.end(); i++ )
	{
		i->print();
	}
	std::cout << std::endl;
}

std::string Word::get_output_string()
{
	std::string output ("");
	for ( auto i = m_syllable_vec.begin(); i != m_syllable_vec.end(); i++ )
	{
		output += (i->get_output_string());
	}

	return output;
}

void Word::print_debug()
{
	int cnt (1);
	std::cout << m_string << std::endl;
	for ( auto i = m_syllable_vec.begin(); i != m_syllable_vec.end(); i++ )
	{
		std::cout << "(" << cnt << ") ";
		i->print_debug();
		++cnt;
	}
	std::cout << std::endl;
}

void Word::filter_accent()
{
	// main accent
	auto positions = m_string.find('\"');
	if (positions!=std::string::npos)
	{
		m_string.erase(positions,1);
	}

	// other accent
	positions = m_string.find('%');
	while (positions!=std::string::npos)
	{
		m_string.erase(positions,1);
		positions = m_string.find('%');
	}

	// also filter backslash because there is no fricative j
	positions = m_string.find('\\');
	if (positions!=std::string::npos)
	{
		m_string.erase(positions,1);
	}
}

