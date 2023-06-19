# Marcel2
Marcel is a tool aimed at plotting and comparing the content of several root files. The root variables to be plotted are specified in an xml file. Its readout is based on the tinyxml2 library.

Usage : marcel2 xmlfile

The input file are declared using :
```xml
<file type="1" name="fileName.root" />
```
Where type can range from 1 to 3 and denote the file set to be.  The file name is the name of the root file to be read. 
All files in a set is merged. A full directory can be read in one go using :
```xml
<fdir type="1" name="directoryName" />
```
Each set can have a name :
```xml
<title type="1" name="Roff" />
```
The variables to be plotted are declared using :
```xml
<plot  name="varName"  nBins="nBins" vMin="min value" vMax="max value" />
```
where var name is the name of the variable to be plotted, nBins the number of bins, vMin and vMax the minimum and maximum values of the variable. Several optional parameter are available. A title can be set using :
```xml
title="title"
```
The axis name can be set using :
```xml
xLabel="xTitle" yLabel="yTitle"
```
Maximum and minimum values can be set using :
```xml
hMin="min value" hMax="max value"
```
The histograms are normalized to 1 by default, to disable this feature use :
```xml
<normalize value="false" />
```
Cut can be assigned to all sets using :
```xml
<cut value="cut" />
```
The cut is a string that can be used to select the events to be plotted. The cut is applied to all the sets. The cut can be set for each set using :
```xml
<pCut type="1" value="cut" />
```
The output files (png pictures) output directory can be set using :
```xml
<directory value="outputDirectory" />
```


