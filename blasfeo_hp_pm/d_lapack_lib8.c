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

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <blasfeo_common.h>
#include <blasfeo_d_aux.h>
#include <blasfeo_d_kernel.h>
#include <blasfeo_d_blasfeo_api.h>
#if defined(BLASFEO_REF_API)
#include <blasfeo_d_blasfeo_ref_api.h>
#endif



// dpotrf
void blasfeo_hp_dpotrf_l(int m, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{

	if(m<=0)
		return;

	if(ci!=0 | di!=0)
		{
#if defined(BLASFEO_REF_API)
		blasfeo_ref_dpotrf_l(m, sC, ci, cj, sD, di, dj);
		return;
#else
		printf("\nblasfeo_dpotrf_l: feature not implemented yet: ci=%d, di=%d\n", ci, di);
		exit(1);
#endif
		}

	const int ps = 8;

	double alpha = 1.0;

	int sdc = sC->cn;
	int sdd = sD->cn;
	double *pC = sC->pA + cj*ps;
	double *pD = sD->pA + dj*ps;
	double *dD = sD->dA;

	if(di==0 & dj==0) // XXX what to do if di and dj are not zero
		sD->use_dA = m;
	else
		sD->use_dA = 0;

	int i, j, l;

	i = 0;
#if 0
	for(; i<m-7; i+=8)
		{
		j = 0;
		for(; j<i; j+=4)
			{
			kernel_dtrsm_nt_rl_inv_8x4_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], sdc, &pD[j*ps+i*sdd], sdd, &pD[j*ps+j*sdd], &dD[j]);
			}
		kernel_dpotrf_nt_l_8x4_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &pC[j*ps+j*sdc], sdc, &pD[j*ps+j*sdd], sdd, &dD[j]);
		kernel_dpotrf_nt_l_4x4_lib4(j+4, &pD[(i+4)*sdd], &pD[(j+4)*sdd], &pC[(j+4)*ps+(i+4)*sdc], &pD[(j+4)*ps+(i+4)*sdd], &dD[j+4]);
		}
	if(m>i)
		{
		if(m-i<=4)
			{
			goto left_4;
			}
		else
			{
			goto left_8;
			}
		}
#else
	for(; i<m-7; i+=8)
		{
		j = 0;
		for(; j<i; j+=8)
			{
			kernel_dtrsm_nt_rl_inv_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j]);
			}
		kernel_dpotrf_nt_l_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j]);
		}
	if(m>i)
		{
		goto left_8;
		}
#endif

	// common return if i==m
	return;

	// clean up loops definitions

#if 0
	left_8:
	j = 0;
	for(; j<i; j+=4)
		{
		kernel_dtrsm_nt_rl_inv_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], sdc, &pD[j*ps+i*sdd], sdd, &pD[j*ps+j*sdd], &dD[j], m-i, m-j);
		}
	kernel_dpotrf_nt_l_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &pC[j*ps+j*sdc], sdc, &pD[j*ps+j*sdd], sdd, &dD[j], m-i, m-j);
	kernel_dpotrf_nt_l_4x4_vs_lib4(j+4, &pD[(i+4)*sdd], &pD[(j+4)*sdd], &pC[(j+4)*ps+(i+4)*sdc], &pD[(j+4)*ps+(i+4)*sdd], &dD[j+4], m-i-4, m-j-4);
	return;
#endif

#if 0
	left_4:
	j = 0;
	for(; j<i-8; j+=12)
		{
		kernel_dtrsm_nt_rl_inv_4x12_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], sdd, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], sdd, &dD[j], m-i, m-j);
		}
	if(j<i-4)
		{
		kernel_dtrsm_nt_rl_inv_4x8_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], sdd, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], sdd, &dD[j], m-i, m-j);
		j += 8;
		}
	else if(j<i)
		{
		kernel_dtrsm_nt_rl_inv_4x4_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j], m-i, m-j);
		j += 4;
		}
	kernel_dpotrf_nt_l_4x4_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j], m-i, m-j);
	return;
#else
	left_8:
	j = 0;
	if(m-i==8)
		{
		for(; j<i; j+=8)
			{
			kernel_dtrsm_nt_rl_inv_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j]);
			}
		kernel_dpotrf_nt_l_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j]);
		}
	else
		{
		for(; j<i; j+=8)
			{
			kernel_dtrsm_nt_rl_inv_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j], m-i, m-j);
			}
		kernel_dpotrf_nt_l_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j], m-i, m-j);
		}
	return;
#endif

	}



