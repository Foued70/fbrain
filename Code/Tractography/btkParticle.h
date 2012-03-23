/*
Copyright or © or Copr. Université de Strasbourg - Centre National de la Recherche Scientifique

31 march 2010
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

#ifndef BTK_PARTICLE_H
#define BTK_PARTICLE_H

    // STL includes
    #include "vector"
    #include "ostream"

    // Local includes
    #include "btkTypes.h"
    #include "btkPoint.h"
    #include "btkVector.h"


    namespace btk
    {

        /**
         * @class Particle
         * @brief Particle of particle filter
         * @author Julien Pontabry
         * @ingroup Tractography
         */
        class Particle
        {
            public:
                /**
                 * @brief Constructor
                 * @param p Point where the particle starts
                 */
                Particle(Point p);

                /**
                 * @brief Set the weight of the particle
                 * @param w Weight
                 */
                void setWeight(Real w);

                /**
                 * @brief Add vector to particle's vector path and move it
                 * @param v Vector
                 * @param mask Image mask
                 * @return 0 if p is out of the mask, 1 if p is in the mask and 2 if p is in the exclusion mask
                 */
                char addToPath(Vector v, Mask::Pointer mask);

                /**
                 * @brief Get particle's weight
                 * @return Weight of the particle
                 */
                Real   weight() const;

                /**
                 * @brief Set particle's last weight
                 * @param w Weight to set
                 */
                 void SetLastWeight(Real w);

                /**
                 * @brief Get last point of the particle
                 * @return Last particle's point
                 */
                Point  lastPoint() const;

                /**
                 * @brief Set last position of the particle
                 * @param p Point to set
                 */
                 void SetLastPoint(Point p);

                /**
                 * @brief Get last vector of the particle
                 * @return Last particle's vector
                 */
                Vector lastVector() const;

                /**
                 * @brief Set last displacement vector of the particle
                 * @param v Vector to set
                 */
                 void SetLastVector(Vector v);

                /**
                 * @brief Get a point on path
                 * @param i Number of the point on the path
                 * @return Point on particle's path
                 */
                Point  getPoint(unsigned int i) const;

                /**
                 * @brief Get a vector on path
                 * @param i Number of the vector on the path
                 * @return Vector on particle's path
                 */
                Vector getVector(unsigned int i) const;

                /**
                 * @brief Get the weight on path
                 * @param i Number of the weight on the path
                 * @return Weight on particle's path
                 */
                Real getWeight(unsigned int i) const;

                /**
                 * @brief Add log likelihood to path
                 * @param likelihood Log likelihood value
                 */
                 void addLikelihood(Real likelihood);

                /**
                 * @brief Get the computed log likelihood on path
                 * @param k Step number on the path
                 * @return Log likelihood of path at step k
                 */
                 Real getLikelihood(unsigned int k) const;

                /**
                 * @brief Get path's length
                 * @return Particle's path length
                 */
                unsigned int length() const;

                /**
                 * @brief Get if this particle is outside the mask
                 * @return True if the particle is outside, false otherwise
                 */
                 bool isOutside();

                /**
                 * @brief Get if this particle is active
                 * @return True if the particle is active, false otherwise
                 */
                 bool isActive();

                /**
                 * @brief Set if this particle is active
                 * @param outside False if the particle should be inactive
                 */
                void setActive(bool active);


                /**
                 * @brief Put particle on stream
                 * @param os Stream to put particle on
                 * @param p Particle to put
                 * @return Stream with particle put on
                 */
                friend std::ostream &operator<<(std::ostream &os, Particle p);

            private:
                /**
                 * @brief Test if a point is in a mask
                 * @param p Point to test
                 * @param mask Image mask
                 * @return 0 if p is out of the mask, 1 if p is in the mask and 2 if p is in the exclusion mask
                 */
                char IsInside(Point p, Mask::Pointer mask);

            private:
                bool m_active; /**< Particle's active state */
                bool m_outside; /**< Particle's outside mask state */

                std::vector<Real>   m_weight;   /**< Particule's weights */
                std::vector<Point>  m_points;   /**< Set of particle's points on path */
                std::vector<Vector> m_vectors;  /**< Set of particle's vectors on path */
                std::vector<Real>   m_likelihood;   /**< Log likelihood on path */
        };

    } // namespace btk

#endif // BTK_PARTICLE_H

