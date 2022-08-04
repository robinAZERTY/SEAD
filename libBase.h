#pragma once
class libBase
{
	public:
		//Constructor
		libBase();

		//Destructor
		~libBase();

		//seter methode
		void set_variable(const int& val) { this->variable = val; };

		//geter methode
		const int get_variable() const { return this->variable; };

		//functionnal methode
		const bool is_equal_to(const int& another_number) const ;

	private:
		//private variables
		int variable = 0;
};