// dpotrf
void blasfeo_hp_dpotrf_l_mn(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	if(m<=0 || n<=0)
		return;

	if(ci!=0 | di!=0)
		{
#if defined(BLASFEO_REF_API)
		blasfeo_ref_dpotrf_l_mn(m, n, sC, ci, cj, sD, di, dj);
		return;
#else
		printf("\nblasfeo_dpotrf_l_mn: feature not implemented yet: ci=%d, di=%d\n", ci, di);
		exit(1);
#endif
		}

	const int ps = 8;

	double alpha = 1.0;

	int sdc = sC->cn;
	int sdd = sD->cn;
	double *pC = sC->pA + cj*ps;
	double *pD = sD->pA + dj*ps;
	double *dD = sD->dA;

	if(di==0 & dj==0) // XXX what to do if di and dj are not zero
		sD->use_dA = 1;
	else
		sD->use_dA = 0;

	int i, j, l;

	i = 0;
#if 0
	for(; i<m-7; i+=8)
		{
		j = 0;
		for(; j<i & j<n-3; j+=4)
			{
			kernel_dtrsm_nt_rl_inv_8x4_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], sdc, &pD[j*ps+i*sdd], sdd, &pD[j*ps+j*sdd], &dD[j]);
			}
		if(j<n)
			{
			if(j<i) // dtrsm
				{
				kernel_dtrsm_nt_rl_inv_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], sdc, &pD[j*ps+i*sdd], sdd, &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
				}
			else // dpotrf
				{
				if(j<n-7)
//				if(0)
					{
					kernel_dpotrf_nt_l_8x4_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &pC[j*ps+j*sdc], sdc, &pD[j*ps+j*sdd], sdd, &dD[j]);
					kernel_dpotrf_nt_l_4x4_lib4(j+4, &pD[(i+4)*sdd], &pD[(j+4)*sdd], &pC[(j+4)*ps+(i+4)*sdc], &pD[(j+4)*ps+(i+4)*sdd], &dD[j+4]);
					}
				else
					{
					kernel_dpotrf_nt_l_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &pC[j*ps+j*sdc], sdc, &pD[j*ps+j*sdd], sdd, &dD[j], m-i, n-j);
					if(j<n-4)
						{
						kernel_dpotrf_nt_l_4x4_vs_lib4(j+4, &pD[(i+4)*sdd], &pD[(j+4)*sdd], &pC[(j+4)*ps+(i+4)*sdc], &pD[(j+4)*ps+(i+4)*sdd], &dD[j+4], m-i-4, n-j-4);
						}
					}
				}
			}
		}
	if(m>i)
		{
		if(m-i<=4)
			{
			goto left_4;
			}
		else
			{
			goto left_8;
			}
		}
#else
	for(; i<m-7; i+=8)
		{
		j = 0;
		for(; j<i & j<n-7; j+=8)
			{
			kernel_dtrsm_nt_rl_inv_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j]);
			}
		if(j<n)
			{
			if(j<i) // dtrsm
				{
				kernel_dtrsm_nt_rl_inv_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
				}
			else // dpotrf
				{
				if(j<n-7)
					{
					kernel_dpotrf_nt_l_8x8_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j]);
					}
				else
					{
					kernel_dpotrf_nt_l_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
					}
				}
			}
		}
	if(m>i)
		{
		goto left_8;
		}
#endif

	// common return if i==m
	return;

	// clean up loops definitions

#if 0
	left_8:
	j = 0;
	for(; j<i & j<n; j+=4)
		{
		kernel_dtrsm_nt_rl_inv_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], sdc, &pD[j*ps+i*sdd], sdd, &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
		}
	if(j<n)
		{
		kernel_dpotrf_nt_l_8x4_vs_lib4(j, &pD[i*sdd], sdd, &pD[j*sdd], &pC[j*ps+j*sdc], sdc, &pD[j*ps+j*sdd], sdd, &dD[j], m-i, n-j);
		if(j<n-4)
			{
			kernel_dpotrf_nt_l_4x4_vs_lib4(j+4, &pD[(i+4)*sdd], &pD[(j+4)*sdd], &pC[(j+4)*ps+(i+4)*sdc], &pD[(j+4)*ps+(i+4)*sdd], &dD[j+4], m-i-4, n-j-4);
			}
		}
	return;
#endif

