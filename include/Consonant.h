/*
 * Consonant.h
 *
 *  Created on: 15 May 2017
 *      Author: Patrick Schmager
 */

#ifndef CONSONANT_H_
#define CONSONANT_H_

#include "Types.h"
#include <string>

class Consonant {
public:
	Consonant(std::string);
	consonant_feature_vec get_features() { return m_features; };
	std::string get_string() { return m_string; };

private:
	// member functions
	void determine_features();

	// members: consonant features
	consonant_feature_vec m_features;

	// members: consonant modifiers
	modifier_t m_unvoiced = false;

	// members: consonant string
	std::string m_string;

	// features: {voiced, nasal, plosive, fricative, approximant, lateral, place of articulation}
	static const std::map<std::string, consonant_feature_vec> CONSONANTS;
};

#endif /* CONSONANT_H_ */
