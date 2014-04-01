/*==========================================================================

  © Université de Strasbourg - Centre National de la Recherche Scientifique

  Date: 03/03/2011
  Author(s): Estanislao Oubel (oubel@unistra.fr)

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

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

/* Standard includes */
#include <tclap/CmdLine.h>

/* Itk includes */
#include "itkImage.h"
#include "itkImageFileReader.h"

/* Btk includes */
#include "btkDiffusionGradientTable.h"

int main( int argc, char *argv[] )
{
  try {

  // Parse arguments
  const char *inputImageFile = NULL, *gTableFile = NULL, *cTableFile = NULL; //, *dir = NULL;

  TCLAP::CmdLine cmd("Transform gradients from world to image coordinates. Can also swap directions or inverse directions (beware of the order of operations on directions)", ' ', "Unversioned");

  TCLAP::ValueArg<std::string> inputArg("i","input","Input image",true,"","string",cmd);
  TCLAP::ValueArg<std::string> gTableArg("g","gtable","Gradient table",true,"","string",cmd);
  TCLAP::ValueArg<std::string> cTableArg("c","ctable","Modified table",true,"","string",cmd);

  TCLAP::SwitchArg w2iSwitchArg("","w2i","Conversion is performed in the world to image direction",cmd,false);
  TCLAP::SwitchArg i2wSwitchArg("","i2w","Conversion is performed in the image to world direction",cmd,false);

  TCLAP::SwitchArg swapxySwitchArg("","swapxy","Swap X and Y diffusion directions",cmd,false);
  TCLAP::SwitchArg swapyzSwitchArg("","swapyz","Swap Y and Z diffusion directions",cmd,false);
  TCLAP::SwitchArg swapxzSwitchArg("","swapxz","Swap X and Z diffusion directions",cmd,false);

  TCLAP::SwitchArg invxSwitchArg("","invx","Inverse X diffusion directions",cmd,false);
  TCLAP::SwitchArg invySwitchArg("","invy","Inverse Y diffusion directions",cmd,false);
  TCLAP::SwitchArg invzSwitchArg("","invz","Inverse Z diffusion directions",cmd,false);


  cmd.parse( argc, argv );

  inputImageFile = inputArg.getValue().c_str();
  gTableFile     = gTableArg.getValue().c_str();
  cTableFile     = cTableArg.getValue().c_str();

  const    unsigned int    Dimension = 4;

  // Read image

  typedef itk::Image< short, Dimension >  ImageType;
  typedef itk::ImageFileReader< ImageType > ImageReaderType;
  ImageReaderType::Pointer  imageReader  = ImageReaderType::New();
  imageReader -> SetFileName(  inputImageFile );
  imageReader -> Update();
  ImageType::Pointer image = imageReader->GetOutput();

  ImageType::SizeType size = image -> GetLargestPossibleRegion().GetSize();
  ImageType::IndexType index = image -> GetLargestPossibleRegion().GetIndex();
  unsigned int numberOfFrames = size[3];

  // Modify gradient table

  typedef btk::DiffusionGradientTable< ImageType > GradientTableType;
  GradientTableType::Pointer gradientTable = GradientTableType::New();

  gradientTable -> SetNumberOfGradients(numberOfFrames);
  gradientTable -> SetImage( image );
  gradientTable -> LoadFromFile( gTableFile);

  //Change coordinate system ....................................
  if ( i2wSwitchArg.isSet() )
    gradientTable -> TransformGradientsToWorldCoordinates();
  if ( w2iSwitchArg.isSet() )
    gradientTable -> TransformGradientsToImageCoordinates();

  //Swap directions .............................................
  if ( swapxySwitchArg.isSet() )
    gradientTable -> SwapXYDirections();
  if ( swapyzSwitchArg.isSet() )
    gradientTable -> SwapYZDirections();
  if ( swapxzSwitchArg.isSet() )
    gradientTable -> SwapXZDirections();

  //Inverse directions ..........................................
  if ( invxSwitchArg.isSet() )
    gradientTable -> InverseXDirection();
  if ( invySwitchArg.isSet() )
    gradientTable -> InverseYDirection();
  if ( invzSwitchArg.isSet() )
    gradientTable -> InverseZDirection();

  gradientTable -> SaveToFile( cTableFile);


  } catch (TCLAP::ArgException &e)  // catch any exceptions
  { std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

  return EXIT_SUCCESS;
}

