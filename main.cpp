#include "KV_converter.h"
#include <array>

int main(int argc, char *argv[])
{
	std::string calibration_file = argv[1];
	
	std::array<double, 6> Vvalues = {85.023, 628.302, 892.35, 1725.0, 1600.2, 1050.13};
	std::array<double, 9> Kvalues = {1.23, 1.34, 1.40, 2.1, 3.5, 4.0125, 40.125, 401.25, 505.13};
	
	std::array<double, 6> convertedVs;
	std::array<double, 9> convertedKs;

	try
	{
		std::cout << "\n\n*-------------------------------------------------------------------*\n";

		KV_converter converter1(calibration_file);

		std::cout << "\n*-------------------------------------------------------------------*\n";

		for(int a = 0; a < Vvalues.size(); a++)
		{
			convertedVs[a] = converter1.convertVtoK(Vvalues[a]);
			std::cout << Vvalues[a] << " mV = " << convertedVs[a] << " *K" << std::endl;
			
			if((Vvalues[a] > VMAX) || (Vvalues[a] < VMIN))
				std::cout << "\nVoltage value given for conversion falls outside give range for\n\tthe cooler controller.\nConverted Kelvin temperature returned is an estimate.\n";
			
			std::cout << "\n*-------------------------------------------------------------------*\n";
		}
		
		for(int a = 0; a < Kvalues.size(); a++)
		{
			convertedKs[a] = converter1.convertKtoV(Kvalues[a]);
			std::cout << Kvalues[a] << " *K = " << convertedKs[a] << " mV" << std::endl;
			
			if((Kvalues[a] > KMAX) || (Kvalues[a] < KMIN))
				std::cout << "\nKelvin temperature given for conversion falls outside give range for\n\tthe cooler controller.\nConverted voltage value returned is an estimate.\n";
		
			std::cout << "\n*-------------------------------------------------------------------*\n";
		}
	}
	catch(const char *error)
	{
		std::cout << error << std::endl;
		return 0;
	}
	
	return 1;
}

	