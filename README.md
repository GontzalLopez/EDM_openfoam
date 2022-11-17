# EDM_openfoam
**Eddy dissipation combustion model updated for Openfoam 9**

Copy the whole src directory into user project folder:
```
foam
cp -r --parents src/combustionModels $WM_PROJECT_USER_DIR
cd $WM_PROJECT_USER_DIR/src/combustionModels
```
Download the code to the src directory:
```
git clone https://github.com/GontzalLopez/EDM_openfoam $WM_PROJECT_USER_DIR/src/combustionModels
```
Add EDM/EDM.C to Make/files using Vim
```
vim Make/files
```
Compile:
```
wmake
```
In constant/combustionProperties:
```
/*--------------------------------*- C++ -*----------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Version:  9
     \\/     M anipulation  |
\*---------------------------------------------------------------------------*/
FoamFile
{
    format      ascii;
    class       dictionary;
    location    "constant";
    object      combustionProperties;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

combustionModel EDM;

EDMCoeffs
{
    semiImplicit    yes;
    Cdiff	          4.0;
    CEDM            4.0;
}

#include "reaction"
// ************************************************************************* //
```
