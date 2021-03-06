/*
Copyright or © or Copr. Université de Strasbourg - Centre National de la Recherche Scientifique

14/08/2012
rousseau@unistra.fr

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

#ifndef __BTKHISTOGRAM_CXX__
#define __BTKHISTOGRAM_CXX__

#include "btkHistogram.h"

namespace btk
{    
Histogram::Histogram()
{
    m_NumberOfSamples = 0;
    m_NumberOfBins = 0;
}

Histogram::~Histogram()
{

}

void Histogram::SetNumberOfBins(unsigned int numberOfBins)
{
  m_Data.resize(numberOfBins);
  m_NormalizedData.resize(numberOfBins);
  m_NumberOfBins = m_Data.size();
}

//This function computes the linear coefficients used to convert bin to values and m_widthOfBin
void Histogram::Setup()
{
    m_ACoefficient = (m_NumberOfBins-1) * 1.0 / (m_UpperBound - m_LowerBound);
    m_BCoefficient = - m_ACoefficient * m_LowerBound;
    m_WidthOfBin   = m_UpperBound / (m_NumberOfBins - 1) ;
}


void Histogram::AddSample(float sample)
{
    //No boundary checking !
    unsigned int bin = (unsigned int) (sample * m_ACoefficient + m_BCoefficient);
    m_Data[bin]++;
    m_NumberOfSamples++;
}


void Histogram::RemoveSample(float sample)
{
    //No boundary checking !
    unsigned int bin = (unsigned int) (sample * m_ACoefficient + m_BCoefficient);
    m_Data[bin]--;
    m_NumberOfSamples--;
}

void Histogram::AddWeightedSample(float sample, float weight)
{
    //No boundary checking !
    unsigned int bin = (unsigned int) (sample * m_ACoefficient + m_BCoefficient);
    m_Data[bin] += weight;
    m_NumberOfSamples += weight;
}


void Histogram::RemoveWeightedSample(float sample, float weight)
{
    //No boundary checking !
    unsigned int bin = (unsigned int) (sample * m_ACoefficient + m_BCoefficient);
    m_Data[bin] -= weight;
    m_NumberOfSamples -= weight;
}

float Histogram::BinToValue(unsigned int bin)
{
    return ( bin*(m_UpperBound - m_LowerBound)/m_NumberOfBins + m_LowerBound ) + m_WidthOfBin / 2.0;
}


void Histogram::ComputeCumulativeDistributionFunction()
{
    m_CumulativeDistributionFunction.resize(m_NumberOfBins);
    m_CumulativeDistributionFunction[0] = m_Data[0];
    for(unsigned int i=1; i<m_NumberOfBins; i++)
    {
        m_CumulativeDistributionFunction[i] = m_Data[i] + m_CumulativeDistributionFunction[i-1];
    }
}


void Histogram::ComputeInverseCumulativeDistributionFunction()
{
    m_InverseCumulativeDistributionFunction.resize(m_NumberOfSamples);
    m_InverseCumulativeDistributionFunction[0] = 0;
    unsigned int currentBin = 0;
    
    for(unsigned int i=1; i<m_NumberOfSamples+1; i++)
    {
        m_InverseCumulativeDistributionFunction[i] = currentBin;
        for(unsigned int bin=currentBin; bin<m_NumberOfBins; bin++)
        {
            if( m_CumulativeDistributionFunction[bin] >= i )
            {
                currentBin = bin;
                m_InverseCumulativeDistributionFunction[i] = bin;
                break;
            }
        }
    }

}

//NORMALIZED VERSIONS -----------------------------------------------------------
void Histogram::NormalizeData()
{
    for(unsigned int i=0; i<m_NumberOfBins; i++)
    {
        m_NormalizedData[i] = (double)(m_Data[i]) / m_NumberOfSamples * (m_SampleQuantification-1);
    }
}

void Histogram::ComputeNormalizedCumulativeDistributionFunction()
{
    m_NormalizedCumulativeDistributionFunction.resize(m_NumberOfBins);
    m_NormalizedCumulativeDistributionFunction[0] = m_NormalizedData[0];
    for(unsigned int i=1; i<m_NumberOfBins; i++)
    {
        m_NormalizedCumulativeDistributionFunction[i] = m_NormalizedData[i] + m_NormalizedCumulativeDistributionFunction[i-1];
    }
}


void Histogram::ComputeNormalizedInverseCumulativeDistributionFunction()
{
    m_NormalizedInverseCumulativeDistributionFunction.resize(m_SampleQuantification);
    m_NormalizedInverseCumulativeDistributionFunction[0] = 0;
    unsigned int currentBin = 0;
    
    for(unsigned int i=1; i<m_SampleQuantification; i++)
    {
        m_NormalizedInverseCumulativeDistributionFunction[i] = currentBin;
        for(unsigned int bin=currentBin; bin<m_NumberOfBins; bin++)
        {
            if( m_NormalizedCumulativeDistributionFunction[bin] >= i )
            {
                currentBin = bin;
                m_NormalizedInverseCumulativeDistributionFunction[i] = bin;
                break;
            }
        }
    }

}

//SAVE INTO TEXT FILES -----------------------------------------------------------

void Histogram::SaveHistogram(const std::string & filename)
{
    std::cout<<"Saving histogram in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_NumberOfBins; i++)
    {
        float value = BinToValue(i);
        file << value << " " << m_Data[i] << std::endl;
    }
    file.close();
}

void Histogram::SaveNormalizedHistogram(const std::string & filename)
{
    std::cout<<"Saving normalized histogram in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_NumberOfBins; i++)
    {
        float value = BinToValue(i);
        file << value << " " << m_NormalizedData[i] << std::endl;
    }
    file.close();
}

void Histogram::SaveCumulativeDistributionFunction(const std::string & filename)
{
    std::cout<<"Saving cdf in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_NumberOfBins; i++)
    {
        float value = BinToValue(i);
        file << value << " " << m_CumulativeDistributionFunction[i] << std::endl;
    }
    file.close();
}
void Histogram::SaveNormalizedCumulativeDistributionFunction(const std::string & filename)
{
    std::cout<<"Saving normalized cdf in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_NumberOfBins; i++)
    {
        float value = BinToValue(i);
        file << value << " " << m_NormalizedCumulativeDistributionFunction[i] << std::endl;
    }
    file.close();
}
void Histogram::SaveInverseCumulativeDistributionFunction(const std::string & filename)
{
    std::cout<<"Saving icdf in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_NumberOfSamples; i++)
    {
        file << i << " " << m_InverseCumulativeDistributionFunction[i] << std::endl;
    }
    file.close();
}
void Histogram::SaveNormalizedInverseCumulativeDistributionFunction(const std::string & filename)
{
    std::cout<<"Saving normalized icdf in "<<filename<<std::endl;
    std::ofstream file;
    file.open(filename.c_str());
    for(unsigned int i=0; i<m_SampleQuantification; i++)
    {
        file << i << " " << m_NormalizedInverseCumulativeDistributionFunction[i] << std::endl;
    }
    file.close();
}


}
#endif // btkHistogram_CXX