#if 0
	left_4:
	j = 0;
	for(; j<i-8 & j<n-8; j+=12)
		{
		kernel_dtrsm_nt_rl_inv_4x12_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], sdd, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], sdd, &dD[j], m-i, n-j);
		}
	if(j<i-4 & j<n-4)
		{
		kernel_dtrsm_nt_rl_inv_4x8_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], sdd, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], sdd, &dD[j], m-i, n-j);
		j += 8;
		}
	else if(j<i & j<n)
		{
		kernel_dtrsm_nt_rl_inv_4x4_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
		j += 4;
		}
	if(j<n)
		{
		kernel_dpotrf_nt_l_4x4_vs_lib4(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
		}
	return;
#else
	left_8:
	j = 0;
	for(; j<i & j<n; j+=8)
		{
		kernel_dtrsm_nt_rl_inv_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &alpha, &pC[j*ps+i*sdc], &pD[j*ps+i*sdd], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
		}
	if(j<n)
		{
		kernel_dpotrf_nt_l_8x8_vs_lib8(j, &pD[i*sdd], &pD[j*sdd], &pC[j*ps+j*sdc], &pD[j*ps+j*sdd], &dD[j], m-i, n-j);
		}
	return;
#endif

	}



// dpotrf
void blasfeo_hp_dpotrf_u(int m, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dpotrf_u(m, sC, ci, cj, sD, di, dj);
#else
	printf("\nblasfeo_dpotrf_u: feature not implemented yet\n");
	exit(1);
#endif
	
	}



// dsyrk dpotrf
void blasfeo_hp_dsyrk_dpotrf_ln_mn(int m, int n, int k, struct blasfeo_dmat *sA, int ai, int aj, struct blasfeo_dmat *sB, int bi, int bj, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dsyrk_dpotrf_ln_mn(m, n, k, sA, ai, aj, sB, bi, bj, sC, ci, cj, sD, di, dj);
#else
	printf("\nblasfeo_dsyrk_dpotrf_ln_mn: feature not implemented yet\n");
	exit(1);
#endif
	}



void blasfeo_hp_dsyrk_dpotrf_ln(int m, int k, struct blasfeo_dmat *sA, int ai, int aj, struct blasfeo_dmat *sB, int bi, int bj, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dsyrk_dpotrf_ln(m, k, sA, ai, aj, sB, bi, bj, sC, ci, cj, sD, di, dj);
#else
	printf("\nblasfeo_dsyrk_dpotrf_ln: feature not implemented yet\n");
	exit(1);
#endif
	}



// dgetrf no pivoting
void blasfeo_hp_dgetrf_np(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgetrf_np(m, n, sC, ci, cj, sD, di, dj);
#else
	printf("\nblasfeo_dgetf_np: feature not implemented yet\n");
	exit(1);
#endif
	}



// dgetrf row pivoting
void blasfeo_hp_dgetrf_rp(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, int *ipiv)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgetrf_rp(m, n, sC, ci, cj, sD, di, dj, ipiv);
#else
	printf("\nblasfeo_dgetrf_rp: feature not implemented yet\n");
	exit(1);
#endif
	}



int blasfeo_hp_dgeqrf_worksize(int m, int n)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgeqrf_worksize(m, n);
#else
	printf("\nblasfeo_dgeqrf_worksize: feature not implemented yet\n");
	exit(1);
#endif
	}



void blasfeo_hp_dgeqrf(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgeqrf(m, n, sC, ci, cj, sD, di, dj, work);
#else
	printf("\nblasfeo_dgeqrf: feature not implemented yet\n");
	exit(1);
#endif
	}



int blasfeo_hp_dgelqf_worksize(int m, int n)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgelqf_worksize(m, n);
#else
	printf("\nblasfeo_dgelqf_worksize: feature not implemented yet\n");
	exit(1);
#endif
	}



void blasfeo_hp_dgelqf(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgelqf(m, n, sC, ci, cj, sD, di, dj, work);
#else
	printf("\nblasfeo_dgelqf: feature not implemented yet\n");
	exit(1);
#endif
	}



int blasfeo_hp_dorglq_worksize(int m, int n, int k)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dorglq_worksize(m, n, k);
#else
	printf("\nblasfeo_dorglq_worksize: feature not implemented yet\n");
	exit(1);
#endif
	}



void blasfeo_hp_dorglq(int m, int n, int k, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dorglq(m, n, k, sC, ci, cj, sD, di, dj, work);
#else
	printf("\nblasfeo_dorglq: feature not implemented yet\n");
	exit(1);
#endif
	}



// LQ factorization with positive diagonal elements
void blasfeo_hp_dgelqf_pd(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgelqf_pd(m, n, sC, ci, cj, sD, di, dj, work);
#else
	printf("\nblasfeo_dgelqf_pd: feature not implemented yet\n");
	exit(1);
#endif
	}



