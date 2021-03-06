#include "btkSimulateLRImageFilter.h"

namespace btk
{

SimulateLRImageFilter::SimulateLRImageFilter()
{
}
//-----------------------------------------------------------------------------------------------------------
SimulateLRImageFilter::~SimulateLRImageFilter()
{
}
//-----------------------------------------------------------------------------------------------------------
void SimulateLRImageFilter::Update()
{
    std::cout<<"Simulating LR images (= Hx)\n";


    //Compute H * x
    vnl_vector<float> Hx;
    m_H.mult(m_X,Hx);

    //resize the vector of simulated input LR images
    m_SimulatedOutputImages.resize(m_LRImages.size());

    //Temporary variables
    itkImage::IndexType lrIndex;  //index of the current voxel in the LR image
    unsigned int lrLinearIndex = 0;

    for(unsigned int i=0; i<m_LRImages.size(); i++)
    {
        //duplicate the LR input image into the simulated LR images to keep all header information
        itkDuplicator::Pointer duplicator = itkDuplicator::New();
        duplicator->SetInputImage( m_LRImages[i] );
        duplicator->Update();
        m_SimulatedOutputImages[i] = duplicator->GetOutput();
        m_SimulatedOutputImages[i]->FillBuffer(0);

        //Get the size of the current LR image
        itkImage::SizeType  lrSize  = m_LRImages[i]->GetLargestPossibleRegion().GetSize();

        //Instantiate an iterator over the current LR image
        itkIteratorWithIndex itLRImage(m_SimulatedOutputImages[i],m_SimulatedOutputImages[i]->GetLargestPossibleRegion());


        //Loop over the voxels of the current LR image
        for(itLRImage.GoToBegin(); !itLRImage.IsAtEnd(); ++itLRImage)
        {

            //Coordinate in the current LR image
            lrIndex = itLRImage.GetIndex();

            //Compute the corresponding linear index of lrIndex
            lrLinearIndex = m_Offset[i] + lrIndex[0] + lrIndex[1]*lrSize[0] + lrIndex[2]*lrSize[0]*lrSize[1];

            //Fill the simulated input LR image
            itLRImage.Set(Hx[lrLinearIndex]);
        }

        itkStatisticsImageFilter::Pointer statisticsImageFilter = itkStatisticsImageFilter::New ();
        statisticsImageFilter->SetInput(m_SimulatedOutputImages[i]);
        statisticsImageFilter->Update();
        std::cout << "Stat of the LR image y=Hx. \nMean: " << statisticsImageFilter->GetMean() << std::endl;
        std::cout << "Std.: " << statisticsImageFilter->GetSigma() << std::endl;
        std::cout << "Min: " << statisticsImageFilter->GetMinimum() << std::endl;
        std::cout << "Max: " << statisticsImageFilter->GetMaximum() << std::endl;
    }
}

}
