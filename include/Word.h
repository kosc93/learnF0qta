/*
 * Word.h
 *
 *  Created on: 16 May 2017
 *      Author: patrick
 */

#ifndef WORD_H_
#define WORD_H_

#include "types.h"
#include "Syllable.h"
#include <vector>
#include <string>

class Word {
public:
	Word(std::string sampa, std::string accentPattern);
	void print();
	void print_debug();
	std::string get_output_string ();

private:
	//member functions
	void determine_accent_pattern(std::string accentPattern);
	void determine_syllable_vec();
	void filter_accent();

	//members
	unsigned int m_numberSyllables = 0;
	std::string m_string;
	std::vector<Syllable> m_syllable_vec;
	std::vector<int16_t> m_accentPattern;

};

#endif /* WORD_H_ */
