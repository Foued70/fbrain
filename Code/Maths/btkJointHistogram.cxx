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

#ifndef __BTKJOINTHISTOGRAM_CXX__
#define __BTKJOINTHISTOGRAM_CXX__

#include "btkJointHistogram.h"

namespace btk
{    

JointHistogram::JointHistogram()
{
  m_NumberOfSamples = 0;
  this->SetNumberOfBins(64,64);
}

JointHistogram::~JointHistogram()
{

}

void JointHistogram::ClearJointHistogram()
{
  m_NumberOfSamples = 0;
  m_Data.fill(0.0);
}

void JointHistogram::SetNumberOfBins(unsigned int nx, unsigned int ny)
{
  m_Data.set_size(nx,ny);
  m_Data.fill(0.0);
  m_NumberOfSamples = 0;
}

unsigned int JointHistogram::GetNumberOfBinsX()
{
  return m_Data.rows();
}

unsigned int JointHistogram::GetNumberOfBinsY()
{
  return m_Data.columns();
}

void JointHistogram::AddSample(double x, double y, double w=1.0)
{
  //NO TEST !
  //x and y have to be in range of bins!
  int i = (int)(x*this->m_Ax+this->m_Bx);
  int j = (int)(y*this->m_Ay+this->m_By);
  m_Data(i,j) += w;
  m_NumberOfSamples += w;
}

double JointHistogram::MutualInformation()
{
  return this->EntropyX() + this->EntropyY() - this->JointEntropy();
}

double JointHistogram::NormalizedMutualInformation()
{
  return (this->EntropyX() + this->EntropyY()) / this->JointEntropy();
}

double JointHistogram::EntropyX()
{
  double res = 0.0;
  double pi = 0;
  for(unsigned int i=0; i < m_Data.rows(); i++)
  {
    pi = m_Data.get_row(i).sum();
    if(pi > 0)
      res += pi * log(pi);
  }
  return - res / m_NumberOfSamples + log(m_NumberOfSamples);
}

double JointHistogram::EntropyY()
{
  double res = 0.0;
  double pj = 0;
  for(unsigned int j=0; j < m_Data.columns(); j++)
  {
    pj = m_Data.get_column(j).sum();
    if(pj > 0)
      res += pj * log(pj);
  }
  return - res / m_NumberOfSamples + log(m_NumberOfSamples);
}

double JointHistogram::JointEntropy()
{
  double res = 0.0;
  for(unsigned int i=0; i < m_Data.rows(); i++)
    for(unsigned int j=0; j < m_Data.columns(); j++)
      if(m_Data(i,j) > 0)
        res += m_Data(i,j) * log(m_Data(i,j));

  return - res / m_NumberOfSamples + log(m_NumberOfSamples);
}


}
#endif // btkJointHistogram_CXX
