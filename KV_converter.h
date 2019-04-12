#ifndef __KV_CONVERTER_H__
#define __KV_CONVERTER_H__

#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define KMIN 1.4
#define KMAX 500

#define VMIN 0090.681
#define VMAX 1644.29

class KV_converter
{
	public:
		KV_converter();
		KV_converter(std::string filename);
		~KV_converter();
		
		double convertVtoK(double Vvalue);
		double convertKtoV(double Kvalue);
		
	private:
		std::vector< std::vector<double> > conversion_table;
		std::vector< std::vector<double> > load_table(std::string filename);
};

#endif //__KV_CONVERTER_H__

			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			
			