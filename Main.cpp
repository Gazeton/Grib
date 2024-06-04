#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include "GribKapituła.h"
using namespace std;

long long PrintMultipleByte(std::fstream& grib, int times) {
	std::string bitSum;
	char byte;
	bitSum = "";
	for (int i = 0; i < times; ++i) {
		grib.read(&byte, 1);
		bitSum += std::bitset<8>((long long)byte).to_string();
	}
	return std::stoll(bitSum, nullptr, 2);
}

int main() {
	fstream grib;
	grib.open("all.grib", std::ios::in | std::ios::binary);

	if (!grib.is_open()) {
		std::cout << "Nie udalo sie otworzyc pliku!\n";
		return 1;
	}

	char byte_char;
	uint32_t start_section_0 = 0;

	char byte;
	char byte2;
	char byte3;
	char byte4;

	std::string binary = "";

	std::cout << R"(
========================== 
	
== General Message Info == 
		
==========================
 
)";

	while (!grib.eof()) {
		grib.read(&byte, 1);
		if (byte == 'G') {
			grib.read(&byte2, 1);
			grib.read(&byte3, 1);
			grib.read(&byte4, 1);

			if (byte2 == 'R' && byte3 == 'I' && byte4 == 'B') {
				std::cout << "Grib Finda at pos: " << (uint32_t)grib.tellg() - 4 << "\n";
				start_section_0 = (uint32_t)grib.tellg() - 4;
				break;
			}
		}
	}

	int pos7777 = 0;
	while (!grib.eof()) {
		grib.read(&byte, 1);
		if (byte == '7') {
			grib.read(&byte2, 1);
			grib.read(&byte3, 1);
			grib.read(&byte4, 1);

			if (byte2 == '7' && byte3 == '7' && byte4 == '7') {
				pos7777 = (int)grib.tellg() - 4;
				std::cout << "7777 Find at position: " << pos7777 << "\n";
				break;
			}
		}
	}
	std::cout << "Distance bettwen end of grib and start of 7777: " << pos7777 - (int)start_section_0 + 4 << "\n";

	grib.seekg(start_section_0 + 4);
	int message_len = 0;

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	message_len = std::stoi(binary, nullptr, 2);

	std::cout << "Whole Message length: " << message_len << "\n";

	std::cout << R"(
======================

==  Section 1 Data  ==

======================

)";
	grib.read(&byte_char, 1);

	uint32_t start_section_1 = grib.tellg();

	uint32_t section_1_length = 0;
	for (int i = 0; i < 3; ++i) {
		uint8_t tmp;
		grib >> tmp;
		section_1_length |= (static_cast<uint32_t>(tmp) << (8 * (2 - i)));
	}
	std::cout << "Section 1 length: " << section_1_length << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Parameter table Version: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Identification of Center: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "process ID number: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Grid Identification info: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "GDS and BMS: " << std::bitset<8>((int)byte_char).to_string() << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Unit parameters: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Indicator of type of level or layer: " << (int)byte_char << "\n";

	std::cout << "Height, pressure, etc. of the level or layer: " << PrintMultipleByte(grib, 2) << "\n";

	std::cout << "YY/MM/DD|HH:MM : ";
	grib.read(&byte_char, 1);
	std::cout << int(byte_char) << "/";
	grib.read(&byte_char, 1);
	std::cout << int(byte_char) << "/";
	grib.read(&byte_char, 1);
	std::cout << int(byte_char);
	grib.read(&byte_char, 1);
	std::cout << ((std::to_string(byte_char).length()) == 1 ? "|0" : "|") << int(byte_char);
	grib.read(&byte_char, 1);
	std::cout << ((std::to_string(byte_char).length()) == 1 ? ":0" : ":") << int(byte_char) << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Time unit: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "P1 - Period of time: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "P2 - Period of time: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Time range indicator: " << (int)byte_char << "\n";

	std::cout << "Numbers for last row: " << PrintMultipleByte(grib, 2) << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Number missing from averages or accumulation: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Reference century: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Identification of sub Center: " << (int)byte_char << "\n";

	std::cout << "Decimal Scale factor: " << PrintMultipleByte(grib, 2) << "\n";

	std::cout << R"(
