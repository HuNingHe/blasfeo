/**************************************************************************************************
*                                                                                                 *
* This file is part of BLASFEO.                                                                   *
*                                                                                                 *
* BLASFEO -- BLAS for embedded optimization.                                                      *
* Copyright (C) 2019 by Gianluca Frison.                                                          *
* Developed at IMTEK (University of Freiburg) under the supervision of Moritz Diehl.              *
* All rights reserved.                                                                            *
*                                                                                                 *
* The 2-Clause BSD License                                                                        *
*                                                                                                 *
* Redistribution and use in source and binary forms, with or without                              *
* modification, are permitted provided that the following conditions are met:                     *
*                                                                                                 *
* 1. Redistributions of source code must retain the above copyright notice, this                  *
*    list of conditions and the following disclaimer.                                             *
* 2. Redistributions in binary form must reproduce the above copyright notice,                    *
*    this list of conditions and the following disclaimer in the documentation                    *
*    and/or other materials provided with the distribution.                                       *
*                                                                                                 *
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND                 *
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED                   *
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE                          *
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR                 *
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES                  *
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;                    *
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND                     *
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT                      *
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS                   *
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                    *
*                                                                                                 *
* Author: Gianluca Frison, gianluca.frison (at) imtek.uni-freiburg.de                             *
*                                                                                                 *
**************************************************************************************************/



#if defined(LA_REFERENCE) | defined(TESTING_MODE)


// dgemm nn
void GEMM_NN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL 
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+(ii+1), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+1));
				c_11 += XMATEL_A(aai+(ii+1), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+1));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+(ii+1), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+kk, bbj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			}
		}
	return;
	}



// dgemm nt
void GEMM_NT(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL 
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_10 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				c_11 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+1));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_01 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+(jj+1), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_10 += XMATEL_A(aai+(ii+1), aaj+kk) * XMATEL_B(bbi+(jj+0), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+(ii+0), aaj+kk) * XMATEL_B(bbi+(jj+0), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			}
		}
	return;
	}



// dgemm tn
void GEMM_TN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL 
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				c_11 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+1));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			}
		}
	return;
	}



// dgemm tt
void GEMM_TT(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL 
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+1), bbj+kk);
				c_11 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+(jj+1), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+1));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+1), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+1));
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10 + beta * XMATEL_C(cci+(ii+1), ccj+(jj+0));
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+(jj+0), bbj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00 + beta * XMATEL_C(cci+(ii+0), ccj+(jj+0));
			}
		}
	return;
	}



// dtrsm_left_lower_nottransposed_notunit
void TRSM_LLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if(sA->use_dA<n)
			{
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			kk = 0;
			for(; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_00 *= dA[ii+0];
			d_01 *= dA[ii+0];
			d_10 -= XMATEL_A(aai+ii+1, aaj+ii) * d_00;
			d_11 -= XMATEL_A(aai+ii+1, aaj+ii) * d_01;
			d_10 *= dA[ii+1];
			d_11 *= dA[ii+1];
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii, bbj+(jj+1));
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_00 *= dA[ii+0];
			d_01 *= dA[ii+0];
			XMATEL_D(ddi+ii, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+jj);
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_00 *= dA[ii+0];
			d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * d_00;
			d_10 *= dA[ii+1];
			XMATEL_D(ddi+ii+0, ddj+jj) = d_00;
			XMATEL_D(ddi+ii+1, ddj+jj) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_00 *= dA[ii+0];
			XMATEL_D(ddi+ii, ddj+jj) = d_00;
			}
		}
	return;
	}



// dtrsm_left_lower_nottransposed_unit
void TRSM_LLNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			kk = 0;
			for(; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * d_00;
			d_11 -= XMATEL_A(aai+ii+1, aaj+kk) * d_01;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii, bbj+(jj+1));
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			XMATEL_D(ddi+ii, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+jj);
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_10 -= XMATEL_A(aai+ii+1, aaj+kk) * d_00;
			XMATEL_D(ddi+ii+0, ddj+jj) = d_00;
			XMATEL_D(ddi+ii+1, ddj+jj) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+ii, aaj+kk) * XMATEL_D(ddi+kk, ddj+jj);
				}
			XMATEL_D(ddi+ii, ddj+jj) = d_00;
			}
		}
	return;
	}



