/*
 * Consonant.cpp
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#include "Consonant.h"
#include "Exception.h"

Consonant::Consonant(std::string input)
{
	m_string = input;
	determine_features();
}

void Consonant::determine_features()
{
	std::string tmp_str (m_string);

	// compute unvoiced consonant modifier
	std::size_t position = tmp_str.find("_0");
	if (position!=std::string::npos)
	{
		if ( (position != 0) && (tmp_str.length() <= 5) && (tmp_str[position+2] == '\0') )
		{
			tmp_str.erase(position,2);
			m_unvoiced = true;
		}
		else
		{
			throw Exception("Invalid unvoiced consonant transcription found: " + tmp_str);
		}
	}

	// check validity
	if (tmp_str.length() > 2)
	{
		throw Exception("Invalid or unused {consonant,modifier} combination found: " + tmp_str);
	}

	// determine features
	try
	{
		m_features = CONSONANTS.at(tmp_str);
		if (m_unvoiced)
		{
			m_features[0] = 0;
		}
	}
	catch (std::exception& e)
	{
	    throw Exception("Invalid or unused SAMPA consonant symbol found: " + tmp_str);
	}
}

const std::map<std::string, consonant_feature_vec> Consonant::CONSONANTS{
		{""	  , {0, 0, 0, 0, 0, 0, 0} },
		{"p"  , {0, 0, 1, 0, 0, 0, 0} },
		{"b"  , {1, 0, 1, 0, 0, 0, 0} },
		{"t"  , {0, 0, 1, 0, 0, 0, 1} },
		{"d"  , {1, 0, 1, 0, 0, 0, 1} },
		{"k"  , {0, 0, 1, 0, 0, 0, 2} },
		{"g"  , {1, 0, 1, 0, 0, 0, 2} },
		{"?"  , {1, 0, 1, 0, 0, 0, 3} },
		{"f"  , {0, 0, 0, 1, 0, 0, 0} },
		{"v"  , {1, 0, 0, 1, 0, 0, 0} },
		{"s"  , {0, 0, 0, 1, 0, 0, 1} },
		{"z"  , {1, 0, 0, 1, 0, 0, 1} },
		{"S"  , {0, 0, 0, 1, 0, 0, 1} },
		{"Z"  , {1, 0, 0, 1, 0, 0, 1} },
		{"T"  , {0, 0, 0, 1, 0, 0, 1} },
		{"D"  , {1, 0, 0, 1, 0, 0, 1} },
		{"C"  , {0, 0, 0, 1, 0, 0, 2} },
		{"j"  , {1, 0, 0, 1, 0, 0, 2} },
		{"x"  , {0, 0, 0, 1, 0, 0, 2} },
		{"h"  , {0, 0, 0, 1, 0, 0, 3} },
		{"m"  , {1, 1, 0, 0, 0, 0, 0} },
		{"n"  , {1, 1, 0, 0, 0, 0, 1} },
		{"N"  , {1, 1, 0, 0, 0, 0, 2} },
		{"l"  , {1, 0, 0, 0, 0, 1, 1} },
		{"R"  , {1, 0, 0, 1, 0, 0, 2} },
		{"r"  , {1, 0, 0, 1, 0, 0, 2} },
		{"pf" , {0, 0, 1, 1, 0, 0, 0} },
		{"ps" , {0, 0, 1, 1, 0, 0, 1} },
		{"ts" , {0, 0, 1, 1, 0, 0, 1} },
		{"tS" , {0, 0, 1, 1, 0, 0, 1} },
		{"pS" , {0, 0, 1, 1, 0, 0, 1} },
		{"dZ" , {1, 0, 1, 1, 0, 0, 1} },
	};

