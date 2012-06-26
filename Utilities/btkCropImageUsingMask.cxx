/*
Copyright or © or Copr. Université de Strasbourg - Centre National de la Recherche Scientifique

16 march 2011
rousseau @ unistra . fr

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
*/

/* Standard includes */
#include <tclap/CmdLine.h>
#include "string"
#include "iomanip"

/* Itk includes */
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImage.h"
#include "itkCropImageFilter.h"

/* Btk includes */
#include "btkImageHelper.h"



template<unsigned int imageDimension>
void CropImageUsingMask(std::string input_file, std::string output_file, std::string mask_file)
{
    //ITK declaration
    typedef short PixelType;
    typedef itk::Image< PixelType, imageDimension > ImageType;
    typedef itk::ImageFileReader< ImageType >  ReaderType;
    typedef itk::ImageFileWriter< ImageType >  WriterType;

    typedef itk::Image< PixelType, 3 >         MaskType;
    typedef itk::ImageFileReader< MaskType >  MaskReaderType;


    //Reading the input image
    typename ImageType::Pointer image = btk::ImageHelper<ImageType>::ReadImage(input_file);


    //Reading the mask
    MaskType::Pointer mask = btk::ImageHelper<MaskType>::ReadImage(mask_file);


    typedef itk::CropImageFilter< ImageType, ImageType >  CropImageFilterType;
    typename CropImageFilterType::Pointer cropFilter = CropImageFilterType::New();

    //find bounding box using the mask image
    typename ImageType::RegionType region;
    region.SetSize(image->GetLargestPossibleRegion().GetSize());
    typename ImageType::SizeType imageSize = region.GetSize();
    typename MaskType::IndexType pixelIndex;
    typename ImageType::SizeType downSize, upSize;

    for(unsigned int i=0; i<imageDimension; i++)
    {
        downSize[i] = imageSize[i]-1;
        upSize[i] = 0;
    }

    //Looking for the size of the bounding box using the mask
    for(unsigned int k=0;k<imageSize[2];k++)
        for(unsigned int j=0;j<imageSize[1];j++)
            for(unsigned int i=0;i<imageSize[0];i++)
            {
                pixelIndex[0] = i;
                pixelIndex[1] = j;
                pixelIndex[2] = k;
                if(mask->GetPixel(pixelIndex) != 0)
                {
                    if(i<downSize[0]) downSize[0] = i;
                    if(j<downSize[1]) downSize[1] = j;
                    if(k<downSize[2]) downSize[2] = k;
                    if(i>upSize[0]) upSize[0] = i;
                    if(j>upSize[1]) upSize[1] = j;
                    if(k>upSize[2]) upSize[2] = k;
                }
            }

    if(imageDimension == 4)
    {
        downSize[3]= 0;
        upSize[3]  = imageSize[3]-1;
    }

    std::cout<<"Bounding box : ("<<downSize[0]<<","<<downSize[1]<<","<<downSize[2]<<") (";
    std::cout<<upSize[0]<<","<<upSize[1]<<","<<upSize[2]<<")"<<std::endl;

    for(unsigned int i=0; i<imageDimension; i++)
    {
        upSize[i] = imageSize[i] -1 - upSize[i];
    }

    cropFilter->SetInput( image );
    cropFilter->SetLowerBoundaryCropSize( downSize );
    cropFilter->SetUpperBoundaryCropSize( upSize );
    cropFilter->UpdateLargestPossibleRegion();

    btk::ImageHelper<ImageType>::WriteImage(cropFilter->GetOutput(),output_file);

}


int main(int argc, char** argv)
{
    try {

        TCLAP::CmdLine cmd("btkCropImageUsingMask: Crop an image (3D or 4D) using a 3D mask (non-zero values)", ' ', "1.0", true);

        TCLAP::ValueArg<std::string> inputImageArg("i","image_file","input image file (short)",true,"","string", cmd);
        TCLAP::ValueArg<std::string> outputImageArg("o","output_file","output image file (short)",true,"","string", cmd);
        TCLAP::ValueArg<std::string> inputMaskArg("m","mask_file","filename of the mask image (dimension = 3)",false,"","string", cmd);
        TCLAP::ValueArg< int > dimArg("d","dim","image dimension of the input image (default is 3)",false,3,"int", cmd);

        // Parse the args.
        cmd.parse( argc, argv );

        // Get the value parsed by each arg.
        std::string input_file       = inputImageArg.getValue();
        std::string output_file      = outputImageArg.getValue();
        std::string mask_file        = inputMaskArg.getValue();
        unsigned int dim             = (unsigned int) dimArg.getValue();

        switch (dim)
        {
        case 3:
            CropImageUsingMask<3>(input_file, output_file, mask_file);
            break;
        case 4:
            CropImageUsingMask<4>(input_file, output_file, mask_file);
            break;
        default:
            std::cerr << "unsupported dimension" << std::endl;
            exit( EXIT_FAILURE );
        }

        return EXIT_SUCCESS;

    }

    catch (TCLAP::ArgException &e)  // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}