// dtrsm_lltn
void TRSM_LLTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if (sA->use_dA<m)
			{
			// invert diagonal of pA
			for(ii=0; ii<m; ii++)
				dA[ii] = 1.0/XMATEL_A(aai+ii, aaj+ii);
			// use only now
			sA->use_dA = m;
			}
		}
	else
		{
		for(ii=0; ii<m; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+1));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_10 *= dA[id+1];
			d_11 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_10;
			d_01 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_11;
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+id+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_10 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_10;
			d_00 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_00 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			}
		}
	return;
	}



// dtrsm_lltu
void TRSM_LLTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+1));
			kk = id+2;

			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_10;
			d_01 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_11;
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+id+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			}
		}
	return;
	}



// dtrsm_left_upper_nottransposed_notunit
void TRSM_LUNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if (sA->use_dA<m)
			{
			// invert diagonal of pA
			for(ii=0; ii<m; ii++)
				dA[ii] = 1.0/XMATEL_A(aai+ii, aaj+ii);
			// use only now
			sA->use_dA = m;
			}
		}
	else
		{
		for(ii=0; ii<m; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+1));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_10 *= dA[id+1];
			d_11 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_10;
			d_01 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_11;
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+id+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_00 *= dA[id+0];
			d_01 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_10 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_10;
			d_00 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_00 *= dA[id+0];
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			}
		}
	return;
	}



// dtrsm_lunu
void TRSM_LUNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+1));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_10;
			d_01 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_11;
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+id+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+1));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+1));
				}
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+0, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			id = m-ii-2;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+id+1, bbj+(jj+0));
			kk = id+2;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_10;
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+id+1, ddj+(jj+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			id = m-ii-1;
			d_00 = alpha * XMATEL_B(bbi+id+0, bbj+(jj+0));
			kk = id+1;
			for(; kk<m; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+kk+0, ddj+(jj+0));
				}
			XMATEL_D(ddi+id+0, ddj+(jj+0)) = d_00;
			}
		}
	return;
	}



// dtrsm_lutn
void TRSM_LUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if(sA->use_dA<n)
			{
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			kk = 0;
			for(; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_00 *= dA[ii+0];
			d_01 *= dA[ii+0];
			d_10 -= XMATEL_A(aai+ii, aaj+(ii+1)) * d_00;
			d_11 -= XMATEL_A(aai+ii, aaj+(ii+1)) * d_01;
			d_10 *= dA[ii+1];
			d_11 *= dA[ii+1];
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii, bbj+(jj+1));
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_00 *= dA[ii+0];
			d_01 *= dA[ii+0];
			XMATEL_D(ddi+ii, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+jj);
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+jj);
				d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_00 *= dA[ii+0];
			d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * d_00;
			d_10 *= dA[ii+1];
			XMATEL_D(ddi+ii+0, ddj+jj) = d_00;
			XMATEL_D(ddi+ii+1, ddj+jj) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_00 *= dA[ii+0];
			XMATEL_D(ddi+ii, ddj+jj) = d_00;
			}
		}
	return;
	}



// dtrsm_lutu
void TRSM_LUTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			kk = 0;
			for(; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+(jj+1));
				d_11 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			d_10 -= XMATEL_A(aai+ii, aaj+(ii+1)) * d_00;
			d_11 -= XMATEL_A(aai+ii, aaj+(ii+1)) * d_01;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(jj+0));
			d_01 = alpha * XMATEL_B(bbi+ii, bbj+(jj+1));
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+(jj+0));
				d_01 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+(jj+1));
				}
			XMATEL_D(ddi+ii, ddj+(jj+0)) = d_00;
			XMATEL_D(ddi+ii, ddj+(jj+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+jj);
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_D(ddi+kk, ddj+jj);
				d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_D(ddi+kk, ddj+jj);
				}
			d_10 -= XMATEL_A(aai+kk, aaj+(ii+1)) * d_00;
			XMATEL_D(ddi+ii+0, ddj+jj) = d_00;
			XMATEL_D(ddi+ii+1, ddj+jj) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<ii; kk++)
				{
				d_00 -= XMATEL_A(aai+kk, aaj+ii) * XMATEL_D(ddi+kk, ddj+jj);
				}
			XMATEL_D(ddi+ii, ddj+jj) = d_00;
			}
		}
	return;
	}



