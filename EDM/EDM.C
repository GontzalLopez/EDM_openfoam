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

\*---------------------------------------------------------------------------*/

#include "EDM.H"
#include "addToRunTimeSelectionTable.H"
#include "zeroGradientFvPatchFields.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace combustionModels
{
    defineTypeNameAndDebug(EDM, 0);
    addToRunTimeSelectionTable
    (
        combustionModel,
        EDM,
        dictionary
    );
}
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::combustionModels::EDM::EDM
(
    const word& modelType,
    const fluidReactionThermo& thermo,
    const compressibleMomentumTransportModel& turb,
    const word& combustionProperties
)
:
    singleStepCombustion
    (
        modelType,
        thermo,
        turb,
        combustionProperties
    ),
    Cdiff_(this->coeffs().template lookup<scalar>("CEDM")),
    CEDM_(this->coeffs().template lookup<scalar>("Cdiff"))	
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

Foam::combustionModels::EDM::~EDM()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

Foam::tmp<Foam::volScalarField>
Foam::combustionModels::EDM::tDiff() const
{
    tmp<volScalarField> tdelta
    (
        new volScalarField
        (
            IOobject
            (
                "tdelta",
                this->mesh().time().timeName(),
                this->mesh(),
                IOobject::NO_READ,
                IOobject::NO_WRITE
            ),
            this->mesh(),
            dimensionedScalar(dimLength, Zero),
            zeroGradientFvPatchScalarField::typeName
        )
    );

    volScalarField& delta = tdelta.ref();
    delta.ref() = cbrt(this->mesh().V());
    delta.correctBoundaryConditions();

    // NOTE: Assume Prt = 1
    return Cdiff_*this->turbulence().nuEff()/sqr(delta);
}


Foam::tmp<Foam::volScalarField>
Foam::combustionModels::EDM::tTurb() const
{

    return ((CEDM_*this->turbulence().epsilon())/max(this->turbulence().k(),dimensionedScalar("SMALL",sqr(dimVelocity), SMALL)));
}

Foam::tmp<Foam::volScalarField>
Foam::combustionModels::EDM::timeScale() 
{
	return (max(this->tTurb(), this->tDiff())); 
}


void Foam::combustionModels::EDM::correct()
{
	
    	this->wFuel_ ==
        dimensionedScalar(dimMass/pow3(dimLength)/dimTime, 0);

    this->fresCorrect();

    const label fuelI = this->fuelIndex();

    const volScalarField& YFuel = this->thermo().composition().Y()[fuelI];

    const dimensionedScalar s = this->s();

    if (this->thermo().composition().contains("O2"))
    {
        const volScalarField& YO2 = this->thermo().composition().Y("O2");

        this->wFuel_ ==
            this->rho()*min(YFuel, YO2/s.value())*timeScale();
    /*
        this->wFuel_ ==
            this->rho()/(this->mesh().time().deltaT(i)*C_)
           *min(YFuel, YO2/s.value());
    */
    }
    else
    {
	    FatalErrorInFunction
	    << "You selected a combustion model that requires O2 mass"
            << " to be present in the mixture"
            << exit(FatalError);
    }

}



bool Foam::combustionModels::EDM::read()
{
    if (singleStepCombustion::read())
    {
        this->coeffs().lookup("Cdiff") >> Cdiff_ ;	    
        this->coeffs().lookup("CEDM") >> CEDM_ ;

        return true;
    }
    else
    {
        return false;
    }
}


// ************************************************************************* //
