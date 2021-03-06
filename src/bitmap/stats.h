/* 
 * Copyright 2009, 2010, Jake Askeland, jake(dot)askeland(at)gmail(dot)com
 * 
 *  * This program is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 *  * This file is part of Topological all shortest paths automatique' (TASPA).
 * 
 *     TASPA is free software: you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation, either version 3 of the License, or
 *     (at your option) any later version.
 * 
 *     TASPA is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 * 
 *     You should have received a copy of the GNU General Public License
 *     along with TASPA.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COLOR_STATS_H
#define COLOR_STATS_H

#include <math.h>
#include <stdlib.h>

class stats {
	private:
		static const int BUFFER_SIZE = 14;
		char buffer[BUFFER_SIZE];

	protected:
	
		double sum;
		double sum_of_squares;
		double variance_value;
		long n;
		
		bool initialized;
		
		double CalculateVariance() {
			// sum (x[i] - mean)^2 / n =
			return (sum_of_squares/n) - pow(sum/n,2) + pow(Mean(),2);
		}
		
	public:
	
		stats() { initialized = false; n=0; sum=0; sum_of_squares=0; }
	
		void add(double val) { 
			sum += val; 
			sum_of_squares += val*val; 
			n++; 
			initialized = false;
		}
		
		double Sum() { return sum; }
				
		double Mean() { return sum/n; }
		
		double Variance() { 
			if (!initialized) variance_value = CalculateVariance();
			return variance_value;
		}
		
		double StdDev() { return sqrt(Variance()); }
		
		double Z(double x) { return (x-Mean())/StdDev(); }

		double Phi(double x) { 
			double z = Z(x);
			if (z >  6.0) return 1.0;
			if (z < -6.0) return 0.0;
			
			static const double b1 =  0.319381530;
			static const double b2 = -0.356563782;
			static const double b3 =  1.781477937;
			static const double b4 = -1.821255978;
			static const double b5 =  1.330274429;
			static const double p0 =  0.231641900;
			static const double c2 =  0.398942300;
			
			double a=fabs(z);
			double t=1.0/(1.0+a*p0);
			double b=c2*exp((-z)*(z/2.0));
			double r=((((b5*t+b4)*t+b3)*t+b2)*t+b1)*t;
			r=1.0-b*r;
			if (z < 0.0) r = 1.0 - r;
			return r;
		}
		
		double Normal(double x) {
			return (x > Mean()) ? 1-Phi(x) : Phi(x);
		}
};

#endif