// dtrsm_rlnn
void TRSM_RLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if (sA->use_dA<n)
			{
			// invert diagonal of pA
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0/XMATEL_A(aai+ii, aaj+ii);
			// use only now
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		id = n-jj-2;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_11 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_01 *= dA[id+1];
			d_11 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_01;
			d_10 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_11;
			d_00 *= dA[id+0];
			d_10 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(id+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				}
			d_01 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_01;
			d_00 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		id = n-jj-1;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			kk = id+1;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_00 *= dA[id+0];
			d_10 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(id));
			kk = id+1;
			for(; kk<n; kk++)
				d_00 -= XMATEL_A(aai+kk, aaj+(id)) * XMATEL_D(ddi+ii, ddj+(kk));
			XMATEL_D(ddi+ii, ddj+(id)) = d_00 * dA[id];
			}
		}
	return;
	}



// dtrsm_rlnu
void TRSM_RLNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		id = n-jj-2;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_11 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_01;
			d_10 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_11;
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(id+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+kk+0, aaj+(id+1)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				}
			d_00 -= XMATEL_A(aai+id+1, aaj+(id+0)) * d_01;
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		id = n-jj-1;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			kk = id+1;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+kk+0, aaj+(id+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(id));
			kk = id+1;
			for(; kk<n; kk++)
				d_00 -= XMATEL_A(aai+kk, aaj+(id)) * XMATEL_D(ddi+ii, ddj+(kk));
			}
		}
	return;
	}



// dtrsm_right_lower_transposed_not-unit
void TRSM_RLTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if(sA->use_dA<n)
			{
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	REAL
		f_00_inv,
		f_10, f_11_inv,
		c_00, c_01,
		c_10, c_11;
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_00_inv = dA[jj+0];
		f_10 = XMATEL_A(aai+jj+1, aaj+(jj+0));
		f_11_inv = dA[jj+1];
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			c_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_10 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				c_11 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				}
			c_00 *= f_00_inv;
			c_10 *= f_00_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			c_01 *= f_11_inv;
			c_11 *= f_11_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				}
			c_00 *= f_00_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			c_01 -= c_00 * f_10;
			c_01 *= f_11_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// factorize diagonal
		f_00_inv = dA[jj];
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii, ddj+kk) * XMATEL_A(aai+jj, aaj+kk);
				}
			c_00 *= f_00_inv;
			XMATEL_D(ddi+ii, ddj+jj) = c_00;
			}
		}
	return;
	}



// dtrsm_right_lower_transposed_unit
void TRSM_RLTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		f_10,
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_10 = XMATEL_A(aai+jj+1, aaj+(jj+0));
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			c_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_10 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				c_11 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+0, aaj+kk);
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+jj+1, aaj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			c_01 -= c_00 * f_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii, ddj+kk) * XMATEL_A(aai+jj, aaj+kk);
				}
			XMATEL_D(ddi+ii, ddj+jj) = c_00;
			}
		}
	return;
	}



// dtrsm_runn
void TRSM_RUNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if(sA->use_dA<n)
			{
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	REAL
		f_00_inv,
		f_10, f_11_inv,
		c_00, c_01,
		c_10, c_11;
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_00_inv = dA[jj+0];
		f_10 = XMATEL_A(aai+jj+0, aaj+(jj+1));
		f_11_inv = dA[jj+1];
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			c_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_10 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				c_11 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			c_00 *= f_00_inv;
			c_10 *= f_00_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			c_01 *= f_11_inv;
			c_11 *= f_11_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			c_00 *= f_00_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			c_01 -= c_00 * f_10;
			c_01 *= f_11_inv;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// factorize diagonal
		f_00_inv = dA[jj];
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii, ddj+kk) * XMATEL_A(aai+kk, aaj+jj);
				}
			c_00 *= f_00_inv;
			XMATEL_D(ddi+ii, ddj+jj) = c_00;
			}
		}
	return;
	}



