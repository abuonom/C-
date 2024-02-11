#include <map>
#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <fstream>
#include <stdlib.h>

class InvalidDate : public std::exception
{
public:
	virtual const char *what() const throw()
	{
		return "Error: invalid Date";
	}
};

class InvalidValuePos : public std::exception
{
public:
	virtual const char *what() const throw()
	{
		return "Error: not a positive number.";
	}
};

class InvalidValueSize : public std::exception
{
public:
	virtual const char *what() const throw()
	{
		return "Error: too large a number.";
	}
};

class InvalidLine : public std::exception
{
public:
	virtual const char *what() const throw()
	{
		return "Invalid line";
	}
};

class BitcoinExchange
{
private:
	std::map<std::string, double> ExchangeData;
	std::string filename;
	bool isValidLine(const std::string line);

public:
	BitcoinExchange(std::string filename);
	~BitcoinExchange();
	void loadExchangeData();
	void printExchangeData();
	void printValueData();
	void PrintResult();
};

BitcoinExchange::BitcoinExchange(std::string filename)
{
	this->filename = filename;
	std::cout << "Costruttore di default richiamato" << std::endl;
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::loadExchangeData()
{
	std::ifstream file("data.csv");
	std::string linea;
	double rate;
	if (!file.is_open())
	{
		std::cerr << "Impossibile aprire il file!" << std::endl;
		return;
	}
	std::getline(file, linea); // salta la prima linea
	while (std::getline(file, linea))
	{
		std::istringstream ss(linea);
		std::string data;
		if (std::getline(ss, data, ',') && ss >> rate)
			ExchangeData[data] = rate;
		else
			throw InvalidLine();
	}
}

bool BitcoinExchange::isValidLine(const std::string line)
{
	// Trova la posizione del separatore '|'
	size_t separator_pos = line.find('|');
	if (separator_pos == std::string::npos)
		throw InvalidDate();

	// Estrai la data e il valore dalla linea
	std::string date_string = line.substr(0, separator_pos);
	std::string value_string = line.substr(separator_pos + 1);

	// Verifica la validità della data
	std::istringstream date_ss(date_string);
	int year, month, day;
	char dash1, dash2;
	bool valid_date = date_ss >> year >> dash1 >> month >> dash2 >> day &&
					  dash1 == '-' && dash2 == '-' &&
					  month >= 1 && month <= 12 &&
					  day >= 1 && day <= 31;
	if (!valid_date)
		throw InvalidDate();

	// Verifica la validità del valore
	char *end;
	double value = strtod(value_string.c_str(), &end);
	if (value <= 0)
		throw InvalidValuePos();
	if (value < 0 || value > 1000)
		throw InvalidValueSize();

	// Restituisci true se sia la data che il valore sono validi, altrimenti false
	return true;
}

void BitcoinExchange::printExchangeData()
{
	std::cout << "Exchange Data:" << std::endl;
	for (std::map<std::string, double>::const_iterator it = ExchangeData.begin(); it != ExchangeData.end(); ++it)
	{
		std::cout << "Data: " << it->first << ", Rate: " << it->second << std::endl;
	}
}

void BitcoinExchange::PrintResult()
{
	std::ifstream file(this->filename.c_str());
	std::string linea;
	double rate;
	if (!file.is_open())
	{
		std::cerr << "Impossibile aprire il file!" << std::endl; // qua devo mette eccezione
		return;
	}
	std::getline(file, linea); // salta la prima linea
	while (std::getline(file, linea))
	{
		try
		{
			this->isValidLine(linea);
			size_t separator_pos = linea.find('|');
			if (separator_pos == std::string::npos)
				throw InvalidDate();

			// Estrai la data e il valore dalla linea
			std::string date_string = linea.substr(0, separator_pos);
			std::string value_string = linea.substr(separator_pos + 1);
			rate = atof(value_string.c_str()) * ExchangeData[date_string];
			std::cout << date_string << " => " << value_string << " = " << rate << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cerr << e.what() << '\n';
		}
	}
}

/*

	 ./btc input.txt
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-01-09 => 1 = 0.32
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.

*/
