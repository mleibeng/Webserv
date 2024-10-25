#!/bin/bash

# Function to display usage
usage() {
	echo "Usage: $0 <Directory> <ClassName>"
	exit 1
}

# Check if the output directory and class name are provided
if [ -z "$1" ] || [ -z "$2" ]; then
	usage
fi

# Output directory and class name
OUTPUT_DIR=$1
CLASS_NAME=$2

# Create the output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Convert class name to uppercase for include guard
CLASS_NAME_UPPER=$(echo "$CLASS_NAME" | tr '[:lower:]' '[:upper:]')

# Header file name
HPP_FILE="${OUTPUT_DIR}/${CLASS_NAME}.hpp"

# Source file name
CPP_FILE="${OUTPUT_DIR}/${CLASS_NAME}.cpp"

# Create the header file
cat << EOF > $HPP_FILE
#ifndef ${CLASS_NAME_UPPER}_H
#define ${CLASS_NAME_UPPER}_H

#define	GREY  "\033[38;5;246m"
#define	RESET "\033[0m"

#include <iostream>

class $CLASS_NAME
{
	public:
		$CLASS_NAME();
		$CLASS_NAME(const $CLASS_NAME &other);
		$CLASS_NAME& operator=(const $CLASS_NAME &other);
		~$CLASS_NAME();

	private:

};

#endif // ${CLASS_NAME_UPPER}_H
EOF

# Create the source file
cat << EOF > $CPP_FILE
#include "${CLASS_NAME}.hpp"

$CLASS_NAME::$CLASS_NAME()
{
	std::cout << GREY << "Default constructor called" << RESET << std::endl;
}

$CLASS_NAME::$CLASS_NAME(const $CLASS_NAME &other)
{
	std::cout << GREY << "Copy constructor called" << RESET << std::endl;
	*this = other;
}

$CLASS_NAME& $CLASS_NAME::operator=(const $CLASS_NAME &other)
{
	if (this != &other)
	{
		std::cout << GREY << "Copy assignment operator called" << RESET << std::endl;
	}
	return *this;
}

$CLASS_NAME::~$CLASS_NAME()
{
	std::cout << GREY << "Destructor called" << RESET << std::endl;
}
EOF

echo "Created $HPP_FILE and $CPP_FILE with a standard class in orthodox canonical form."
