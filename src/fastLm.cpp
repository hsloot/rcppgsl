// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 4 -*-
//
// fastLm.cpp: Rcpp and GSL based implementation of lm
//
// Copyright (C)  2010 Dirk Eddelbuettel, Romain Francois
//
// This file is part of RcppGSL.
//
// RcppGSL is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 2 of the License, or
// (at your option) any later version.
//
// RcppGSL is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with RcppGSL.  If not, see <http://www.gnu.org/licenses/>.

#include <RcppGSL.h>

#include <gsl/gsl_multifit.h>
#include <cmath>

RCPP_FUNCTION_2( Rcpp::List, fastLm, SEXP ys, SEXP Xs ){
	
	RcppGSL::vector<double> y = ys ;
	RcppGSL::matrix<double> X = Xs ; 

	using Rcpp::_ ;

    int n=X.nrow(),k=X.ncol() ;
    double chisq;

    RcppGSL::vector<double> c(k) ;
    RcppGSL::matrix<double> cov(k,k) ;
    
    gsl_multifit_linear_workspace *work = gsl_multifit_linear_alloc (n, k);
    gsl_multifit_linear (X, y, c, cov, &chisq, work);
    gsl_multifit_linear_free (work);
         
    gsl_vector_view diag = gsl_matrix_diagonal(cov) ;
          
    Rcpp::NumericVector stderr ; stderr = diag ;
    std::transform( stderr.begin(), stderr.end(), stderr.begin(), sqrt ) ;
    
    Rcpp::List res = Rcpp::List::create( 
    	_["coef"] = c, 
    	_["stderr"] = stderr
    	) ;
    
    c.free() ;
    cov.free();
    y.free();
    X.free();
	
    return res ;
}

