/*!
 * .xy to .x00 Converter v1.2.0
 * Copyright Aaron Wang 2023-7-26
 * 
 * Contact: 
 *     email: wang_aaron_@hotmail.com
 *     Github: https://github.com/drasimov
 */
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>

#include <windows.h>
#include <tchar.h>

using namespace std;

/* 
 * Using Windows API, method prints out last time file was edited, used to
 * print out the "FileDateTime" in the .x00 file
 * 
 * Code references Documentation: 
 * https://learn.microsoft.com/en-us/windows/win32/sysinfo/retrieving-the-last-write-time
 */
string GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    stringstream ss;
    ss << stLocal.wDay << "-" << stLocal.wMonth << "-" << stLocal.wYear << "," << stLocal.wHour << ":" << stLocal.wMinute;
    string timeString = ss.str();
    return timeString;
}

/* 
 * Takes any .xy files passed to it and creates a .x00 file with the same 
 * information, with some preset values in the header.
 * 
 * @param argc (default c parameter) gives number of parameters following
 *        argv ^ list of strings, in this case file routes, passed to the method,
 *               note that argv[0] represents the executable file's own path
 * 
 * @return nothing meaningful, data is directly processed
 */
int main ( int argc, char *argv[] )
{
	if ( argc >= 2 ){

		for(int i=1; i<argc; i++){
			string inputFile = argv[i];

			if(inputFile.substr(inputFile.find_last_of(".") + 1) == "xy") {
				// stores initial state to extract LastWriteTime later
				HANDLE hFile;
				TCHAR szBuf[MAX_PATH];
				hFile = CreateFile((LPCSTR)argv[i], GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
				
				// adds two blank lines to the end of the .xy file
				ofstream fapp(inputFile.c_str(), ios::app);
				fapp << "\n\n";
				fapp.close();
				
				// sets up read/write from the .xy file into a .x00 with the same name
				ifstream fin(inputFile.c_str());
				inputFile = inputFile.substr(0, inputFile.find_last_of("."));
				ofstream fout((inputFile + ".x00").c_str());
				inputFile = inputFile.substr(inputFile.find_last_of("\\")+1);

				// read and stores the contents of the .xy as a list (vector) of pairs of numbers
				vector<pair<double, double>> rows;
				pair<double, double> ir;
				while(!fin.eof()){
					fin >> ir.first >> ir.second;
					rows.push_back(ir);
				}
				fin.close();

				// prints the header for the .x00, see README for the format
				fout << "HR-XRDScan\n";
				fout << "FileName, " << inputFile.substr(inputFile.find_last_of("\\")+1) + ".x00\n";
				fout << "FileDateTime, " << GetLastWriteTime(hFile, szBuf, MAX_PATH) << "\n";
				fout << "Sample, " << inputFile.substr(0, inputFile.find("__")) + "\n";
				fout << "Reflection, 0 0 4\n";
				fout << "Wavelength, 1.5405980\n";
				fout << "GenkVmA, 45, 40\n";
				double omega = (rows[0].first+ir.first)/2;
				fout << "Omega, " << omega << "\n";
				fout << "TwoTheta, " << omega*2 << "\n";
				fout << "X, 0.00\nY, 0.00\nPhi, 0.000\nPsi, 0.000\n";
				fout << "ScanType, CONTINUOUS\n";
				fout << "ScanAxis, Omega/2Theta\n";
				fout << "FirstAngle, " << rows[0].first << "\n";
				fout << "ScanRange, " << ir.first - rows[0].first << "\n";
				fout << "StepWidth, " << rows[1].first - rows[0].first << "\n";
				fout << "TimePerStep, 0.500\n";
				fout << "NrOfData, " << rows.size()-1 << "\n";
				fout << "ScanData\n";

				// prints the intensity data out to 14 decimal points precision
				for(int i=0; i<rows.size()-1; i++){
					fout << fixed;
					fout << setprecision(14);
					fout << rows[i].second << "\n";
				}
				fout.close();
			}
		}
	}
	return 0;
}