// dtrsm_runu
void TRSM_RUNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL
		f_10,
		c_00, c_01,
		c_10, c_11;
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		f_10 = XMATEL_A(aai+jj+0, aaj+(jj+1));
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			c_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_10 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				c_11 -= XMATEL_D(ddi+ii+1, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = c_10;
			c_01 -= c_00 * f_10;
			c_11 -= c_10 * f_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+0));
			c_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(jj+1));
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 -= XMATEL_D(ddi+ii+0, ddj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = c_00;
			c_01 -= c_00 * f_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// factorize diagonal
		for(ii=0; ii<m; ii++)
			{
			c_00 = alpha * XMATEL_B(bbi+ii, bbj+jj);
			for(kk=0; kk<jj; kk++)
				{
				c_00 -= XMATEL_D(ddi+ii, ddj+kk) * XMATEL_A(aai+kk, aaj+jj);
				}
			XMATEL_D(ddi+ii, ddj+jj) = c_00;
			}
		}
	return;
	}



// dtrsm_right_upper_transposed_notunit
void TRSM_RUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	REAL *dA = sA->dA;
	if(ai==0 & aj==0)
		{
		if (sA->use_dA<n)
			{
			// invert diagonal of pA
			for(ii=0; ii<n; ii++)
				dA[ii] = 1.0/XMATEL_A(aai+ii, aaj+ii);
			// use only now
			sA->use_dA = n;
			}
		}
	else
		{
		for(ii=0; ii<n; ii++)
			dA[ii] = 1.0 / XMATEL_A(aai+ii, aaj+ii);
		sA->use_dA = 0; // nonzero offset makes diagonal dirty
		}
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		id = n-jj-2;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_11 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_01 *= dA[id+1];
			d_11 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_01;
			d_10 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_11;
			d_00 *= dA[id+0];
			d_10 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(id+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				}
			d_01 *= dA[id+1];
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_01;
			d_00 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		id = n-jj-1;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			kk = id+1;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_00 *= dA[id+0];
			d_10 *= dA[id+0];
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(id));
			kk = id+1;
			for(; kk<n; kk++)
				d_00 -= XMATEL_A(aai+id, aaj+(kk)) * XMATEL_D(ddi+ii, ddj+(kk));
			XMATEL_D(ddi+ii, ddj+(id)) = d_00 * dA[id];
			}
		}
	return;
	}



// dtrsm_rutu
void TRSM_RUTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	int i1 = 1;
	int ii, jj, kk, id;
	REAL
		d_00, d_01,
		d_10, d_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	// solve
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		id = n-jj-2;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			d_11 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_11 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_01;
			d_10 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_11;
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			XMATEL_D(ddi+ii+1, ddj+(id+1)) = d_11;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_01 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+1));
			kk = id+2;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_01 -= XMATEL_A(aai+id+1, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				}
			d_00 -= XMATEL_A(aai+id+0, aaj+(id+1)) * d_01;
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+0, ddj+(id+1)) = d_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		id = n-jj-1;
		for(; ii<m-1; ii+=2)
			{
			d_00 = alpha * XMATEL_B(bbi+ii+0, bbj+(id+0));
			d_10 = alpha * XMATEL_B(bbi+ii+1, bbj+(id+0));
			kk = id+1;
			for(; kk<n; kk++)
				{
				d_00 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+0, ddj+(kk+0));
				d_10 -= XMATEL_A(aai+id+0, aaj+(kk+0)) * XMATEL_D(ddi+ii+1, ddj+(kk+0));
				}
			XMATEL_D(ddi+ii+0, ddj+(id+0)) = d_00;
			XMATEL_D(ddi+ii+1, ddj+(id+0)) = d_10;
			}
		for(; ii<m; ii++)
			{
			d_00 = alpha * XMATEL_B(bbi+ii, bbj+(id));
			kk = id+1;
			for(; kk<n; kk++)
				d_00 -= XMATEL_A(aai+id, aaj+(kk)) * XMATEL_D(ddi+ii, ddj+(kk));
			}
		}
	return;
	}



