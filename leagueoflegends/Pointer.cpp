#include "Pointer.h"

#include <ios>
#include <sstream>

QWORD Pointer::Hex() {
	return (QWORD)this;
}

std::string Pointer::HexS() {
	std::stringstream ss;
	ss << "0x" << std::hex << (QWORD)this;
	return ss.str();
}