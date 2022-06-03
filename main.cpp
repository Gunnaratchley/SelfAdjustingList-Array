#include <iostream>
#include <string>

#include "self_adjusting_array.h"
#include "self_adjusting_list.h"

int main()
{
	nwacc::linked_list<std::string> names;
	names.push_front("Shane");
	names.push_front("Daniel");
	names.push_front("Lela");
	names.push_front("Tamara");

	std::cout << "Starting Names using for each loop:" << std::endl;
	for (auto& value : names) {
		std::cout << value << " | ";
	}
	std::cout << std::endl;

	names.find("Daniel");
	std::cout << "Test for find function:" << std::endl;
	for (auto& value : names) {
		std::cout << value << " | ";
	}
	std::cout << std::endl;
	auto position = names.begin();
	names.insert(names.begin(), "New Name");
	names.insert(names.begin(), "Another new one");
	std::cout << "Display for inserting in front as well as a normal for loop:" << std::endl;
	for (auto iterator = names.begin(); iterator != names.end(); iterator++) {
		std::cout << *iterator << " | ";
	}
	std::cout << std::endl;

	nwacc::array_list<int> list(5);
	list.push_back(45);
	list.push_back(23);
	list.push_back(12);
	list.push_back(10);
	list.push_back(13);
	std::cout << "Starting list values using for loop:" << std::endl;
	for (auto iterator = list.begin(); iterator != list.end(); iterator++) {
		std::cout << *iterator << " | ";
	}
	std::cout << std::endl;
	list.find(12);
	std::cout << "Display after find function using for each loop:" << std::endl;
	for (const auto& element : list) {
		std::cout << element << " | ";
	}

	return 0;
}