// dtrmm_right_upper_transposed_notunit (A triangular !!!)
void TRMM_RUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			kk = jj;
			c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
			c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				c_01 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+1), aaj+kk);
				c_11 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+(jj+1), aaj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10;
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01;
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			kk = jj;
			c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				c_01 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+1), aaj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+(jj+0), aaj+kk);
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			}
		}	
	return;
	}



// dtrmm_right_lower_nottransposed_notunit (A triangular !!!)
void TRMM_RLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL 
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			c_01 = 0.0; ;
			c_11 = 0.0; ;
			kk = jj;
			c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
			c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				c_11 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10;
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01;
			XMATEL_D(ddi+(ii+1), ddj+(jj+1)) = alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			c_01 = 0.0; ;
			kk = jj;
			c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
			kk++;
			for(; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_01 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+1));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+0), ddj+(jj+1)) = alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		ii = 0;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0; ;
			c_10 = 0.0; ;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				c_10 += XMATEL_B(bbi+(ii+1), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			XMATEL_D(ddi+(ii+1), ddj+(jj+0)) = alpha * c_10;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0; ;
			for(kk=jj; kk<n; kk++)
				{
				c_00 += XMATEL_B(bbi+(ii+0), bbj+kk) * XMATEL_A(aai+kk, aaj+(jj+0));
				}
			XMATEL_D(ddi+(ii+0), ddj+(jj+0)) = alpha * c_00;
			}
		}
	return;
	}



// dsyrk_lower not-transposed
void SYRK_LN(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<m-1; jj+=2)
		{
		// diagonal
		c_00 = 0.0;
		c_10 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			c_10 += XMATEL_A(aai+jj+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			c_11 += XMATEL_A(aai+jj+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		XMATEL_D(ddi+jj+1, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+0)) + alpha * c_10;
		XMATEL_D(ddi+jj+1, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+1)) + alpha * c_11;
		// lower
		ii = jj+2;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_10 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				c_11 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+1)) + alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			}
		}
	if(jj<m)
		{
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj, aaj+kk) * XMATEL_B(bbi+jj, bbj+kk);
			}
		XMATEL_D(ddi+jj, ddj+jj) = beta * XMATEL_C(cci+jj, ccj+jj) + alpha * c_00;
		}
	return;
	}



// dsyrk_lower not-transposed
void SYRK_LN_MN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0 | n<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<n-1; jj+=2)
		{
		// diagonal
		c_00 = 0.0;
		c_10 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			c_10 += XMATEL_A(aai+jj+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			c_11 += XMATEL_A(aai+jj+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		XMATEL_D(ddi+jj+1, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+0)) + alpha * c_10;
		XMATEL_D(ddi+jj+1, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+1)) + alpha * c_11;
		// lower
		ii = jj+2;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_10 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				c_11 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+1)) + alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			}
		}
	for(; jj<n; jj++)
		{
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj, aaj+kk) * XMATEL_B(bbi+jj, bbj+kk);
			}
		XMATEL_D(ddi+jj, ddj+jj) = beta * XMATEL_C(cci+jj, ccj+jj) + alpha * c_00;
		// lower
		for(ii=jj+1; ii<m; ii++)
			{
			c_00 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii, aaj+kk) * XMATEL_B(bbi+jj, bbj+kk);
				}
			XMATEL_D(ddi+ii, ddj+jj) = beta * XMATEL_C(cci+ii, ccj+jj) + alpha * c_00;
			}
		}
	return;
	}



