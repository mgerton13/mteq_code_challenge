#include "KV_converter.h"
#include <algorithm>

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
	std::vector<double> Kvals, Vvals, Slopes;
	table_reader.exceptions( std::ifstream::failbit | std::ifstream::badbit );
	
	try{
		table_reader.open(file_name.c_str(), std::fstream::in);
			
		while(getline(table_reader, line))
		{
			std::istringstream ss(line);
			std::vector<double> temp_array(3,0);
			
			for(int a = 0; a <3; a++)
				ss >> temp_array[a];

			Kvals.push_back(temp_array[0]);
			Vvals.push_back(temp_array[1]);
			Slopes.push_back(temp_array[2]);
		}	
	}
	catch(std::ifstream::failure e){
		if(table_reader.eof()) {			
			conv_table.push_back(Kvals);
			conv_table.push_back(Vvals);
			conv_table.push_back(Slopes);

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
// Notes: 		 Uses std::lower_bound for searching instead of just iterating
//				 lower_bound has O(log(n)) efficiency, so it is more scalable
//				 should a much larger configuration file be provided
double KV_converter::convertVtoK(double Vvalue)
{
	Vvalue = Vvalue / 1000; //Voltage values passed in are in mVolts, so convert to V

	int index = 0;
	
	std::vector<double>::iterator it;
	std::vector<double> sorted = conversion_table[1];
	std::sort(sorted.begin(), sorted.end());
	it = std::lower_bound(sorted.begin(), sorted.end(), Vvalue);

	index = sorted.size() - (it - sorted.begin());
	
	double tempV1 = conversion_table[1][index];
		
	if(index != 0 && conversion_table[1][index-1] == Vvalue) 
		return conversion_table[0][index-1];
	else
	{
		double tempK1 = conversion_table[0][index];
		double slope  = conversion_table[2][index];
		
		double deltaV = Vvalue - tempV1;
		double result = (deltaV/slope) + tempK1;

		return result;
	}
}
		
// Public method for converter class
// Input Params: Kelvin temperature
// Returns:		 Converted voltage value (mV)
// Notes: 		 Uses std::lower_bound for searching instead of just iterating
//				 lower_bound has O(log(n)) efficiency, so it is more scalable
//				 should a much larger configuration file be provided
double KV_converter::convertKtoV(double Kvalue)
{
	int index = 0;
	std::vector<double>::iterator it;
	it = std::lower_bound(conversion_table[0].begin(), conversion_table[0].end(), Kvalue);
	
	if(it == conversion_table[0].end())				//if lower_bound return <vector>.end() than all values in the vector are smaller, 	
		index = 0;									//so we set index = 0 to calculate off the correct values.
	else
		index = (it - conversion_table[0].begin());

	double tempK1 = conversion_table[0][index];
	
	if(conversion_table[0][index] == Kvalue)
		return conversion_table[1][index] * 1000;
	else
	{
		double tempV1 = conversion_table[1][index]* 1000;  // convert to mV
		double slope  = conversion_table[2][index];			

		double deltaK = Kvalue - tempK1;
		double result = slope*deltaK + tempV1;
		
		return result;
	}	
}