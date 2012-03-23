/*
Copyright or © or Copr. Université de Strasbourg - Centre National de la Recherche Scientifique

22 march 2010
< pontabry at unistra dot fr >

This software is governed by the CeCILL-B license under French law and
abiding by the rules of distribution of free software. You can use,
modify and/ or redistribute the software under the terms of the CeCILL-B
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info".

As a counterpart to the access to the source code and rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty and the software's author, the holder of the
economic rights, and the successive licensors have only limited
liability.

In this respect, the user's attention is drawn to the risks associated
with loading, using, modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean that it is complicated to manipulate, and that also
therefore means that it is reserved for developers and experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or
data to be ensured and, more generally, to use and operate it in the
same conditions as regards security.

The fact that you are presently reading this means that you have had
knowledge of the CeCILL-B license and that you accept its terms.
*/

#ifndef BTK_TYPES_H
#define BTK_TYPES_H

    // ITK includes
    #include "itkImage.h"
    #include "itkImageRegion.h"
    #include "itkImageRegionIterator.h"
    #include "itkImageRegionIteratorWithIndex.h"
    #include "itkImageFileReader.h"
    #include "itkImageFileWriter.h"
    #include "itkBSplineInterpolateImageFunction.h"
    #include "itkVariableSizeMatrix.h"
    #include "itkContinuousIndex.h"
    #include "itkResampleImageFilter.h"
    #include "itkNearestNeighborInterpolateImageFunction.h"

    // STL includes
    #include "cfloat"
    #include "cmath"
    #include "vector"


    namespace btk
    {
        /**
         * Display a variable and its value on standard error output.
         * @param x Variable to display
         */
        #define Pr(x) std::cerr << #x << " = " << (x) << std::endl

        /**
         * Display information in level one display mode.
         * @param mode Mode in which we are
         * @param inst Instruction to display
         */
        #define Display1(mode,inst) if(mode > 0) { inst; }

        /**
         * Display information in level two display mode.
         * @param mode Mode in which we are
         * @param inst Instruction to display
         */
        #define Display2(mode,inst) if(mode > 1) { inst; }

        /**
          * Real type
          */
        typedef double Real;

        /**
         * Double epsilon
         */
        const Real EPSILON  = DBL_EPSILON;

        /**
         * Double maximum
         */
        const Real MAX_REAL = DBL_MAX;

        /**
         * Double minimum
         */
         const Real MIN_REAL = DBL_MIN;

        /**
         * Equality test on floating point numbers, according to the machine precision.
         * @param a Left operand
         * @param b Right operand
         * @return True if a=b, false otherwise
         */
        inline bool EQUAL(Real a, Real b, Real epsilon=EPSILON)
        {
            Real diff = std::abs(a-b);

            return (diff < epsilon);
        }

        /**
         * Round a number with the standard rules.
         * @param x number to round
         * @return The number rounded
         */
        inline Real round(Real x)
        {
            return std::floor(x + 0.5);
        }

        typedef float Float;

        /**
         * ITK matrix type.
         */
        typedef itk::VariableSizeMatrix<Real> Matrix;


        /**
         * Size of a DWI sequence.
         */
        const unsigned int SequenceDimension = 4;

        /**
         * DWI sequence.
         */
        typedef itk::Image<Float, SequenceDimension> Sequence;

        /**
         * DWI sequence's region.
         */
        typedef itk::ImageRegion<SequenceDimension> SequenceRegion;

        /**
         * DWI sequence's iterator.
         */
        typedef itk::ImageRegionIterator<Sequence> SequenceIterator;

        /**
         * DWI sequence's reader.
         */
        typedef itk::ImageFileReader<Sequence> SequenceReader;

        /**
         * DWI sequence's writer.
         */
        typedef itk::ImageFileWriter<Sequence> SequenceWriter;


        /**
         * Size of a DWI image (reference or gradient).
         */
        const unsigned int ImageDimension = 3;

        /**
         * DWI image (reference or gradient).
         */
        typedef itk::Image<Float, ImageDimension> Image;

        /**
         * DWI image's (reference or gradient) region.
         */
        typedef itk::ImageRegion<ImageDimension> ImageRegion;

        /**
         * DWI image's (reference or gradient) iterator.
         */
        typedef itk::ImageRegionIterator<Image> ImageIterator;

        /**
         * DWI image's (reference or gradient) reader.
         */
        typedef itk::ImageFileReader<Image> ImageReader;

        /**
         * DWI image's (reference or gradient) writer.
         */
        typedef itk::ImageFileWriter<Image> ImageWriter;

        /**
         * DWI image's interpolator.
         */
        typedef itk::BSplineInterpolateImageFunction<Image, Float, Float> ImageInterpolator;

        /**
         * Dwi image's continuous index.
         */
         typedef itk::ContinuousIndex<Real,ImageDimension> ImageContinuousIndex;


        /**
         * Dimension of a Mask.
         */
        const unsigned int MaskDimension = 3;

        /**
         * Image mask.
         */
        typedef itk::Image<char, MaskDimension> Mask;

        /**
         * Image mask's region.
         */
        typedef itk::ImageRegion<MaskDimension> MaskRegion;

        /**
         * Image mask's iterator.
         */
        typedef itk::ImageRegionIterator<Mask> MaskIterator;

        /**
         * Image mask's reader.
         */
        typedef itk::ImageFileReader<Mask> MaskReader;

        /**
         * Image mask's writer.
         */
        typedef itk::ImageFileWriter<Mask> MaskWriter;

        /**
         * Image mask's resampler
         */
        typedef itk::ResampleImageFilter<Mask,Mask> MaskResampler;

        /**
         * Image mask's interpolator
         */
        typedef itk::NearestNeighborInterpolateImageFunction<Mask,double> MaskInterpolator;


        /**
         * Dimension of a label map
         */
        const unsigned int LabelDimension = 3;

        /**
         * Label map
         */
        typedef itk::Image<short, LabelDimension> LabelMap;

        /**
         * Label map's reader
         */
        typedef itk::ImageFileReader<LabelMap> LabelMapReader;

        /**
         * Label map's iterator
         */
        typedef itk::ImageRegionIteratorWithIndex<LabelMap> LabelMapIterator;

        /**
         * Label map's resampler
         */
        typedef itk::ResampleImageFilter<LabelMap,LabelMap> LabelResampler;

        /**
         * Label map's interpolator
         */
        typedef itk::NearestNeighborInterpolateImageFunction<LabelMap,double> LabelInterpolator;

    } // namespace btk

#endif // BTK_TYPES_H
