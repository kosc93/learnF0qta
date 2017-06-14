/*
 * Vowel.cpp
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#include "Vowel.h"
#include "Exception.h"

Vowel::Vowel(std::string input)
{
	m_string = input;
	determine_features();
}

void Vowel::determine_features()
{
	std::string tmp_str (m_string);
	vowelFeatures_t diphthongVowel {0}, diphthongVowel2 {0};

	// compute long vowel modifier
	std::size_t position = tmp_str.find(":");
	if (position!=std::string::npos)
	{
		if (position != 0)
		{
			tmp_str.erase(position,1);
			m_long = true;
		}
		else
		{
			throw Exception("Invalid long vowel transcription found: " + tmp_str);
		}
	}

	// compute nasal vowel modifier
	position = tmp_str.find("~");
	if (position!=std::string::npos)
	{
		if (position != 0)
		{
			tmp_str.erase(position,1);
			m_nasal = true;
		}
		else
		{
			throw Exception("Invalid nasal vowel transcription found: " + tmp_str);
		}
	}

	// compute first diphthong modifier
	position = tmp_str.find("_^");
	if (position != std::string::npos)
	{
		if (position != 0)
		{
			tmp_str.erase(position,2);
			m_diphthong = true;
			diphthongVowel = VOWELS.at(tmp_str.substr(0,1));
			tmp_str.erase(0,1);
		}
		else
		{
			throw Exception("Invalid first diphthong transcription found: " + tmp_str);
		}
	}

	// compute second diphthong modifier, e.g. Ingenieur
	position = tmp_str.find("_^");
	if (position != std::string::npos)
	{
		if (position != 0)
		{
			tmp_str.erase(position,2);
			m_diphthong2 = true;
			diphthongVowel2 = VOWELS.at(tmp_str.substr(0,1));
			tmp_str.erase(0,1);
		}
		else
		{
			throw Exception("Invalid first diphthong transcription found: " + tmp_str);
		}
	}

	// compute syllabic consonant modifier
	position = tmp_str.find("=");
	if (position!=std::string::npos)
	{
		if (position == 0 && tmp_str.length() < 2)
		{
			tmp_str = "=";
		}
		else
		{
			throw Exception("Invalid syllabic consonant transcription found: " + tmp_str);
		}
	}

	// check validity
	if (tmp_str.length() > 1)
	{
		throw Exception("Invalid or unused {vowel,modifier} combination found: " + tmp_str);
	}

	// determine features
	try
	{
		m_features = VOWELS.at(tmp_str);
		if (m_long)
		{
			m_features[3]++;
		}
		if (m_nasal)
		{
			m_features[4]++;
		}
		if (m_diphthong)
		{
			m_features[3]++;
			// calculate mean for diphthongs -> rounding problem?
			if (m_diphthong2)
			{
				m_features[3]++;
				m_features[0] = (m_features[0] + diphthongVowel[0] + diphthongVowel2[0])/3.0;
				m_features[1] = (m_features[1] + diphthongVowel[1] + diphthongVowel2[1])/3.0;
				m_features[2] = (m_features[2] + diphthongVowel[2] + diphthongVowel2[2])/3.0;
			}
			else
			{
				m_features[0] = (m_features[0] + diphthongVowel[0])*0.5;
				m_features[1] = (m_features[1] + diphthongVowel[1])*0.5;
				m_features[2] = (m_features[2] + diphthongVowel[2])*0.5;
			}
		}
	}
	catch (std::exception& e)
	{
	    throw Exception("Invalid or unused SAMPA vowel symbol found: " + tmp_str);
	}
}

const std::map<std::string, vowelFeatures_t> Vowel::VOWELS{
		{"i" , {-2, 3, 0, 0, 0} },
		{"I" , {-1, 2, 0, 0, 0} },
		{"e" , {-2, 1, 0, 0, 0} },
		{"E" , {-2,-1, 0, 0, 0} },
		{"y" , {-2, 3, 1, 0, 0} },
		{"Y" , {-1, 2, 1, 0, 0} },
		{"2" , {-2, 1, 1, 0, 0} },
		{"9" , {-2,-1, 1, 0, 0} },
		{"@" , { 0, 0, 0, 0, 0} },
		{"6" , { 0,-2, 0, 0, 0} },
		{"a" , { 0,-3, 0, 0, 0} },
		{"A" , { 2,-3, 0, 0, 0} },
		{"u" , { 2, 3, 1, 0, 0} },
		{"U" , { 1, 2, 1, 0, 0} },
		{"o" , { 2, 1, 1, 0, 0} },
		{"O" , { 2,-1, 1, 0, 0} },
		{"=" , { 0, 0, 0, 0, 0} }
	};