// LQ factorization with positive diagonal elements, array of matrices
// [L, A] <= lq( [L. A] )
// L lower triangular, of size (m)x(m)
// A full of size (m)x(n1)
void blasfeo_hp_dgelqf_pd_la(int m, int n1, struct blasfeo_dmat *sD, int di, int dj, struct blasfeo_dmat *sA, int ai, int aj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgelqf_pd_la(m, n1, sD, di, dj, sA, ai, aj, work);
#else
	printf("\nblasfeo_dgelqf_pd_la: feature not implemented yet\n");
	exit(1);
#endif
	}



// LQ factorization with positive diagonal elements, array of matrices
// [L, L, A] <= lq( [L. L, A] )
// L lower triangular, of size (m)x(m)
// A full of size (m)x(n1)
void blasfeo_hp_dgelqf_pd_lla(int m, int n1, struct blasfeo_dmat *sD, int di, int dj, struct blasfeo_dmat *sL, int li, int lj, struct blasfeo_dmat *sA, int ai, int aj, void *work)
	{
#if defined(BLASFEO_REF_API)
	blasfeo_ref_dgelqf_pd_lla(m, n1, sD, di, dj, sL, li, lj, sA, ai, aj, work);
#else
	printf("\nblasfeo_dgelqf_pd_lla: feature not implemented yet\n");
	exit(1);
#endif
	}



#if defined(LA_HIGH_PERFORMANCE)



void blasfeo_dpotrf_l(int m, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dpotrf_l(m, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dpotrf_l_mn(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dpotrf_l_mn(m, n, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dpotrf_u(int m, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dpotrf_u(m, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dsyrk_dpotrf_ln_mn(int m, int n, int k, struct blasfeo_dmat *sA, int ai, int aj, struct blasfeo_dmat *sB, int bi, int bj, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dsyrk_dpotrf_ln_mn(m, n, k, sA, ai, aj, sB, bi, bj, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dsyrk_dpotrf_ln(int m, int k, struct blasfeo_dmat *sA, int ai, int aj, struct blasfeo_dmat *sB, int bi, int bj, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dsyrk_dpotrf_ln(m, k, sA, ai, aj, sB, bi, bj, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dgetrf_np(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj)
	{
	blasfeo_hp_dgetrf_np(m, n, sC, ci, cj, sD, di, dj);
	}



void blasfeo_dgetrf_rp(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, int *ipiv)
	{
	blasfeo_hp_dgetrf_rp(m, n, sC, ci, cj, sD, di, dj, ipiv);
	}



int blasfeo_dgeqrf_worksize(int m, int n)
	{
	return blasfeo_hp_dgeqrf_worksize(m, n);
	}



void blasfeo_dgeqrf(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *v_work)
	{
	blasfeo_hp_dgeqrf(m, n, sC, ci, cj, sD, di, dj, v_work);
	}



int blasfeo_dgelqf_worksize(int m, int n)
	{
	return blasfeo_hp_dgelqf_worksize(m, n);
	}



void blasfeo_dgelqf(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
	blasfeo_hp_dgelqf(m, n, sC, ci, cj, sD, di, dj, work);
	}



int blasfeo_dorglq_worksize(int m, int n, int k)
	{
	return blasfeo_hp_dorglq_worksize(m, n, k);
	}



void blasfeo_dorglq(int m, int n, int k, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
	blasfeo_hp_dorglq(m, n, k, sC, ci, cj, sD, di, dj, work);
	}



void blasfeo_dgelqf_pd(int m, int n, struct blasfeo_dmat *sC, int ci, int cj, struct blasfeo_dmat *sD, int di, int dj, void *work)
	{
	blasfeo_hp_dgelqf_pd(m, n, sC, ci, cj, sD, di, cj, work);
	}



void blasfeo_dgelqf_pd_la(int m, int n1, struct blasfeo_dmat *sD, int di, int dj, struct blasfeo_dmat *sA, int ai, int aj, void *work)
	{
	blasfeo_hp_dgelqf_pd_la(m, n1, sD, di, dj, sA, ai, aj, work);
	}



void blasfeo_dgelqf_pd_lla(int m, int n1, struct blasfeo_dmat *sD, int di, int dj, struct blasfeo_dmat *sL, int li, int lj, struct blasfeo_dmat *sA, int ai, int aj, void *work)
	{
	blasfeo_hp_dgelqf_pd_lla(m, n1, sD, di, dj, sL, li, lj, sA, ai, aj, work);
	}



#endif

