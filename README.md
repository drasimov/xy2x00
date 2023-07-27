# .xy to .x00 Converter v1.1.0
Copyright Aaron Wang 2023-7-26 

This tool copies information from .xy files to .x00 files for use in X'Pert Epitaxy software in analyzing results of x-ray crystallography. 

### To set up:
Drag the shortcut icon onto the desktop, this will be the easiest way to run the converter.

### To run:
Simply drag and drop the .xy file onto the .exe icon and a .x00 file will appear next to the original location of the .xy file! Currently, the system can only process one file at a time, updates pending...

### To redistribute:
The converter-redist.zip file will contain all the necessary files, simply copy to another computer and extract the files. To ensure the shortcur works, rename the "converter-redist" folder to "xy2x00".

### Dependencies
Uses gcc compiler, so requires the following dlls:
- libgcc_s_dw2-1.dll
- libstdc++-6.dll

These are included in the folder.

### Notes:
- only .xy files are accepted! nothing will happen if other files are dropped, though.
- the program is assuming the .xy files have intensity to a precision of 14 decimal places, and that the omega angle is increasing at a constant interval. Otherwise, a x00 file cannot be created without data loss.
- the FileDateTime can only be accessed here under a Windows operating system. If you are using a different system and errors occur, this is likely the cause.
- a few blank lines are added to the end of the original .xy file each time you run it; this is normal and ensures that all lines of data are read to the .x00.
- for further assumptions, see the full x00 header below:

```xml
HR-XRDScan
FileName, D1268.x00 [alter here]
FileDateTime, 07-Jul-2023,15:11 [alter here]
Sample, D1268 [alter here]
Reflection, 0 0 4
Wavelength, 1.5405980
GenkVmA, 45, 40
Omega, 32.77140	[alter here]
TwoTheta, 65.54280	[alter here]
X, 0.00
Y, 0.00
Phi, 0.000
Psi, 0.000
ScanType, CONTINUOUS
ScanAxis, Omega/2Theta
FirstAngle, 32.50000	[alter here]
ScanRange, 0.65000	[alter here]
StepWidth, 0.00020	[alter here]
TimePerStep, 0.500
NrOfData, 3251	[alter here]
ScanData
[insert single file data here]
```
