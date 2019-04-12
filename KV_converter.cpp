#include "KV_converter.h"

//Default Constructor
KV_converter::KV_converter(){}

//Default Destructor
KV_converter:: ~KV_converter(){}

// Standard Constructor
// Input params: calibration table file name
KV_converter::KV_converter(std::string filename)
{
	conversion_table = load_table(filename);
	
	if(conversion_table.empty()) 
		throw "Error intaking calibration file.\nAborting.";
}

// Private method for converter classs.  Only called in constructor
// Input params: calibration table file name
// Returns:		 parsed vector containing table
std::vector< std::vector<double>> KV_converter::load_table(std::string file_name)
{	
	std::vector< std::vector<double>> conv_table;
	std::string line;
	std::fstream table_reader;
	
	table_reader.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	
	try{
		table_reader.open(file_name.c_str(), std::fstream::in);
			
		while(getline(table_reader, line))
		{
			std::istringstream ss(line);
			std::vector<double> temp_array(3,0);
						
			for(int a = 0; a <3; a++) 
				ss >> temp_array[a];
							
			conv_table.push_back(temp_array);
		}			
	}
	catch(std::ifstream::failure e){
		if(table_reader.eof()) {
			std::cout << "End of file." << std::endl;
			std::cout << "Calibration table loaded sucessfully." << std::endl;
		}
		else
			std::cout << "Exception opening/reading/closing file" << std::endl;
	}
	return conv_table;		
}

// Public method for converter class
// Input Params: Voltage Value (in Volts!!)
// Returns:		 Converted Kelvin Temperature (*K)

double KV_converter::convertVtoK(double Vvalue)
{
	int index = 0;
	
	Vvalue = Vvalue / 1000; //Voltage values passed in are in Volts, so convert to mV

	while(conversion_table[index][1] > Vvalue && (conversion_table.begin())++ != conversion_table.end()) 
		index++;

	double tempV1 = conversion_table[index][1];
	
	if(tempV1 == Vvalue) 
		return conversion_table[index][0];
	else
	{
		double tempK1 = conversion_table[index][0];
		double slope  = conversion_table[index][2];
		
		double deltaV = Vvalue - tempV1;
		double result = (deltaV/slope) + tempK1;

		return result;
	}
}
		
// Public method for converter class
// Input Params: Kelvin temperature
// Returns:		 Converted voltage value (mV)

double KV_converter::convertKtoV(double Kvalue)
{
	int index = 0;
	
	while(conversion_table[index][0] < Kvalue && (conversion_table.begin())++ != conversion_table.end()) 
		index++;
	
	double tempK1 = conversion_table[index][0];
	
	if(tempK1 == Kvalue)
		return conversion_table[index][1] * 1000;
	else
	{
		double tempV1 = conversion_table[index][1] * 1000;  // convert to mV
		double slope  = conversion_table[index][2];			// already in mV

		double deltaK = Kvalue - tempK1;
		double result = slope*deltaK + tempV1;
		
		return result;
	}	
}