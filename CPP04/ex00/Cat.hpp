#ifndef CAT_HPP
# define CAT_HPP

# include <string>
# include <iostream>
# include "Animal.hpp"

using std::string;
using std::cout;
using std::endl;


class Cat : public Animal
{
protected:
	string type;
public:
	Cat();
	Cat(const Cat &toCopy);
	Cat& operator=(const Cat &tocopy);
	~Cat();
	void makeSound() const;
	string getType() const;
};
#endif