======================

==  Section 2 Data  ==

======================

)";
	grib.seekg(start_section_1 + section_1_length);
	uint32_t start_section_2 = grib.tellg();

	int section_2_length = 0;
	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	section_2_length = std::stoi(binary, nullptr, 2);
	std::cout << "Section 2 length: " << section_2_length << "\n";

	grib.read(&byte_char, 1);
	int vert_coord_nr = (int)byte_char;
	std::cout << "Number of Vertical Coordinates: " << vert_coord_nr << "\n";

	grib.read(&byte_char, 1);
	int octet_nr = (int)byte_char;
	std::cout << "Octet number: " << octet_nr << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Data representation type: " << (int)byte_char << "\n";

	std::cout << "Numbers of points along a latitude circle: " << PrintMultipleByte(grib, 2) << "\n";

	std::cout << "Numbers of points along a longitude meridian: " << PrintMultipleByte(grib, 2) << "\n";

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	int sign = binary[0] - '0';
	binary.erase(0, 1);
	std::cout << "Latitude:" << ((sign) ? " -" : " ") << std::stoi(binary, nullptr, 2) / 1000 << " degrees \n";

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	sign = binary[0] - '0';
	binary.erase(0, 1);
	std::cout << "Longitude:" << ((sign) ? " -" : " ") << std::stoi(binary, nullptr, 2) / 1000 << " degrees \n";

	grib.read(&byte_char, 1);
	std::cout << "Resolution and component flags: " << std::bitset<8>((int)byte_char).to_string() << "\n";

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	sign = binary[0] - '0';
	binary.erase(0, 1);
	std::cout << "Latitude of last grid point:" << ((sign) ? " -" : " ") << std::stoi(binary, nullptr, 2) / 1000 << " degrees \n";

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	sign = binary[0] - '0';
	binary.erase(0, 1);
	std::cout << "Longitude of last grid point:" << ((sign) ? " -" : " ") << std::stoi(binary, nullptr, 2) / 1000 << " degrees \n";

	std::cout << "Longitudinal Direction Increment: " << PrintMultipleByte(grib, 2) / 1000 << " degrees \n";

	std::cout << "Latitudinal Direction Increment: " << PrintMultipleByte(grib, 2) / 1000 << " degrees \n";

	grib.read(&byte_char, 1);
	string scanning_mode_flags = std::bitset<8>((int)byte_char).to_string();
	bool i_pp = (scanning_mode_flags[0] == '1');
	bool j_pp = (scanning_mode_flags[1] == '1');
	bool I_priority = (scanning_mode_flags[2] == '0');

	cout << "Scanning mode flags : Points scan in " << ((i_pp) ? "+i" : "-i") << " and " << ((j_pp) ? "+j" : "-j") << " direction: " << ((I_priority) ? "(FORTRAN: (I,J))" : "(FORTRAN: (J,I))") << "\n";

	for (int i = 1; (i + 28) < octet_nr; ++i) {
		grib.read(&byte_char, 1);
	}

	for (int i = 1; i <= vert_coord_nr; i++) {
		binary = "";
		int end = (vert_coord_nr == 0) ? 2 : 4;
		for (int j = 0; j < end; ++j) {
			grib.read(&byte_char, 1);
			binary += std::bitset<8>((int)byte_char).to_string();
		}
		cout << "Wiersz " << i << " Punktow: " << std::stoi(binary, nullptr, 2) << " \n";
	}

	std::cout << R"(
======================

==  Section 3 Data  ==

======================

)";

	grib.seekg(start_section_2 + section_2_length);
	uint32_t start_section_3 = grib.tellg();

	binary = "";
	for (int i = 0; i < 3; ++i) {
		grib.read(&byte_char, 1);
		binary += std::bitset<8>((int)byte_char).to_string();
	}
	int section_3_length = std::stoi(binary, nullptr, 2);

	std::cout << "Section 3 length: " << section_3_length << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Flag to decode: " << (int)byte_char << "\n";

	grib.read(&byte_char, 1);
	std::cout << "Binary Scale Factor: " << (int)byte_char << "\n";

	return 0;
}