// dsyrk_lower transposed
void SYRK_LT(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<m-1; jj+=2)
		{
		// diagonal
		c_00 = 0.0;
		c_10 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+kk, aaj+(jj+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
			c_10 += XMATEL_A(aai+kk, aaj+(jj+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
			c_11 += XMATEL_A(aai+kk, aaj+(jj+1)) * XMATEL_B(bbi+kk, bbj+(jj+1));
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		XMATEL_D(ddi+jj+1, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+0)) + alpha * c_10;
		XMATEL_D(ddi+jj+1, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+1)) + alpha * c_11;
		// lower
		ii = jj+2;
		for(; ii<m-1; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				c_11 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+1)) + alpha * c_11;
			}
		for(; ii<m; ii++)
			{
			c_00 = 0.0;
			c_01 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			}
		}
	if(jj<m)
		{
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+kk, aaj+jj) * XMATEL_B(bbi+kk, bbj+jj);
			}
		XMATEL_D(ddi+jj, ddj+jj) = beta * XMATEL_C(cci+jj, ccj+jj) + alpha * c_00;
		}
	return;
	}



// dsyrk_upper not-transposed
void SYRK_UN(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<m-1; jj+=2)
		{
		// upper
		ii = 0;
		for(; ii<jj; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_10 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_01 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				c_11 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+1)) + alpha * c_11;
			}
		// diagonal
		c_00 = 0.0;
		c_01 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			c_01 += XMATEL_A(aai+jj+0, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
			c_11 += XMATEL_A(aai+jj+1, aaj+kk) * XMATEL_B(bbi+jj+1, bbj+kk);
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		XMATEL_D(ddi+jj+0, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+1)) + alpha * c_01;
		XMATEL_D(ddi+jj+1, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+1)) + alpha * c_11;
		}
	if(jj<m)
		{
		// upper
		ii = 0;
		for(; ii<jj; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+ii+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				c_10 += XMATEL_A(aai+ii+1, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			}
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+jj+0, aaj+kk) * XMATEL_B(bbi+jj+0, bbj+kk);
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		}
	return;
	}



// dsyrk_upper transposed
void SYRK_UT(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{
	if(m<=0)
		return;

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int ii, jj, kk;
	REAL
		c_00, c_01,
		c_10, c_11;
#if defined(LA_REFERENCE)
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	REAL *pA = sA->pA + ai + aj*lda;
	REAL *pB = sB->pA + bi + bj*ldb;
	REAL *pC = sC->pA + ci + cj*ldc;
	REAL *pD = sD->pA + di + dj*ldd;
	const int aai=0; const int aaj=0;
	const int bbi=0; const int bbj=0;
	const int cci=0; const int ccj=0;
	const int ddi=0; const int ddj=0;
#else
	int aai=ai; int aaj=aj;
	int bbi=bi; int bbj=bj;
	int cci=ci; int ccj=cj;
	int ddi=di; int ddj=dj;
#endif
	jj = 0;
	for(; jj<m-1; jj+=2)
		{
		// upper
		ii = 0;
		for(; ii<jj; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			c_01 = 0.0;
			c_11 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_01 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				c_11 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+1));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			XMATEL_D(ddi+ii+0, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+1)) + alpha * c_01;
			XMATEL_D(ddi+ii+1, ddj+(jj+1)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+1)) + alpha * c_11;
			}
		// diagonal
		c_00 = 0.0;
		c_01 = 0.0;
		c_11 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+kk, aaj+(jj+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
			c_01 += XMATEL_A(aai+kk, aaj+(jj+0)) * XMATEL_B(bbi+kk, bbj+(jj+1));
			c_11 += XMATEL_A(aai+kk, aaj+(jj+1)) * XMATEL_B(bbi+kk, bbj+(jj+1));
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		XMATEL_D(ddi+jj+0, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+1)) + alpha * c_01;
		XMATEL_D(ddi+jj+1, ddj+(jj+1)) = beta * XMATEL_C(cci+jj+1, ccj+(jj+1)) + alpha * c_11;
		}
	if(jj<m)
		{
		// upper
		ii = 0;
		for(; ii<jj; ii+=2)
			{
			c_00 = 0.0;
			c_10 = 0.0;
			for(kk=0; kk<k; kk++)
				{
				c_00 += XMATEL_A(aai+kk, aaj+(ii+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				c_10 += XMATEL_A(aai+kk, aaj+(ii+1)) * XMATEL_B(bbi+kk, bbj+(jj+0));
				}
			XMATEL_D(ddi+ii+0, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+0, ccj+(jj+0)) + alpha * c_00;
			XMATEL_D(ddi+ii+1, ddj+(jj+0)) = beta * XMATEL_C(cci+ii+1, ccj+(jj+0)) + alpha * c_10;
			}
		// diagonal
		c_00 = 0.0;
		for(kk=0; kk<k; kk++)
			{
			c_00 += XMATEL_A(aai+kk, aaj+(jj+0)) * XMATEL_B(bbi+kk, bbj+(jj+0));
			}
		XMATEL_D(ddi+jj+0, ddj+(jj+0)) = beta * XMATEL_C(cci+jj+0, ccj+(jj+0)) + alpha * c_00;
		}
	return;
	}



#elif defined(LA_EXTERNAL_BLAS_WRAPPER)



// dgemm nn
void GEMM_NN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cn = 'n';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pC = sC->pA+ci+cj*sC->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pC+jj*ldc, &i1, pD+jj*ldd, &i1);
		}
	GEMM(&cn, &cn, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
	return;
	}



