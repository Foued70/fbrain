/*==========================================================================
  
  © Université de Strasbourg - Centre National de la Recherche Scientifique
  
  Date: 09/01/2013
  Author(s): Julien Pontabry (pontabry@unistra.fr)
  
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

#include "btkNadarayaWatsonReconstructionErrorFunction.h"


// STL includes
#include "numeric"


namespace btk
{

NadarayaWatsonReconstructionErrorFunction::NadarayaWatsonReconstructionErrorFunction() : m_BandwidthMatrix(NULL), Superclass()
{
    // ----
}

//----------------------------------------------------------------------------------------

void NadarayaWatsonReconstructionErrorFunction::ActivateParameters(unsigned int index)
{
    unsigned int index2 = index + m_NumberOfParameters;
    unsigned int index3 = index + m_TwoTimeNumberOfParameters;

    #pragma omp parallel for default(shared) schedule(dynamic)
    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        (*m_CurrentParameters)(index, j) = (*m_InputParameters)(index,j);
        (*m_CurrentParameters)(index2,j) = (*m_InputParameters)(index2,j);
        (*m_CurrentParameters)(index3,j) = (*m_InputParameters)(index3,j);
    }

    m_numberOfActivatedParameters++;
}

//----------------------------------------------------------------------------------------

void NadarayaWatsonReconstructionErrorFunction::DesactivateParameters(unsigned int index)
{
    unsigned int index2 = index + m_NumberOfParameters;
    unsigned int index3 = index + m_TwoTimeNumberOfParameters;

    #pragma omp parallel for default(shared) schedule(dynamic)
    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        (*m_CurrentParameters)(index, j) = 0;
        (*m_CurrentParameters)(index2,j) = 0;
        (*m_CurrentParameters)(index3,j) = 0;
    }

    m_numberOfActivatedParameters--;
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::Evaluate()
{
    // Compute the cost for each vector
    double value = 0;

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        vnl_vector< double > v = m_CurrentParameters->get_column(j);

        value = value + m_ImagesWeightVector->get(j) * ( m_InputParameters->get_column(j) - this->NadarayaWatsonMultivariateKernelEstimator(*m_CurrentParameters, v, j) ).squared_magnitude();
    }

    return value;
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::ComputeMeanParameterError()
{
    double meanError = 0.0;

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        vnl_vector< double > v = m_CurrentParameters->get_column(j);
        vnl_vector< double > f = this->NadarayaWatsonMultivariateKernelEstimator(*m_CurrentParameters, v, j);

        for(unsigned int i = 0; i < m_NumberOfParameters; i++)
        {
            double error1 = (*m_InputParameters)(i,j) - f(i);
            double error2 = (*m_InputParameters)(i+m_NumberOfParameters,j) - f(i+m_NumberOfParameters);
            double error3 = (*m_InputParameters)(i+m_TwoTimeNumberOfParameters,j) - f(i+m_TwoTimeNumberOfParameters);

            meanError += std::sqrt( error1*error1 + error2*error2 + error3*error3 );
        }
    }

    meanError /= m_NumberOfParameters*m_NumberOfVectors;

    return meanError;
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::FunctionEvaluate(double h)
{
    m_BandwidthMatrixInverse.fill_diagonal(1.0/h);

    return this->Evaluate();
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::GradientEvaluate(double h)
{
    double delta = 0.1/2.0;

    // Use the finite centered difference with step=1 to approximage gradient
    double fPlus = this->FunctionEvaluate(h+delta);
    double fMinus = this->FunctionEvaluate(h-delta);

    return (fPlus - fMinus) / delta;
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::EvaluateActivation(unsigned int activatedIndex)
{
    unsigned int activatedIndex2 = activatedIndex + m_NumberOfParameters;
    unsigned int activatedIndex3 = activatedIndex + m_TwoTimeNumberOfParameters;


    // Compute the cost for each vector
    double value = 0;

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        vnl_vector< double > v = m_CurrentParameters->get_column(j);

        // Activate parameter
        v(activatedIndex)  = (*m_InputParameters)(activatedIndex,j);
        v(activatedIndex2) = (*m_InputParameters)(activatedIndex2,j);
        v(activatedIndex3) = (*m_InputParameters)(activatedIndex3,j);

        value = value + m_ImagesWeightVector->get(j) * ( m_InputParameters->get_column(j) - this->NadarayaWatsonMultivariateKernelEstimatorActivation(activatedIndex, v, j) ).squared_magnitude();
    }

    return value;
}

//----------------------------------------------------------------------------------------

double NadarayaWatsonReconstructionErrorFunction::EvaluateDesactivation(unsigned int desactivatedIndex)
{
    unsigned int desactivatedIndex2 = desactivatedIndex + m_NumberOfParameters;
    unsigned int desactivatedIndex3 = desactivatedIndex + m_TwoTimeNumberOfParameters;


    // Compute the cost for each vector
    double value = 0;

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        vnl_vector< double > v = m_CurrentParameters->get_column(j);

        // Activate parameter
        v(desactivatedIndex)  = 0;
        v(desactivatedIndex2) = 0;
        v(desactivatedIndex3) = 0;

        value = value + m_ImagesWeightVector->get(j) * ( m_InputParameters->get_column(j) - this->NadarayaWatsonMultivariateKernelEstimatorDesactivation(desactivatedIndex, v, j) ).squared_magnitude();
    }


    return value;
}

//----------------------------------------------------------------------------------------

vnl_matrix< double > NadarayaWatsonReconstructionErrorFunction::GetResiduals()
{
    vnl_matrix< double > residuals(m_InputParameters->rows(),m_InputParameters->columns());
    residuals.fill(0.0);

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        vnl_vector< double > v = m_CurrentParameters->get_column(j);
        residuals.set_column(j, m_InputParameters->get_column(j) - this->NadarayaWatsonMultivariateKernelEstimator(*m_CurrentParameters, v, j) * m_ImagesWeightVector->get(j) );
    }

    return residuals;
}

//----------------------------------------------------------------------------------------

vnl_matrix< double > &NadarayaWatsonReconstructionErrorFunction::GetReconstruction(vnl_matrix< double > &parameters)
{
    vnl_matrix< double > *reconstruction_ptr = new vnl_matrix< double >(m_InputParameters->rows(),parameters.columns());
    vnl_matrix< double >     &reconstruction = *reconstruction_ptr;

    reconstruction.fill(0.0);

    for(unsigned int j = 0; j < reconstruction.columns(); j++)
    {
        vnl_vector< double > v = parameters.get_column(j);
        reconstruction.set_column(j, this->NadarayaWatsonMultivariateKernelEstimator(*m_CurrentParameters, v, j) * m_ImagesWeightVector->get(j) );
    }

    return reconstruction;
}

//----------------------------------------------------------------------------------------

void NadarayaWatsonReconstructionErrorFunction::Initialize()
{
    Superclass::Initialize();

    // Compute determinant and inverse of bandwidth diagonal matrix.
    if(m_BandwidthMatrix != NULL)
    {
        m_BandwidthMatrixInverse = vnl_diag_matrix< double >(m_BandwidthMatrix->get_diagonal());
        m_BandwidthMatrixInverse.invert_in_place();
    }

    // Compute the coefficient of the gaussian kernel
    m_GaussianCoefficient = (1.0 / sqrt(2.0 * M_PI));
}

//----------------------------------------------------------------------------------------

std::string NadarayaWatsonReconstructionErrorFunction::OptimizeParameters()
{
    // Initialisation of gradient descent parameters
    double h = m_BandwidthMatrix->get(0,0); // Parameter to optimise

    double     initialStep = 1e-1;  // Initial step of the gradient descent
    double         stepMin = 1e-2;  // Minimal step
    double         stepMax = 0.5;   // Maximal step
    double    stepIncrease = 1e-2;  // Step increase for the linear search
    int maxNumOfIterations = 30;    // Maximum number of iterations
    int   currentIteration = 1;     // Current iteration number
    double         epsilon = 1e-2;  // Threshold of gradient magnitude
    double              m1 = 0.1;   // First constant of Wolfe's conditions
    double              m2 = 0.9;   // Second constant of Wolfe's conditions

    double  gradient = this->GradientEvaluate(h);
    double direction = (gradient < 0.0) ? 1 : -1;

//    btkCoutVariable(currentIteration);
//    btkCoutVariable(h);
//    btkCoutVariable(gradient);
//    btkCoutVariable(direction);
//    btkCoutVariable(initialStep);
//    std::cerr << std::endl;

    while(currentIteration <= maxNumOfIterations && std::abs(gradient) > epsilon)
    {
        // Search for a good step
        bool  firstWolfeCondition = false;
        bool secondWolfeCondition = false;
        double               step = initialStep;
        double                  f = this->FunctionEvaluate(h);

        do
        {
            firstWolfeCondition  = false;
            secondWolfeCondition = false;

            double delta = step*direction;

            firstWolfeCondition = this->FunctionEvaluate(h+delta) <= f + m1*delta*gradient;

            if(firstWolfeCondition)
            {
                secondWolfeCondition = direction*this->GradientEvaluate(h+delta) >= m2*direction*gradient;

                if(!secondWolfeCondition)
                    step += stepIncrease;
            }
            else // !firstWolfeCondition
                step -= stepIncrease;
        }
        while((!firstWolfeCondition || !secondWolfeCondition) && (step > stepMin && step < stepMax));

        h += step*direction;
        gradient = this->GradientEvaluate(h);
        direction = (gradient < 0) ? 1 : -1;
        currentIteration++;

//        btkCoutVariable(currentIteration);
//        btkCoutVariable(h);
//        btkCoutVariable(gradient);
//        btkCoutVariable(direction);
//        btkCoutVariable(step);
//        std::cerr << std::endl;
    }

    m_BandwidthMatrixInverse.fill_diagonal(1.0/h);

    std::stringstream message;
    message << "\tOptimized bandwidth: " << h;

    return message.str();
}

//----------------------------------------------------------------------------------------

vnl_vector< double > NadarayaWatsonReconstructionErrorFunction::NadarayaWatsonMultivariateKernelEstimator(vnl_matrix< double > &data, vnl_vector< double > &v, unsigned int currentSampleJ)
{
    vnl_vector< double > estimate(m_InputParameters->rows(), 0.0);

    double sumOfWeights = 0;

    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        if(j != currentSampleJ)
        {
            double weight = this->MultivariateGaussianKernel(m_BandwidthMatrixInverse * (v - data.get_column(j)));

            sumOfWeights += weight;

            estimate += m_InputParameters->get_column(j) * weight;
        }
    }

    estimate /= sumOfWeights;

    return estimate;
}

//----------------------------------------------------------------------------------------

vnl_vector< double > NadarayaWatsonReconstructionErrorFunction::NadarayaWatsonMultivariateKernelEstimatorActivation(unsigned int activatedIndex, vnl_vector< double > &v, unsigned int currentSampleJ)
{
    vnl_vector< double > estimate(v.size(), 0.0);

    double sumOfWeights = 0;

    unsigned int activatedIndex2 = activatedIndex + m_NumberOfParameters;
    unsigned int activatedIndex3 = activatedIndex + m_TwoTimeNumberOfParameters;


    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        if(j != currentSampleJ)
        {
            vnl_vector< double > data = m_CurrentParameters->get_column(j);

            // Activate parameter
            data(activatedIndex)  = (*m_InputParameters)(activatedIndex,j);
            data(activatedIndex2) = (*m_InputParameters)(activatedIndex2,j);
            data(activatedIndex3) = (*m_InputParameters)(activatedIndex3,j);

            double weight = this->MultivariateGaussianKernel(m_BandwidthMatrixInverse * (v - data));

            sumOfWeights += weight;

            estimate += m_InputParameters->get_column(j) * weight;
        }
    }

    estimate /= sumOfWeights;

    return estimate;
}

//----------------------------------------------------------------------------------------

vnl_vector< double > NadarayaWatsonReconstructionErrorFunction::NadarayaWatsonMultivariateKernelEstimatorDesactivation(unsigned int desactivatedIndex, vnl_vector< double > &v, unsigned int currentSampleJ)
{
    vnl_vector< double > estimate(v.size(), 0.0);

    double sumOfWeights = 0;

    unsigned int desactivatedIndex2 = desactivatedIndex + m_NumberOfParameters;
    unsigned int desactivatedIndex3 = desactivatedIndex + m_TwoTimeNumberOfParameters;


    for(unsigned int j = 0; j < m_NumberOfVectors; j++)
    {
        if(j != currentSampleJ)
        {
            vnl_vector< double > data = m_CurrentParameters->get_column(j);

            // Desactivate parameter
            data(desactivatedIndex)  = 0;
            data(desactivatedIndex2) = 0;
            data(desactivatedIndex3) = 0;

            double weight = this->MultivariateGaussianKernel(m_BandwidthMatrixInverse * (v - data));

            sumOfWeights += weight;

            estimate += m_InputParameters->get_column(j) * weight;
        }
    }

    estimate /= sumOfWeights;

    return estimate;
}

//----------------------------------------------------------------------------------------

inline double NadarayaWatsonReconstructionErrorFunction::MultivariateGaussianKernel(vnl_vector< double > v)
{
    return this->GaussianKernel(v.two_norm());
}

//----------------------------------------------------------------------------------------

inline double NadarayaWatsonReconstructionErrorFunction::GaussianKernel(double v)
{
    return m_GaussianCoefficient * std::exp( -0.5 * v*v );
}

} // namespace btk
