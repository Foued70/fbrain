/*==========================================================================

  © Université de Strasbourg - Centre National de la Recherche Scientifique

  Date: 22/03/2012
  Author(s): Schweitzer Marc (marc.schweitzer@unistra.fr)

  This software is governed by the CeCILL-B license under French law and
  abiding by the rules of distribution of free software.  You can  use,
  modify and/ or redistribute the software under the terms of the CeCILL-B
  license as circulated by CEA, CNRS and INRIA at the following URL
  "http://www.cecill.info".

  As a counterpart to the access to the source code and  rights to copy,
  modify and redistribute granted by the license, users are provided only
  with a limited warranty  and the software's author,  the holder of the
  economic rights,  and the successive licensors  have only  limited
  liability.

  In this respect, the user's attention is drawn to the risks associated
  with loading,  using,  modifying and/or developing or reproducing the
  software by the user in light of its specific status of free software,
  that may mean  that it is complicated to manipulate,  and  that  also
  therefore means  that it is reserved for developers  and  experienced
  professionals having in-depth computer knowledge. Users are therefore
  encouraged to load and test the software's suitability as regards their
  requirements in conditions enabling the security of their systems and/or
  data to be ensured and,  more generally, to use and operate it in the
  same conditions as regards security.

  The fact that you are presently reading this means that you have had
  knowledge of the CeCILL-B license and that you accept its terms.

==========================================================================*/

#ifndef __BTK_MOTIONCORRECTIONFILTER_H__
#define __BTK_MOTIONCORRECTIONFILTER_H__

/* ITK */
#include "itkImage.h"
#include "itkImageMaskSpatialObject.h"
#include "itkIdentityTransform.h"
#include "itkTransformFactory.h"
#include "itkAffineTransform.h"
#include "itkEuler3DTransform.h"

/* BTK */
#include "btkMacro.h"
#include "btkSuperResolutionType.h"

/* OTHERS */
#include "iostream"


namespace btk
{
/**
 * @class MotionCorrectionFilter
 * @brief The MotionCorrectionFilter class is a base class for MotionCorrection
 * @author Marc Schweitzer
 * @ingroup SuperResolution
 */
class MotionCorrectionFilter
{
public:


    MotionCorrectionFilter(){};
    ~MotionCorrectionFilter(){};

     virtual void Update() = 0;


    // GETTER/SETTER :


    btkGetMacro(ImagesLR,std::vector< itkImage::Pointer > );
    btkSetMacro(ImagesLR,std::vector< itkImage::Pointer > );


    btkGetMacro(ImagesMaskLR,std::vector< itkImageMask::Pointer >  );
    btkSetMacro(ImagesMaskLR,std::vector< itkImageMask::Pointer >  );

    btkGetMacro(MasksLR,std::vector< itkMask::Pointer >);
    btkSetMacro(MasksLR,std::vector< itkMask::Pointer >);

    btkGetMacro(ImageHR, itkImage::Pointer);
    btkSetMacro(ImageHR, itkImage::Pointer);

    btkGetMacro(ImageMaskHR, itkImageMask::Pointer);
    btkSetMacro(ImageMaskHR, itkImageMask::Pointer);

    btkGetMacro(ReferenceImage, itkImage::Pointer);
    btkSetMacro(ReferenceImage, itkImage::Pointer);

    btkGetMacro(TransformsLR, std::vector< itkTransformBase::Pointer >);
    btkSetMacro(TransformsLR, std::vector< itkTransformBase::Pointer >);





protected:

    virtual void Initialize() = 0;
    virtual void DoRegistration() = 0;

    std::vector< itkImage::Pointer >         m_ImagesLR;
    std::vector< itkImageMask::Pointer >     m_ImagesMaskLR;
    std::vector< itkMask::Pointer >          m_MasksLR;
    std::vector< itkTransformBase::Pointer>  m_TransformsLR;
    itkImage::Pointer                        m_ImageHR;
    itkImage::Pointer                        m_OutputHRImage;
    itkImageMask::Pointer                    m_ImageMaskHR;
    itkMask::Pointer                         m_MaskHR;
    itkImage::Pointer                        m_ReferenceImage;



private:


};
}

#endif