// dgemm nt
void GEMM_NT(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cn = 'n';
	char ct = 't';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pC = sC->pA+ci+cj*sC->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pC+jj*ldc, &i1, pD+jj*ldd, &i1);
		}
	GEMM(&cn, &ct, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
	return;
	}



// dgemm tn
void GEMM_TN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cn = 'n';
	char ct = 't';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pC = sC->pA+ci+cj*sC->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pC+jj*ldc, &i1, pD+jj*ldd, &i1);
		}
	GEMM(&ct, &cn, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
	return;
	}



// dgemm tt
void GEMM_TT(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cn = 'n';
	char ct = 't';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pC = sC->pA+ci+cj*sC->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pC+jj*ldc, &i1, pD+jj*ldd, &i1);
		}
	GEMM(&ct, &ct, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
	return;
	}



// dtrsm_left_lower_nottransposed_notunit
void TRSM_LLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*sD->m, &i1);
		}
	TRSM(&cl, &cl, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_lower_nottransposed_unit
void TRSM_LLNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*sD->m, &i1);
		}
	TRSM(&cl, &cl, &cn, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_lower_transposed_notunit
void TRSM_LLTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char ct = 't';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*sD->m, &i1);
		}
	TRSM(&cl, &cl, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_lower_transposed_unit
void TRSM_LLTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*sD->m, &i1);
		}
	TRSM(&cl, &cl, &ct, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_upper_nottransposed_notunit
void TRSM_LUNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cl, &cu, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_upper_nottransposed_unit
void TRSM_LUNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cl, &cu, &cn, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_upper_transposed_notunit
void TRSM_LUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cl, &cu, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_left_upper_transposed_unit
void TRSM_LUTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cl, &cu, &ct, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_lower_nottransposed_notunit
void TRSM_RLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cl, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_lower_nottransposed_unit
void TRSM_RLNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cl, &cn, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_lower_transposed_notunit
void TRSM_RLTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cl, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_lower_transposed_unit
void TRSM_RLTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cl, &ct, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_upper_nottransposed_notunit
void TRSM_RUNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cu, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_upper_nottransposed_unit
void TRSM_RUNU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cu, &cn, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_upper_transposed_notunit
void TRSM_RUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrsm_right_upper_transposed_unit
void TRSM_RUTU(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRSM(&cr, &cu, &ct, &cu, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrmm_right_upper_transposed_notunit (A triangular !!!)
void TRMM_RUTN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRMM(&cr, &cu, &ct, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dtrmm_right_lower_nottransposed_notunit (A triangular !!!)
void TRMM_RLNN(int m, int n, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA+ai+aj*sA->m;
	REAL *pB = sB->pA+bi+bj*sB->m;
	REAL *pD = sD->pA+di+dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldd = sD->m;
	if(!(pB==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pB+jj*ldb, &i1, pD+jj*ldd, &i1);
		}
	TRMM(&cr, &cl, &cn, &cn, &m, &n, &alpha, pA, &lda, pD, &ldd);
	return;
	}



// dsyrk lower not-transposed (allowing for different factors => use dgemm !!!)
void SYRK_LN(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA + ai + aj*sA->m;
	REAL *pB = sB->pA + bi + bj*sB->m;
	REAL *pC = sC->pA + ci + cj*sC->m;
	REAL *pD = sD->pA + di + dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<m; jj++)
			COPY(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
		}
	if(pA==pB)
		{
		SYRK(&cl, &cn, &m, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		}
	else
		{
		GEMM(&cn, &ct, &m, &m, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
		}
	return;
	}



// dsyrk lower not-transposed (allowing for different factors => use dgemm !!!)
void SYRK_LN_MN(int m, int n, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA + ai + aj*sA->m;
	REAL *pB = sB->pA + bi + bj*sB->m;
	REAL *pC = sC->pA + ci + cj*sC->m;
	REAL *pD = sD->pA + di + dj*sD->m;
	int i1 = 1;
	int mmn = m-n;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<n; jj++)
			COPY(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
		}
	if(pA==pB)
		{
		SYRK(&cl, &cn, &n, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		GEMM(&cn, &ct, &mmn, &n, &k, &alpha, pA+n, &lda, pB, &ldb, &beta, pD+n, &ldd);
		}
	else
		{
		GEMM(&cn, &ct, &m, &n, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
		}
	return;
	}



// dsyrk lower transposed (allowing for different factors => use dgemm !!!)
void SYRK_LT(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA + ai + aj*sA->m;
	REAL *pB = sB->pA + bi + bj*sB->m;
	REAL *pC = sC->pA + ci + cj*sC->m;
	REAL *pD = sD->pA + di + dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<m; jj++)
			COPY(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
		}
	if(pA==pB)
		{
		SYRK(&cl, &ct, &m, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		}
	else
		{
		GEMM(&ct, &cn, &m, &m, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
		}
	return;
	}



// dsyrk upper not-transposed (allowing for different factors => use dgemm !!!)
void SYRK_UN(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA + ai + aj*sA->m;
	REAL *pB = sB->pA + bi + bj*sB->m;
	REAL *pC = sC->pA + ci + cj*sC->m;
	REAL *pD = sD->pA + di + dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<m; jj++)
			COPY(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
		}
	if(pA==pB)
		{
		SYRK(&cu, &cn, &m, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		}
	else
		{
		GEMM(&cn, &ct, &m, &m, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
		}
	return;
	}



// dsyrk upper transposed (allowing for different factors => use dgemm !!!)
void SYRK_UT(int m, int k, REAL alpha, struct XMAT *sA, int ai, int aj, struct XMAT *sB, int bi, int bj, REAL beta, struct XMAT *sC, int ci, int cj, struct XMAT *sD, int di, int dj)
	{

	// invalidate stored inverse diagonal of result matrix
	sD->use_dA = 0;

	int jj;
	char cl = 'l';
	char cn = 'n';
	char cr = 'r';
	char ct = 't';
	char cu = 'u';
	REAL *pA = sA->pA + ai + aj*sA->m;
	REAL *pB = sB->pA + bi + bj*sB->m;
	REAL *pC = sC->pA + ci + cj*sC->m;
	REAL *pD = sD->pA + di + dj*sD->m;
	int i1 = 1;
	int lda = sA->m;
	int ldb = sB->m;
	int ldc = sC->m;
	int ldd = sD->m;
	if(!(beta==0.0 || pC==pD))
		{
		for(jj=0; jj<m; jj++)
			COPY(&m, pC+jj*sC->m, &i1, pD+jj*sD->m, &i1);
		}
	if(pA==pB)
		{
		SYRK(&cu, &ct, &m, &k, &alpha, pA, &lda, &beta, pD, &ldd);
		}
	else
		{
		GEMM(&ct, &cn, &m, &m, &k, &alpha, pA, &lda, pB, &ldb, &beta, pD, &ldd);
		}
	return;
	}



#else

#error : wrong LA choice

#endif
