/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2020 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::combustionModels::EDM

Description
    Simple infinitely fast chemistry combustion model based on the principle
    mixed is burnt. Additional parameter C is used to distribute the heat
    release rate.in time

SourceFiles
    EDM.C

\*---------------------------------------------------------------------------*/

#ifndef EDM_H
#define EDM_H

#include "singleStepCombustion.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace combustionModels
{

/*---------------------------------------------------------------------------*\
                   Class EDM Declaration
\*---------------------------------------------------------------------------*/

class EDM
:
    public singleStepCombustion
{
    // Private Data

        //- Molecular diffusion constant
        scalar Cdiff_;
	scalar CEDM_;

public:

    //- Runtime type information
    TypeName("EDM");


    // Constructors

        //- Construct from components
        EDM
        (
            const word& modelType,
            const fluidReactionThermo& thermo,
            const compressibleMomentumTransportModel& turb,
            const word& combustionProperties
        );

        //- Disallow default bitwise copy construction
        EDM(const EDM&);


    //- Destructor
    virtual ~EDM();


    // Member Functions

    	//- Calculate diffusive ts
        tmp<volScalarField> tDiff() const;

    	//- Calculate turbulent ts
        tmp<volScalarField> tTurb() const;

    	//- Calculate maximum time-scale
        tmp<volScalarField> timeScale();

        //- Correct combustion rate
        virtual void correct();

        //- Update properties
        virtual bool read();


    // Member Operators

        //- Disallow default bitwise assignment
        void operator=(const EDM&) = delete;
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace combustionModels
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
