/*!
 * .xy to .x00 Converter v1.0.0
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
 * Code obtained from Documentation: 
 * https://learn.microsoft.com/en-us/windows/win32/sysinfo/retrieving-the-last-write-time
 */
BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
    FILETIME ftCreate, ftAccess, ftWrite;
    SYSTEMTIME stUTC, stLocal;
    DWORD dwRet;

    // Retrieve the file times for the file.
    if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
        return FALSE;

    // Convert the last-write time to local time.
    FileTimeToSystemTime(&ftWrite, &stUTC);
    SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

    // Build a string showing the date and time.
    stringstream ss;
    ss << stLocal.wDay << "-" << stLocal.wMonth << "-" << stLocal.wYear << "," << stLocal.wHour << ":" << stLocal.wMinute;
    string timeString = ss.str();
    cout << timeString;

    if( S_OK == dwRet )
        return TRUE;
    else return FALSE;
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
				ofstream out(inputFile.c_str(), ios::app);
				out << "\n\n";
				
				// sets up read/write from the .xy file into a .x00 with the same name
				FILE * fin = freopen(inputFile.c_str(), "r", stdin);
				inputFile = inputFile.substr(0, inputFile.find_last_of("."));
				FILE * fout = freopen((inputFile + ".x00").c_str(), "w", stdout);
				inputFile = inputFile.substr(inputFile.find_last_of("\\")+1);

				// read and stores the contents of the .xy as a list (vector) of pairs of numbers
				vector<pair<double, double>> rows;
				pair<double, double> ir;
				while(!cin.eof()){
					cin >> ir.first >> ir.second;
					rows.push_back(ir);
				}
				fclose(fin);

				// prints the header for the .x00, see README for the format
				cout << "HR-XRDScan\n";
				cout << "FileName, " << inputFile.substr(inputFile.find_last_of("\\")+1) + ".x00\n";
				cout << "FileDateTime, ";
				GetLastWriteTime(hFile, szBuf, MAX_PATH);
				cout << "\n";
				cout << "Sample, " << inputFile.substr(0, inputFile.find("__")) + "\n";
				cout << "Reflection, 0 0 4\n";
				cout << "Wavelength, 1.5405980\n";
				cout << "GenkVmA, 45, 40\n";
				double omega = (rows[0].first+ir.first)/2;
				cout << "Omega, " << omega << "\n";
				cout << "TwoTheta, " << omega*2 << "\n";
				cout << "X, 0.00\nY, 0.00\nPhi, 0.000\nPsi, 0.000\n";
				cout << "ScanType, CONTINUOUS\n";
				cout << "ScanAxis, Omega/2Theta\n";
				cout << "FirstAngle, " << rows[0].first << "\n";
				cout << "ScanRange, " << ir.first - rows[0].first << "\n";
				cout << "StepWidth, " << rows[1].first - rows[0].first << "\n";
				cout << "TimePerStep, 0.500\n";
				cout << "NrOfData, " << rows.size()-1 << "\n";
				cout << "ScanData\n";

				// prints the intensity data out to 14 decimal points precision
				for(int i=0; i<rows.size()-1; i++){
					cout << fixed;
					cout << setprecision(14);
					cout << rows[i].second << "\n";
				}
				fclose(fout);
			}
		}
	}
	return 0;
}