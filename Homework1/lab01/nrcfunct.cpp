#include "defmat.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double at,bt,ct;
#define PYTHAG(a,b) ((at=fabs(a)) > (bt=fabs(b)) ? \
(ct=bt/at,at*sqrt(1.0+ct*ct)) : (bt ? (ct=at/bt,bt*sqrt(1.0+ct*ct)): 0.0))

static double maxarg1,maxarg2;
#define MAXIMUM(a,b) (maxarg1=(a),maxarg2=(b),(maxarg1) > (maxarg2) ?\
    (maxarg1) : (maxarg2))
#define SIGN(a,b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#define SWAP(g,h) {double y=(g);(g)=(h);(h)=y;}

#define PI 3.141592654
#define M 714025
#define IA 1366
#define IC 150889
#define M1 259200
#define IA1 7141
#define IC1 54773
#define RM1 (1.0/M1)
#define M2 134456
#define IA2 8121
#define IC2 28411
#define RM2 (1.0/M2)
#define M3 243000
#define IA3 4561
#define IC3 51349
#define MBIG 1000000000
#define MSEED 161803398
#define MZ 0
#define FAC (1.0/MBIG)
#define TINY 1.0e-20
#define IM 11979
#define IIA 430
#define IIC 2531
#define NACC 24
#define IB1 1L
#define IB3 4L
#define IB4 8L
#define IB32 0x80000000L
#define MASK IB1+IB3+IB4
#define ITMAX 100
#define EPS 3.0e-7
#define ACC 40.0
#define BIGNO 1.0e10
#define BIGNI 1.0e-10

typedef struct IMMENSE {unsigned long l,r;} immense;
typedef struct GREAT {unsigned short l,c,r;} great;
unsigned long bit[33];  /* defining declaration */

/*                      Inverse Matrix                              */

int     gaussj(double **a, int n,double **b, int m)
{
    int *indxc,*indxr,*ipiv,error=TRUE;
    int i,icol,irow,j,k,l,ll,*ivector(int, int);
    double big,dum,pivinv,fabs(double);
    void free_ivector(int *, int, int);

    indxc=ivector(1,n);
    indxr=ivector(1,n);
    ipiv=ivector(1,n);
    for (j=1;j<=n;j++) ipiv[j]=0;
    for (i=1;i<=n;i++) {
        big=0.0;
        for (j=1;j<=n;j++) {
            if (ipiv[j] != 1) {
                for (k=1;k<=n;k++) {
                    if (ipiv[k] == 0) {
                        if (fabs(a[j][k]) >= big) {
                            big=fabs(a[j][k]);
                            irow=j;
                            icol=k;
                        }
                    }
                    else if (ipiv[k] > 1) {
                        error = FALSE;
                        goto Return;
                    }
                }
            }
        }
        ++(ipiv[icol]);
        if (irow != icol) {
            for (l=1;l<=n;l++) SWAP(a[irow][l],a[icol][l])
            for (l=1;l<=m;l++) SWAP(b[irow][l],b[icol][l])
        }
        indxr[i]=irow;
        indxc[i]=icol;
        if (fabs(a[icol][icol]) < TINY) {
            error = FALSE;
            goto Return;
        }
        pivinv=1.0/a[icol][icol];
        a[icol][icol]=1.0;
        for (l=1;l<=n;l++) a[icol][l] *= pivinv;
        for (l=1;l<=m;l++) b[icol][l] *= pivinv;
        for (ll=1;ll<=n;ll++) {
            if (ll != icol) {
                dum=a[ll][icol];
                a[ll][icol]=0.0;
                for (l=1;l<=n;l++) a[ll][l] -= a[icol][l]*dum;
                for (l=1;l<=m;l++) b[ll][l] -= b[icol][l]*dum;
            }
        }
    }
    for (l=n;l>=1;l--) {
        if (indxr[l] != indxc[l]) {
            for (k=1;k<=n;k++)
                SWAP(a[k][indxr[l]],a[k][indxc[l]]);
        }
    }
Return:
    free_ivector(ipiv,1,n);
    free_ivector(indxr,1,n);
    free_ivector(indxc,1,n);
    return(error);
}

int *ivector(int nl,int nh)
{
    int *v;

    v=(int *)malloc((unsigned) (nh-nl+1)*sizeof(int));
    return v-nl;
}

void free_ivector(int *v, int nl,int nh)
{
    free((char*) (v+nl));
}

void free_dmatrix(double **m,int nrl,int nrh,int ncl,int nch)
{
    int i;
    for(i=1;i<=nrh;i++) free(m[i]);
    free(m);
}

double **dmatrix(int nrl,int nrh,int ncl,int nch)
{
    int i, j;
    double **m;
    m = (double **)calloc(nrh+1, sizeof(double));
    for(i=1; i<=nrh; i++)
        m[i] = (double *)calloc(nch+1, sizeof(double));
    for(i=1;i<=nrh;i++)
		for(j=1;j<=nch;j++) m[i][j]=0.0;
    return m;
}

void print_dmatrix(FILE *fp, double **m, int nrl,int nrh,int ncl,int nch)
{
    int i, j;
    for(i=1;i<=nrh;i++) {
		for(j=1;j<=nch;j++)
			fprintf(fp, "%7.3f ", m[i][j]);
		fprintf(fp, "\n");
	}
    for(i=1;i<=nrh;i++) {
		for(j=1;j<=nch;j++)
			printf("%7.3f ", m[i][j]);
		printf("\n");
	}
}

double *dvector(int nl,int nh)
{
    double *v;

    v=(double *)malloc((unsigned) (nh-nl+1)*sizeof(double));
    return v-nl;
}

void free_dvector(double *v,int nl,int nh)
{
    free((char*) (v+nl));
}


double  round(double input) {   return((int) input);    }


double   absolute(double f)
{
    if(f < 0.0)     return(-f);
    else            return(f);
}

int inverse(double *m)
{
	int     i, j, row, col, out, gaussj(double **, int, double **, int);
	double  **a, **b, **dmatrix(int, int, int, int);
	void    free_dmatrix(double **);

	row = col = 3;
    a = dmatrix(1, row, 1, row);        
	b = dmatrix(1, row, 1, row);
    for(i=1; i<=row; i++)               
		b[i][i] = 1.0;
    for(i=0; i<row; i++) 
		for(j=0; j<row; j++)
			a[i+1][j+1] = (double) *(m+i*row+j);
    out = gaussj(a,row,b,row);
    if(out != TRUE) return FALSE;
    for(i=0; i<row; i++) 
		for(j=0; j<row; j++)
			*(m+i*row+j) = (float) a[i+1][j+1];
	return TRUE;
}


