/*==========================================================================
  
  © Université de Strasbourg - Centre National de la Recherche Scientifique
  
  Date: 
  Author(s):Marc Schweitzer (marc.schweitzer(at)unistra.fr)
  
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

#ifndef BTKSUPERRESOLUTIONCOSTFUNCTIONVNLWRAPPER_HXX
#define BTKSUPERRESOLUTIONCOSTFUNCTIONVNLWRAPPER_HXX

#include "vnl/vnl_cost_function.h"

#include "btkSuperResolutionCostFunction.hxx"

namespace btk
{

template < class TImage >
class SuperResolutionCostFunctionVNLWrapper : public vnl_cost_function
{
  public:
    /** typedefs  */
    typedef TImage                                          ImageType;
    typedef typename ImageType::PixelType                   VoxelType;

    typedef  btk::SuperResolutionCostFunction< ImageType >  CostFunctionType;

    public:
        /** Constructor */
        SuperResolutionCostFunctionVNLWrapper(unsigned int dim);
        SuperResolutionCostFunctionVNLWrapper(){}

        /** Destructor */
        ~SuperResolutionCostFunctionVNLWrapper();

        /** Cost Function */
        double f(const vnl_vector< double > &x);

        /** Get Gradient */
        virtual void gradf(vnl_vector< double > const& x, vnl_vector< double >& gradient);

        /** Get a pointer to the btk Cost function */
        btkGetMacro(CostFunction, typename CostFunctionType::Pointer);

    protected:


    private:

        typename CostFunctionType::Pointer m_CostFunction;

};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "btkSuperResolutionCostFunctionVNLWrapper.txx"
#endif



#endif // BTKSUPERRESOLUTIONCOSTFUNCTIONVNLWRAPPER_HXX
