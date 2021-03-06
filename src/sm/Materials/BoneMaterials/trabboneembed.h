/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2013   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#ifndef trabboneembed_h
#define trabboneembed_h

#include "sm/Materials/structuralmaterial.h"
#include "floatarray.h"
#include "floatmatrix.h"
#include "cltypes.h"
#include "matconst.h"
#include "matstatus.h"
#include "sm/Materials/structuralms.h"
#include "cltypes.h"

///@name Input fields for TrabBoneEmbed
//@{
#define _IFT_TrabBoneEmbed_Name "trabboneembed"
#define _IFT_TrabBoneEmbed_eps0 "eps0"
#define _IFT_TrabBoneEmbed_nu0 "nu0"
//@}

namespace oofem {
/**
 * This class implements associated Material Status to TrabBoneEmbed.
 */
class TrabBoneEmbedStatus : public StructuralMaterialStatus
{
protected:
    double tempAlpha, alpha;
    double tempDam, dam;
    double tempPSED, psed;
    double tempTSED, tsed;
    FloatMatrix smtrx, matConstD;
    FloatArray densStress, tempPlasDef, plasDef, tempIncPlasDef;

public:
    TrabBoneEmbedStatus(GaussPoint * g);
    virtual ~TrabBoneEmbedStatus();

    void printOutputAt(FILE *file, TimeStep *tStep) override;

    double giveTempTSED();

    void setTempDam(double da) { tempDam = da; }
    void setTempTSED(double tse) { tempTSED = tse; }
    void setTempAlpha(double al) { tempAlpha = al; }
    void setTempPlasDef(FloatArray epsip) { tempPlasDef = epsip; }
    void setSmtrx(FloatMatrix smt) { smtrx = smt; }

    const FloatArray &givePlasDef() const { return plasDef; }

    const char *giveClassName() const override { return "TrabBoneEmbedStatus"; }

    void initTempStatus() override;
    void updateYourself(TimeStep *tStep) override;

    void saveContext(DataStream &stream, ContextMode mode) override;
    void restoreContext(DataStream &stream, ContextMode mode) override;
};


/**
 * Trabecular bone embedding material model.
 */
class TrabBoneEmbed : public StructuralMaterial
{
protected:
    double eps0, nu0;

public:
    TrabBoneEmbed(int n, Domain * d);

    void performPlasticityReturn(GaussPoint *gp, const FloatArray &totalStrain);

    double computeDamageParam(double alpha, GaussPoint *gp);

    double computeDamage(GaussPoint *gp, TimeStep *tStep);

    virtual void computeCumPlastStrain(double &alpha, GaussPoint *gp, TimeStep *tStep);

    /// Constructs the anisotropic compliance tensor.
    void constructIsoComplTensor(FloatMatrix &answer, const double eps0, const double nu0);

    void give3dMaterialStiffnessMatrix(FloatMatrix &answer,
                                       MatResponseMode mode, GaussPoint *gp,
                                       TimeStep *tStep) override;

    void giveRealStressVector_3d(FloatArray &answer, GaussPoint *gp,
                                 const FloatArray &reducedStrain, TimeStep *tStep) override;

    const char *giveInputRecordName() const override { return _IFT_TrabBoneEmbed_Name; }
    const char *giveClassName() const override { return "TrabBoneEmbed"; }

    IRResultType initializeFrom(InputRecord *ir) override;

    MaterialStatus *CreateStatus(GaussPoint *gp) const override;

    int giveIPValue(FloatArray &answer, GaussPoint *gp, InternalStateType type, TimeStep *tStep) override;
};
} // end namespace oofem
#endif
