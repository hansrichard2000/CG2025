/* This program illustrates the use of the glut library for interfacing with a window system */
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "glf.h" 

#define MAX			1024

#define N			100
#define TRUE		1
#define FALSE		0

#define LAGRANGE	1
#define BEZIER		2
#define RBFINT		3
#define BSPLINE		4
#define XMARGIN		25
#define YMARGIN		25

/* globals */

GLsizei wh=600, ww=800;			/* initial window size */
GLfloat r=0.3, g=0.5, b=0.1;	/* drawing color */
int		fill=0;					/* fill flag */
int		count=0, icount=0, knotcount=0;
int		color=1;
int		active=0;
int		dim=2;
int		order=4;
int		index=-1;
int		kindex=-1;
int		width=4;
int		poly_deg=1;
int		base_mode=0;
int		subdivision_mode=4;
int		display_mode=FALSE;
bool	station_mode=FALSE;
bool	debug_mode=FALSE;
bool	idle_mode=FALSE;

double	CP[MAX][4], RBF[MAX][4], nCP[MAX][4], iCP[MAX][4];
double	lT[MAX], lCP[MAX][4], Mask[N][N];
double	T[MAX], TP[MAX][4], deltaT=1.0, deltaR=0.025, deltaW=0.001, R=1.0, W=1./16.;
double	scale=1.0, xpos=0., ypos=0.;
double	scaleDelta=1.125, C=1.0, idletv=-1, tv=-1.;
FILE	*fp;

void	myMouse(int, int, int, int);
void	myDisplay(void);
void	myReshape(GLsizei, GLsizei);
void	right_menu(int);
double	myBasis(double x, double r);
void	free_dmatrix(double **m,int nrl,int nrh,int ncl,int nch);
void	print_dmatrix(FILE *fp, double **m, int nrl,int nrh,int ncl,int nch);
double	**dmatrix(int nrl,int nrh,int ncl,int nch);
double	Bsplineft(double *Recknots, int cnt, int j, int m, double t);
int		inverse(double *m);
int     gaussj(double **a, int n,double **b, int m);
void	myRBFCoefficient();
void	myRBFCalculate(double *v, double *t, double x, int size);
void	myLagrangeCalculate(double *v, double *t, double x, int size);
void	myLagrangeCalculateDisplay(double *v, double *t, double x, int size);
void	myBezierCalculateDisplay(double *v, double *t, double x, int size);
void	myBezierCalculate(double *v, double *t, double x, int size);
void	myBSplineCalculateDisplay(double *v, double *t, double x, int size);
void	myBSplineCalculate(double *v, double *t, double x, int size);
void	myMakeKnotTPVectors();
void	myBezierDegreeElevation();
void	myBSplineKnotInsertion();
void	myBox(double x, double y, int flag);
void	myNR4Subdivision();
void	myDDSubdivision();
void	myInitialSubdivision();
void	myRBFDisplay();
void	myMakeKnotVectors();
void	myMakeTPKnotVectors();
void	myKnotVectors();
void	myLagrange();
void	myIdleFunc();
void	myCaption();
void	myTextCaption();
void	myBSpline();
void	myBezier();
void	myPrintCP();
void	myPrintlCP();
void	myCaption();
void	myDeletePoint();
void	myIntoduction();
double	combi(int n, int i); 

void myRBFCoefficient()
{
	int	i, j;
	double **M, **I;

	if(count > 200) return;
    M = dmatrix(1, count+poly_deg+1, 1, count+poly_deg+1);        
    I = dmatrix(1, count+poly_deg+1, 1, count+poly_deg+1);        
	for(i=1; i<=count+poly_deg+1; i++) I[i][i]=1.0;
	for(i=1; i<=count; i++)
		for(j=1; j<=poly_deg+1; j++)
			M[j][i+poly_deg+1]=pow(T[i-1],(double)j-1.);
	for(i=1; i<=count; i++) {
		for(j=1; j<=poly_deg+1; j++)
			M[i+poly_deg+1][j]=pow(T[i-1],(double)j-1);
		for(j=1; j<=count; j++)
			M[i+poly_deg+1][j+poly_deg+1]=myBasis(T[j-1]-T[i-1],R);
	}
//	print_dmatrix(M, 1, count+poly_deg+1, 1, count+poly_deg+1);    
    gaussj(M,count+poly_deg+1,I,count+poly_deg+1);
//	print_dmatrix(M, 1, count+poly_deg+1, 1, count+poly_deg+1);    

	for(i=1; i<=count+poly_deg+1; i++) {
		RBF[i-1][0]=RBF[i-1][1]=0.;
		for(j=1; j<=count; j++) {
			RBF[i-1][0]+=M[i][j+poly_deg+1]*CP[j-1][0];
			RBF[i-1][1]+=M[i][j+poly_deg+1]*CP[j-1][1];
		}
//		printf("%7.3f %7.3f ", RBF[i-1][0], RBF[i-1][1]);
	}
	free_dmatrix(I, 1, count+poly_deg+1, 1, count+poly_deg+1);
	free_dmatrix(M, 1, count+poly_deg+1, 1, count+poly_deg+1);
}	

void myRBFSubdivision(int order)
{
	int	i, j, k, m=order/2-1, li, ri, p;
	double x, v[64], w[64], a[64], tx, ty;

	if(count*2 > MAX) return;

	for(k=0; k<count; k++) {
		li=k-m; ri=k+1+m;
		for(i=li; i<=ri; i++) {
			if(i<0) p=count+i;
			else if(i>=count) p=i-count;
			else p=i;
			for(j=0; j<dim; j++) lCP[i-li][j]=CP[p][j];
			lT[i-li]=T[p];
		}
		
		for(i=li; i<0; i++) lT[i-li]=lT[i-li]-T[count-1]-deltaT;
		for(i=count; i<=ri; i++) lT[i-li]=T[count-1]+deltaT+lT[i-li];
//		printf("%5.2f %5.2f %5.2f %5.2f\n", lT[0], lT[1], lT[2], lT[3]);

		myInitialSubdivision();

		for(i=0; i<order+poly_deg+1; i++) {
			RBF[i][0]=RBF[i][1]=0.;
			for(j=0; j<order; j++) {
				RBF[i][0]+=Mask[i][j+poly_deg+1]*lCP[j][0];
				RBF[i][1]+=Mask[i][j+poly_deg+1]*lCP[j][1];
			}
		}
		x=(lT[order-1]+lT[0])/2.;
		myRBFCalculate(v, lT, x, order);
		for(i=0; i<poly_deg+1; i++)
			w[i]=pow(x,(double)i);
		for(i=0; i<order; i++)
			w[poly_deg+i+1]=myBasis(x-lT[i],R);
		for(i=0; i<order; i++) {
			a[i]=0.;
			for(j=0; j<order+poly_deg+1; j++)
				a[i]+=w[j]*Mask[j][i+poly_deg+1];
		}
		tx = ty = 0.;
		for(i=0; i<order; i++) {
			tx += a[i]*lCP[i][0];
			ty += a[i]*lCP[i][1];
		}
		if(debug_mode) {
			printf("%3d :", k);
			for(i=0; i<order; i++) printf("%7.4f ", a[i]);
			printf("%7.4f %7.4f", tx, ty);
			fprintf(fp, "%3d :", k);
			for(i=0; i<order; i++) fprintf(fp, "%7.4f ", a[i]);
			fprintf(fp, "(%7.4f, %7.4f) ", tx, ty);
		}

		for(i=0; i<dim; i++) {
			nCP[2*k][i]=CP[k][i];
			if(debug_mode) {
				printf("%7.4f ", v[i]);
				fprintf(fp, "%7.4f ", v[i]);
			}
			nCP[2*k+1][i]=v[i];
		}
		if(debug_mode) {
			printf("\n\n");
			fprintf(fp, "\n\n");
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if(icount>0) {
		glBegin(GL_LINE_STRIP);
		for(i=0; i<icount; i++)
			glVertex2d(iCP[i][0],wh-iCP[i][1]);
		glEnd();
	}

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	count*=2;
	deltaT*=.5;
	for(i=0; i<count; i++) {
		for(j=0; j<dim; j++)
			CP[i][j]=nCP[i][j];
		T[i]=i*deltaT;
	}

	glColor3f(b, g, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++) 
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	if(station_mode) R*=0.5;
//	myCaption();
	glFlush();
	glutSwapBuffers();

	myRBFCoefficient();
}	

void	myBox(double x, double y, int flag)
{
	glBegin(flag);
		glVertex2d(x+width,wh-y-width);
		glVertex2d(x+width,wh-y+width);
		glVertex2d(x-width,wh-y+width);
		glVertex2d(x-width,wh-y-width);
	glEnd();
}

void	myInitialSubdivision()
{
	int		i, j;
	double	**M, **I;

	M = dmatrix(1, order+poly_deg+1, 1, order+poly_deg+1);        
    I = dmatrix(1, order+poly_deg+1, 1, order+poly_deg+1);        
	for(i=1; i<=order+poly_deg+1; i++) {
		for(j=1; j<=order+poly_deg+1; j++) 
			M[i][j]=I[i][j]=0.0;
		I[i][i]=1.0;
	}
	for(i=1; i<=order; i++)
		for(j=1; j<=poly_deg+1; j++)
			M[j][i+poly_deg+1]=pow(lT[i-1],(double)j-1.);
	for(i=1; i<=order; i++) {
		for(j=1; j<=poly_deg+1; j++)
			M[i+poly_deg+1][j]=pow(lT[i-1],(double)j-1.);
		for(j=1; j<=order; j++)
			M[i+poly_deg+1][j+poly_deg+1]=myBasis(lT[j-1]-lT[i-1],R);
	}
	if(debug_mode)
		print_dmatrix(fp, M, 1, order+poly_deg+1, 1, order+poly_deg+1);    
	gaussj(M,order+poly_deg+1,I,order+poly_deg+1);
	if(debug_mode)
		print_dmatrix(fp, M, 1, order+poly_deg+1, 1, order+poly_deg+1);    

	for(i=0; i<order+poly_deg+1; i++) 
		for(j=0; j<order+poly_deg+1; j++) 
			Mask[i][j]=M[i+1][j+1];

	free_dmatrix(I, 1, order+poly_deg+1, 1, count+poly_deg+1);
	free_dmatrix(M, 1, order+poly_deg+1, 1, count+poly_deg+1);
}

void myNR4Subdivision()
{
	int	order=4, i, j, k, m=order/2-1, li, ri, p;

	if(count*2 > MAX) return;

	for(k=0; k<count; k++) {
		li=k-m; ri=k+1+m;
		for(i=li; i<=ri; i++) {
			if(i<0) p=count+i;
			else if(i>=count) p=i-count;
			else p=i;
			for(j=0; j<dim; j++) lCP[i-li][j]=CP[p][j];
		}
		for(i=0; i<dim; i++) {
			nCP[2*k][i]=CP[k][i];
			nCP[2*k+1][i]=(1./2.+W)*(lCP[1][i]+lCP[2][i])-W*(lCP[0][i]+lCP[3][i]);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if(icount>0) {
		glBegin(GL_LINE_STRIP);
		for(i=0; i<icount; i++)
			glVertex2d(iCP[i][0],wh-iCP[i][1]);
		glEnd();
	}

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	count*=2;
	deltaT*=.5;
	for(i=0; i<count; i++) {
		for(j=0; j<dim; j++)
			CP[i][j]=nCP[i][j];
		T[i]=i*deltaT;
	}

	glColor3f(b, r, g);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++) 
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

//	myCaption();
	glFlush();
	glutSwapBuffers();
	myRBFCoefficient();
}	

void myUNR4Subdivision()
{
	int	order=4, i, j, k, m=order/2-1, li, ri, p;

	if(count*2 > MAX) return;

	for(k=0; k<count; k++) {
		li=k-m; ri=k+1+m;
		for(i=li; i<=ri; i++) {
			if(i<0) p=count+i;
			else if(i>=count) p=i-count;
			else p=i;
			for(j=0; j<dim; j++) lCP[i-li][j]=CP[p][j];
		}
		for(i=0; i<dim; i++) {
			nCP[2*k][i]=(1.+W)*((3.+2.*W)/(4.*(1.+W))*lCP[1][i]+
				(1.+2.*W)/(4.*(1.+W))*lCP[2][i])-W/2.*(lCP[0][i]+lCP[3][i]);
			nCP[2*k+1][i]=(1.+W)*((1.+2.*W)/(4.*(1.+W))*lCP[1][i]+
				(3.+2.*W)/(4.*(1.+W))*lCP[2][i])-W/2.*(lCP[0][i]+lCP[3][i]);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if(icount>0) {
		glBegin(GL_LINE_LOOP);
		for(i=0; i<icount; i++)
			glVertex2d(iCP[i][0],wh-iCP[i][1]);
		glEnd();
	}

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	count*=2;
	deltaT*=.5;
	for(i=0; i<count; i++) {
		for(j=0; j<dim; j++) CP[i][j]=nCP[i][j];
		T[i]=i*deltaT;
	}

	glColor3f(b, r, g);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++) 
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	myCaption();
	glFlush();
	glutSwapBuffers();
	myRBFCoefficient();
}	

void myDDSubdivision()
{
	int		order=4, i, j, k, m=order/2-1, li, ri, p;
	double	w=3./48.;

	if(count*2 > MAX) return;
	for(k=0; k<count; k++) {
		li=k-m; ri=k+1+m;
		for(i=li; i<=ri; i++) {
			if(i<0) p=count+i;
			else if(i>=count) p=i-count;
			else p=i;
			for(j=0; j<dim; j++) lCP[i-li][j]=CP[p][j];
		}
		for(i=0; i<dim; i++) {
			nCP[2*k][i]=CP[k][i];
			nCP[2*k+1][i]=(1./2.+w)*(lCP[1][i]+lCP[2][i])-w*(lCP[0][i]+lCP[3][i]);
		}
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	if(icount>0) {
		glBegin(GL_LINE_STRIP);
		for(i=0; i<icount; i++)
			glVertex2d(iCP[i][0],wh-iCP[i][1]);
		glEnd();
	}

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	count*=2;
	deltaT*=.5;
	for(i=0; i<count; i++) {
		for(j=0; j<dim; j++)
			CP[i][j]=nCP[i][j];
		T[i]=i*deltaT;
	}

	glColor3f(b, r, g);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++) 
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

//	myCaption();
	glFlush();
	glutSwapBuffers();
	myRBFCoefficient();
}	

void	myPrintCP()
{
	for(int i=0; i<count; i++) 
		printf("%5.2f %5.2f ", CP[i][0], CP[i][1]);
	printf("\n");
}
	
void	myPrintlCP()
{
	for(int i=0; i<order; i++) 
		printf("%5.2f %5.2f ", lCP[i][0], lCP[i][1]);
	printf("\n");
}

void	myRBFDisplay()
{
	int		i;
	double	x, v[3];
	if(count<=0 || count > 200) return;

	glColor3f(r, 0., 0.);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<=MAX; i++) {
		x=(T[count-1]-T[0])*i/MAX;
		myRBFCalculate(v, T, x, count);
		v[1]=wh-v[1];
		glVertex3dv(v);
	}
	glEnd();
}

void	myLagrange()
{
	int		i;
	double	x, v[3];

	if(count<=0 || count > 200) return;
	glColor3f(r, 0., 0.);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<=MAX; i++) {
		x=(T[count-1]-T[0])*i/MAX+T[0];
		myLagrangeCalculate(v, T, x, count);
		v[1]=wh-v[1];
		glVertex3dv(v);
	}
	glEnd();
	tv=(T[count-1]-T[0])*idletv+T[0];
	myLagrangeCalculateDisplay(v, T, tv, count);
}

void	myLagrangeCalculate(double *v, double *t, double x, int size)
{
	int		i, j;
	double	l;

	v[0]=v[1]=v[2]=0.;
	for(i=0; i<size; i++) {
		l=1.;
		for(j=0; j<size; j++) {
			if(j!=i) l*=(x-t[j])/(t[i]-t[j]);
		}	
		v[0]+=CP[i][0]*l;
		v[1]+=CP[i][1]*l;
	}
}	

void	myLagrangeCalculateDisplay(double *v, double *t, double x, int size)
{
	if(x<0. || x>T[size-1]) return;
	if(size==3) {
//		x=x*(T[2]-T[0])+T[0];
		lCP[0][0]=(T[2]-T[0])/(T[1]-T[0])*CP[1][0]-(T[2]-T[1])/(T[1]-T[0])*CP[0][0];
		lCP[0][1]=(T[2]-T[0])/(T[1]-T[0])*CP[1][1]-(T[2]-T[1])/(T[1]-T[0])*CP[0][1];

		lCP[2][0]=(T[2]-T[0])/(T[2]-T[1])*CP[1][0]-(T[1]-T[0])/(T[2]-T[1])*CP[2][0];
		lCP[2][1]=(T[2]-T[0])/(T[2]-T[1])*CP[1][1]-(T[1]-T[0])/(T[2]-T[1])*CP[2][1];
		glBegin(GL_LINES);
			glVertex2f(CP[0][0], wh-CP[0][1]); 
			glVertex2f(lCP[0][0], wh-lCP[0][1]);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(CP[2][0], wh-CP[2][1]); 
			glVertex2f(lCP[2][0], wh-lCP[2][1]); 
		glEnd();

		lCP[1][0]=(T[2]-x)/(T[2]-T[0])*CP[0][0]+(x-T[0])/(T[2]-T[0])*lCP[0][0];
		lCP[1][1]=(T[2]-x)/(T[2]-T[0])*CP[0][1]+(x-T[0])/(T[2]-T[0])*lCP[0][1];

		lCP[3][0]=(T[2]-x)/(T[2]-T[0])*lCP[2][0]+(x-T[0])/(T[2]-T[0])*CP[2][0];
		lCP[3][1]=(T[2]-x)/(T[2]-T[0])*lCP[2][1]+(x-T[0])/(T[2]-T[0])*CP[2][1];

		lCP[4][0]=(T[2]-x)/(T[2]-T[0])*lCP[1][0]+(x-T[0])/(T[2]-T[0])*lCP[3][0];
		lCP[4][1]=(T[2]-x)/(T[2]-T[0])*lCP[1][1]+(x-T[0])/(T[2]-T[0])*lCP[3][1];

		glBegin(GL_LINES);
			glVertex2f(lCP[1][0], wh-lCP[1][1]); 
			glVertex2f(lCP[3][0], wh-lCP[3][1]); 
		glEnd();
		myBox(lCP[4][0], lCP[4][1],GL_POLYGON);
	}	
	else if(size==4) {
//		x=x*(T[3]-T[0])+T[0];
		lCP[0][0]=(T[2]-T[0])/(T[1]-T[0])*CP[1][0]-(T[2]-T[1])/(T[1]-T[0])*CP[0][0];
		lCP[0][1]=(T[2]-T[0])/(T[1]-T[0])*CP[1][1]-(T[2]-T[1])/(T[1]-T[0])*CP[0][1];

		lCP[1][0]=(T[3]-T[0])/(T[1]-T[0])*CP[1][0]-(T[3]-T[1])/(T[1]-T[0])*CP[0][0];
		lCP[1][1]=(T[3]-T[0])/(T[1]-T[0])*CP[1][1]-(T[3]-T[1])/(T[1]-T[0])*CP[0][1];

		lCP[2][0]=(T[2]-T[0])/(T[2]-T[1])*CP[1][0]-(T[1]-T[0])/(T[2]-T[1])*CP[2][0];
		lCP[2][1]=(T[2]-T[0])/(T[2]-T[1])*CP[1][1]-(T[1]-T[0])/(T[2]-T[1])*CP[2][1];

		lCP[3][0]=(T[3]-T[1])/(T[2]-T[1])*CP[2][0]-(T[3]-T[2])/(T[2]-T[1])*CP[1][0];
		lCP[3][1]=(T[3]-T[1])/(T[2]-T[1])*CP[2][1]-(T[3]-T[2])/(T[2]-T[1])*CP[1][1];

		lCP[4][0]=(T[3]-T[0])/(T[3]-T[2])*CP[2][0]-(T[2]-T[0])/(T[3]-T[2])*CP[3][0];
		lCP[4][1]=(T[3]-T[0])/(T[3]-T[2])*CP[2][1]-(T[2]-T[0])/(T[3]-T[2])*CP[3][1];

		lCP[5][0]=(T[3]-T[1])/(T[3]-T[2])*CP[2][0]-(T[2]-T[1])/(T[3]-T[2])*CP[3][0];
		lCP[5][1]=(T[3]-T[1])/(T[3]-T[2])*CP[2][1]-(T[2]-T[1])/(T[3]-T[2])*CP[3][1];

		glBegin(GL_LINES);
			glVertex2f(CP[0][0], wh-CP[0][1]); 
			glVertex2f(lCP[1][0], wh-lCP[1][1]);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(lCP[2][0], wh-lCP[2][1]); 
			glVertex2f(lCP[3][0], wh-lCP[3][1]); 
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(lCP[4][0], wh-lCP[4][1]); 
			glVertex2f(CP[3][0], wh-CP[3][1]); 
		glEnd();

		lCP[6][0]=(T[2]-x)/(T[2]-T[0])*CP[0][0]+(x-T[0])/(T[2]-T[0])*lCP[0][0];
		lCP[6][1]=(T[2]-x)/(T[2]-T[0])*CP[0][1]+(x-T[0])/(T[2]-T[0])*lCP[0][1];

		lCP[7][0]=(T[2]-x)/(T[2]-T[0])*lCP[2][0]+(x-T[0])/(T[2]-T[0])*CP[2][0];
		lCP[7][1]=(T[2]-x)/(T[2]-T[0])*lCP[2][1]+(x-T[0])/(T[2]-T[0])*CP[2][1];

		lCP[10][0]=(T[2]-x)/(T[2]-T[0])*lCP[6][0]+(x-T[0])/(T[2]-T[0])*lCP[7][0];
		lCP[10][1]=(T[2]-x)/(T[2]-T[0])*lCP[6][1]+(x-T[0])/(T[2]-T[0])*lCP[7][1];

		glBegin(GL_LINES);
			glVertex2f(lCP[6][0], wh-lCP[6][1]); 
			glVertex2f(lCP[7][0], wh-lCP[7][1]); 
		glEnd();
		myBox(lCP[10][0], lCP[10][1],GL_LINE_LOOP);

		lCP[8][0]=(T[3]-x)/(T[3]-T[1])*CP[1][0]+(x-T[1])/(T[3]-T[1])*lCP[3][0];
		lCP[8][1]=(T[3]-x)/(T[3]-T[1])*CP[1][1]+(x-T[1])/(T[3]-T[1])*lCP[3][1];

		lCP[9][0]=(T[3]-x)/(T[3]-T[1])*lCP[5][0]+(x-T[1])/(T[3]-T[1])*CP[3][0];
		lCP[9][1]=(T[3]-x)/(T[3]-T[1])*lCP[5][1]+(x-T[1])/(T[3]-T[1])*CP[3][1];

		lCP[11][0]=(T[3]-x)/(T[3]-T[1])*lCP[8][0]+(x-T[1])/(T[3]-T[1])*lCP[9][0];
		lCP[11][1]=(T[3]-x)/(T[3]-T[1])*lCP[8][1]+(x-T[1])/(T[3]-T[1])*lCP[9][1];

		glBegin(GL_LINES);
			glVertex2f(lCP[8][0], wh-lCP[8][1]); 
			glVertex2f(lCP[9][0], wh-lCP[9][1]); 
		glEnd();
		myBox(lCP[11][0], lCP[11][1],GL_LINE_LOOP);
		
		lCP[12][0]=(T[3]-x)/(T[3]-T[0])*lCP[10][0]+(x-T[0])/(T[3]-T[0])*lCP[11][0];
		lCP[12][1]=(T[3]-x)/(T[3]-T[0])*lCP[10][1]+(x-T[0])/(T[3]-T[0])*lCP[11][1];

		glBegin(GL_LINES);
			glVertex2f(lCP[10][0], wh-lCP[10][1]); 
			glVertex2f(lCP[11][0], wh-lCP[11][1]); 
		glEnd();
		myBox(lCP[12][0], lCP[12][1],GL_POLYGON);
	}
	else {
		myLagrangeCalculate(v, T, x, count);
		myBox(v[0], v[1], GL_POLYGON);
	}
}

void	myBSpline()
{
	int		i;
	double	x, v[3];

	if(count<=0 || count > 200) return;
	glColor3f(r, 0., 0.);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<MAX; i++) {
		x=(double)i/MAX*(T[knotcount-poly_deg-1]-T[poly_deg])+T[poly_deg];
		myBSplineCalculate(v, T, x, count);
		v[1]=wh-v[1];
		glVertex3dv(v);
	}
	glEnd();

	tv=(double)idletv*(T[knotcount-poly_deg-1]-T[poly_deg])+T[poly_deg];
	myBSplineCalculateDisplay(v, T, tv, count);
}

void	myBSplineCalculateDisplay(double *v, double *t, double x, int size)
{
	int		i, j;

	if(x<T[knotcount-poly_deg-1] && x>T[poly_deg]) {
		myBSplineCalculate(v, T, x, size);
		myBox(v[0], v[1], GL_POLYGON);
	}
/*
	if(x<0. || x>1.) return;

	for(i=0; i<count; i++) {
		lCP[i][0]=CP[i][0];
		lCP[i][1]=CP[i][1];
	}
	for(i=0; i<count; i++) {
		for(j=0; j<count-i-1; j++) {
			lCP[j][0]=lCP[j][0]*(1.-x)+lCP[j+1][0]*x;
			lCP[j][1]=lCP[j][1]*(1.-x)+lCP[j+1][1]*x;
		}
		glBegin(GL_LINES);
		for(j=0; j<count-i-1; j++) {
			glVertex2f(lCP[j][0], wh-lCP[j][1]); 
			glVertex2f(lCP[j+1][0], wh-lCP[j+1][1]); 
		}
		glEnd();
	
	}
	myBox(lCP[0][0], lCP[0][1],GL_LINE_LOOP);
*/
}	

void	myBSplineCalculate(double *v, double *t, double x, int size)
{
	int		i;
	double	l;

	v[0]=v[1]=v[2]=0.;
	for(i=0; i<size; i++) {
		l=Bsplineft(T, size, i, poly_deg+1, x);
		v[0]+=CP[i][0]*l;
		v[1]+=CP[i][1]*l;
	}
}	

double	Bsplineft(double *Recknots, int cnt, int j, int m, double t)
{
	double	f1=0., f2=0.;

	if(j==cnt-1 && t == Recknots[cnt]) return 1.;
	if(m==1)
	{
		if(Recknots[j] <= t && t < Recknots[j+1]) return 1.;
		else return 0.;
	}
	if(Recknots[j+m-1] != Recknots[j]) f1=(t-Recknots[j])/(Recknots[j+m-1]-Recknots[j]);
	if(Recknots[j+m] != Recknots[j+1]) f2=(Recknots[j+m]-t)/(Recknots[j+m]-Recknots[j+1]);
	return (f1*Bsplineft(Recknots, cnt, j,m-1,t)+f2*Bsplineft(Recknots, cnt, j+1,m-1,t));
}			

void	myBSplineKnotInsertion()
{
	int		i, j, k, order=poly_deg+1, knots_ins;
	double	alpha, tknot;

	if(kindex<1 || kindex>knotcount-1) return;
	knots_ins = kindex-1;
	tknot = T[kindex];
	for(j=0; j<knots_ins-order+2; j++)
 	{
		lCP[j][0]=CP[j][0];
		lCP[j][1]=CP[j][1];
	}
	for(; j<=knots_ins; j++)
	{
		alpha = (tknot-T[j])/(T[j+order-1]-T[j]);
		lCP[j][0]=((1.-alpha)*CP[j-1][0]+alpha*CP[j][0]);
		lCP[j][1]=((1.-alpha)*CP[j-1][1]+alpha*CP[j][1]);
	}
	for(; j<=count; j++)
 	{
		lCP[j][0]=CP[j-1][0];
		lCP[j][1]=CP[j-1][1];
	}

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();

	count++;
	for(j=0; j<count; j++)
	{
		CP[j][0]=lCP[j][0];
		CP[j][1]=lCP[j][1];
	}

	for(j=knotcount+order-1; j>kindex+1; j--)
		T[j] = T[j-1];
	T[j] = tknot;			
	knotcount++;
	myMakeKnotTPVectors();

	glColor3f(r, g, b);
	for(i=0; i<count; i++) {
		if(CP[i][2]) myBox(CP[i][0], CP[i][1], GL_POLYGON);
		else myBox(CP[i][0], CP[i][1], GL_LINE_LOOP);
	}

	glColor3f(b, r, g);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();
	glColor3f(g, r, b);

	if(display_mode==BSPLINE) myBSpline();
	myKnotVectors();

	glFlush();
	glutSwapBuffers();

    return;
}

void	myBezier()
{
	int		i;
	double	x, v[3];

	if(count<=0 || count > 200) return;
	glColor3f(r, 0., 0.);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<=MAX; i++) {
		x=(double)i/MAX;
		myBezierCalculate(v, T, x, count);
		v[1]=wh-v[1];
		glVertex3dv(v);
	}
	glEnd();

	tv=idletv;
	myBezierCalculateDisplay(v, T, tv, count);
}

void	myBezierCalculateDisplay(double *v, double *t, double x, int size)
{
	int		i, j;

	if(x<0. || x>1.) return;

	for(i=0; i<count; i++) {
		lCP[i][0]=CP[i][0];
		lCP[i][1]=CP[i][1];
	}
	for(i=0; i<count; i++) {
		for(j=0; j<count-i-1; j++) {
			lCP[j][0]=lCP[j][0]*(1.-x)+lCP[j+1][0]*x;
			lCP[j][1]=lCP[j][1]*(1.-x)+lCP[j+1][1]*x;
		}
		glBegin(GL_LINES);
		for(j=0; j<count-i-2; j++) {
			glVertex2f(lCP[j][0], wh-lCP[j][1]); 
			glVertex2f(lCP[j+1][0], wh-lCP[j+1][1]); 
		}
		glEnd();
	
	}
	myBox(lCP[0][0], lCP[0][1], GL_POLYGON);
}	

void	myBezierCalculate(double *v, double *t, double x, int size)
{
	int		i;
	double	l;

	v[0]=v[1]=v[2]=0.;
	for(i=0; i<size; i++) {
		l=combi(size-1,i)*pow(x, (double)i)*pow(1.-x, (double)size-1-i);
		v[0]+=CP[i][0]*l;
		v[1]+=CP[i][1]*l;
	}
}	

void	myBezierDegreeElevation()
{
	float	a[MAX][MAX], m[MAX], div=1.0, left, right, x, y;
	int		degree=1, i, j, k, n=count;

	for(i=0; i<=n; i++) {
		lCP[i][0]=CP[i][0];
		lCP[i][1]=CP[i][1];
		for(j=0; j<=n+degree; j++) a[i][j]=0.0;
	}
	for(i=0; i<=n+degree; i++) m[i]=(float) 0.0;
    for(i=0; i<degree; i++) div *= (double) n+i;
  	for(i=0; i<n; i++)
	{
		m[0]=1.0;
		for(j=1; j<=degree; j++) m[j]+=m[j-1];
		for(j=0; j<=degree; j++)
        {
        	left = 1.;                
        	for(k=0; k<j; k++) left *= (double)degree-k;
        	right = 1.;
        	for(k=0; k<degree-j; k++) right *= (double)n-i+k;
        	a[i][i+j] = (float) left*m[j]*right/div;
		}	
	}
	for(i=1; i<n+degree; i++)
	{
		x = 0.0;
		y = 0.0;
		for(j=0; j<n; j++)
		{
			x += lCP[j][0]*a[j][i];
			y += lCP[j][1]*a[j][i];	
		}		
		CP[i][0] = x;
		CP[i][1] = y;
	}	
	CP[i][0] = lCP[n-1][0];
	CP[i][1] = lCP[n-1][1];

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(g, b, r);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(lCP[i][0],wh-lCP[i][1]);
	glEnd();

	count = n+degree;
	myMakeKnotVectors();

	glColor3f(r, g, b);
	for(i=0; i<count; i++) {
		if(CP[i][2]) myBox(CP[i][0], CP[i][1], GL_POLYGON);
		else myBox(CP[i][0], CP[i][1], GL_LINE_LOOP);
	}

	glColor3f(b, r, g);
	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();
	glColor3f(g, r, b);

	if(display_mode==BEZIER) myBezier();
	myKnotVectors();

	glFlush();
	glutSwapBuffers();
}

double	combi(int	n, int 	m)
{
	int		i;
	double	total = 1.;

	if(m==0 || m==n) return 1.;
	else if(m<0 || m>n) return 1.;
	for(i=n; i>m; i--)
		total *= (double)i;
	for(i=n-m; i>0; i--)
		total /= (double)i;
	return total;		
}

void	myRBFCalculate(double *v, double *t, double x, int size)
{
	int	i;

	v[0]=v[1]=v[2]=0.;
	for(i=0; i<poly_deg+1; i++) {
		v[0]+=RBF[i][0]*pow(x, i);
		v[1]+=RBF[i][1]*pow(x, i);
	}
	for(i=0; i<size; i++) {
		v[0]+=RBF[i+poly_deg+1][0]*myBasis(x-t[i],R);
		v[1]+=RBF[i+poly_deg+1][1]*myBasis(x-t[i],R);
	}
}	

double myBasis(double x, double r)
{
	if(base_mode==0)
		return sqrt(x*x+C*C*r*r);
	else if(base_mode==1)
		return pow((x*x+C*C*r*r),1.5);
	else
		return exp(-r*x*x);
}

void myDisplay() {
	int		i;

	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(r, g, b);
	for(i=0; i<count; i++) {
		if(CP[i][2]) myBox(CP[i][0], CP[i][1], GL_POLYGON);
		else myBox(CP[i][0], CP[i][1], GL_LINE_LOOP);
	}

	glBegin(GL_LINE_STRIP);
	for(i=0; i<count; i++)
		glVertex2d(CP[i][0],wh-CP[i][1]);
	glEnd();
	glColor3f(g, r, b);
	
	if(display_mode==LAGRANGE) myLagrange();
	else if(display_mode==BEZIER) myBezier();
	else if(display_mode==BSPLINE) myBSpline();
	else if(display_mode==RBFINT) myRBFDisplay();

	myKnotVectors();

	myCaption();
	glFlush();
	glutSwapBuffers();
}

void myCaption()
{
	char	s[256];
	float	cscale=14., space=110., sspace=90., lm=80., um=wh-30.;

	glColor3f(r, g, b);
	glPushMatrix();
	glTranslatef(lm+space*0, um, 0.);
	glScalef(cscale, cscale, 1.);
	sprintf(s, "n=%d,", count);
	glfDrawSolidString(s);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lm+space*2, um, 0.);
	glScalef(cscale, cscale, 1.);
	sprintf(s, "w=%4.3f,", W);
	glfDrawSolidString(s);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(lm+space*4, um, 0.);
	glScalef(cscale, cscale, 1.);
	if(subdivision_mode==0) glfDrawSolidString("rbf4,");
	else if(subdivision_mode==1) glfDrawSolidString("rbf6,");
	else if(subdivision_mode==2) glfDrawSolidString("NR4,");
	else if(subdivision_mode==3) glfDrawSolidString("D-D,");
	else glfDrawSolidString("UNR4,");
	glPopMatrix();

}

void myTextCaption()
{
	printf("N=%3d,", count);
//	printf("I=%3d,", index);
//	printf("c=%4.2f,", C);
	printf("d=%1d,", poly_deg);
	printf("r=%4.2f,", R);
	printf("w=%4.3f,", W);
	if(base_mode==0) printf("mq1,");
	else if(base_mode==1)printf("mq3,");
	else printf("gs ,");
	if(subdivision_mode==0) printf("rbf4");
	else if(subdivision_mode==1) printf("rbf6");
	else if(subdivision_mode==2) printf("NR4 ");
	else if(subdivision_mode==3) printf("D-D ");
	else printf("UNR4");
//	if(station_mode) printf("st,");
//	else printf("ns,");
	printf("\n");
}

/* rehaping routine called whenever window is resized or moved */

void myReshape(GLsizei w, GLsizei h)
{
	/* adjust clipping box */
//	glClearColor (1.0, 1.0, 1.0, 1.0);
	glClearColor (0.85, 0.85, 0.85, 1.0);
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity(); 
	glOrtho(0.0, (GLdouble)w, 0.0, (GLdouble)h, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW); 
	glLoadIdentity(); 
	glLineWidth(1.5);
	ww = w;
	wh = h; 
}

void myMouse(int btn, int state, int x, int y)
{
	int i, j=-1;

	if(btn==GLUT_LEFT_BUTTON && state==GLUT_DOWN) {
		for(i=1; i<knotcount/2; i++) 
			if(TP[i][0]-width<x && x<TP[i][0]+width && TP[i][1]-width<y && y<TP[i][1]+width) j=i;
		for(i=knotcount-2; i>=knotcount/2; i--) 
			if(TP[i][0]-width<x && x<TP[i][0]+width && TP[i][1]-width<y && y<TP[i][1]+width) j=i;

		if(j!=-1) {
			kindex=j;
			TP[kindex][2] = 1;
			active = 2;
		}
		else {
			for(i=0; i<count; i++) 
				if(CP[i][0]-width<x && x<CP[i][0]+width && CP[i][1]-width<y && y<CP[i][1]+width) break;
			if(i<count) index=i;
			else {
				index=count;
				if(index>=MAX) index--;
				CP[index][0] = x;
				CP[index][1] = y;
				icount++;
				count++;
				myMakeKnotVectors();
				myRBFCoefficient();
			}
			active = 1;
			CP[index][2] = 1;
		}
		glutPostRedisplay();
	}
	else if(btn==GLUT_LEFT_BUTTON && state==GLUT_UP && active) 
	{
		if(active == 1) {
			CP[index][0] = iCP[index][0] = x;
			CP[index][1] = iCP[index][1] = y;
			CP[index][2] = 0;
		}
		else if(active == 2) {
			if(x<TP[kindex-1][0]) x=TP[kindex-1][0];
			if(x>TP[kindex+1][0]) x=TP[kindex+1][0];
			TP[kindex][0] = x;
			TP[kindex][2] = 0;
			myMakeTPKnotVectors();
		}
		active = 0;
//		myMakeKnotVectors();
		myRBFCoefficient();
		glutPostRedisplay();
	}
}

void myDeletePoint()
{
	if(index == -1 || count==0) return;
	for(int i=index; i<count-1; i++) {
		CP[i][0]=CP[i+1][0];
		CP[i][1]=CP[i+1][1];
		CP[i][2]=CP[i+1][2];
	}
	count--;
	myMakeKnotVectors();
	myRBFCoefficient();
	index=-1;
}

/* motionFunc - Mouse movement (with a button down) callback */

void myMotionFunc(int x, int y)
{
	if(active == 1) {
		CP[index][0] = iCP[index][0] = x;
		CP[index][1] = iCP[index][1] = y;
		myRBFCoefficient();
		glutPostRedisplay();
	}
	else if(active == 2) {
		if(x<TP[kindex-1][0]) x=TP[kindex-1][0];
		if(x>TP[kindex+1][0]) x=TP[kindex+1][0];
		TP[kindex][0] = x;
		myMakeTPKnotVectors();
		glutPostRedisplay();
	}
}

/* passiveMotionFunc - Mouse movement (with no button down) callback */

void myPassiveMotionFunc(int x, int y)
{
	int	i, j=-1;
	static int oindex=-1, okindex=-1;

	for(i=0; i<=count; i++) 
		if(CP[i][0]-width<x && x<CP[i][0]+width && CP[i][1]-width<y && y<CP[i][1]+width) break;
	if(i<=count) index=i;
	else index=-1;

	for(i=1; i<knotcount/2; i++) 
		if(TP[i][0]-width<x && x<TP[i][0]+width && TP[i][1]-width<y && y<TP[i][1]+width) j=i;
	for(i=knotcount-2; i>=knotcount/2; i--) 
		if(TP[i][0]-width<x && x<TP[i][0]+width && TP[i][1]-width<y && y<TP[i][1]+width) j=i;
	if(j != -1) {
		kindex=j;
		index=-1;
	}	
	else kindex=-1;

	if(oindex != index) {
		if(index<count) {
			if(oindex != -1)
				CP[oindex][2]=0;
			CP[index][2]=1;
		}
		glutPostRedisplay();
		oindex = index;
	}
	if(okindex != kindex) {
		if(kindex<knotcount) {
			if(okindex != -1)
				TP[okindex][2]=0;
			TP[kindex][2]=1;
		}
		glutPostRedisplay();
		okindex = kindex;
	}
}

void	myKnotVectors()
{
	int		i;
	double	x0=XMARGIN, x1=ww-XMARGIN, x, y=wh-YMARGIN;
	glBegin(GL_LINES);
		glVertex2d(x0, wh-y);
		glVertex2d(x1, wh-y);
	glEnd();
	if(display_mode == LAGRANGE || display_mode == BEZIER || display_mode == BSPLINE) {
		for(i=0; i<knotcount; i++) {
			if(TP[i][2]) myBox(TP[i][0], TP[i][1], GL_POLYGON);
			else myBox(TP[i][0], TP[i][1], GL_LINE_LOOP);
//			printf("%2.0f ", T[i]);
		}
//		printf("\n");
		if(tv>T[0] && tv<T[knotcount-1]) {
			x=(T[knotcount-1]-tv)/(T[knotcount-1]-T[0])*x0+
				(tv-T[0])/(T[knotcount-1]-T[0])*x1;
			myBox(x,y,GL_POLYGON);
		}
	}
}

void myMakeKnotVectors()
{
	int		i;
	double	x0=XMARGIN, x1=ww-XMARGIN, x, y=wh-YMARGIN;
	if(display_mode == LAGRANGE || display_mode == RBFINT) {
		knotcount=count;
		for(i=0; i<knotcount; i++) T[i]=i;
		for(i=0; i<knotcount; i++) {
			TP[i][0]=(T[knotcount-1]-T[i])/(T[knotcount-1]-T[0])*x0
				+(T[i]-T[0])/(T[knotcount-1]-T[0])*x1;
			TP[i][1]=y;
		}

	}		
	else if(display_mode == BEZIER) {
		knotcount=(count-1)*2;
		for(i=0; i<knotcount/2; i++) {
			T[i]=0; 
			T[knotcount-i-1]=1;
		}
		for(i=0; i<knotcount; i++) {
			TP[i][0]=(T[knotcount-1]-T[i])/(T[knotcount-1]-T[0])*x0
				+(T[i]-T[0])/(T[knotcount-1]-T[0])*x1;
			TP[i][1]=y;
		}
	}		
	else if(display_mode == BSPLINE) {
		knotcount=count+poly_deg+1;
		for(i=0; i<poly_deg; i++) {
			T[i]=0.; 
			T[knotcount-i-1]=count-poly_deg;
		}
		for(i=poly_deg; i<count+1; i++) {
			T[i]=i-poly_deg; 
		}
		for(i=0; i<knotcount; i++) {
			TP[i][0]=(T[knotcount-1]-T[i])/(T[knotcount-1]-T[0])*x0
				+(T[i]-T[0])/(T[knotcount-1]-T[0])*x1;
			TP[i][1]=y;
		}
	}		
}

void myMakeTPKnotVectors()
{
	int		i;
	for(i=0; i<knotcount; i++) {
		T[i]=1.-(double)(TP[knotcount-1][0]-TP[i][0])/(TP[knotcount-1][0]-TP[0][0]);
	}
}

void myMakeKnotTPVectors()
{
	int		i;

	double	x0=XMARGIN, x1=ww-XMARGIN, x, y=wh-YMARGIN;
	for(i=0; i<knotcount; i++) {
		TP[i][0]=(T[knotcount-1]-T[i])/(T[knotcount-1]-T[0])*x0
			+(T[i]-T[0])/(T[knotcount-1]-T[0])*x1;
		TP[i][1]=y;
	}
}
/*
void myKnotVectors()
{
	int		i;
	float	f=0.;

	printf("please enter %d increasing knots vectors : \n", count);
	for(i=0; i<count; i++) {
		scanf("%f", &f);
		if(i>0 && lT[i-1]>=f) {
			printf("Error : please enter increasing knot vectors\n"); 
			while(getchar()!='\n');
			return;
		}	
		lT[i]=f;
	}
	for(i=0; i<count; i++) T[i]=lT[i];
}
*/

void right_menu(int id)
{
	int	i,j;
	if(id == 1) {
		count=0;
		icount=0;
		deltaT=1.0;
		glutPostRedisplay();
	}	
	else if(id == 2) {
		display_mode = RBFINT;
		myMakeKnotVectors();
		myRBFCoefficient();
		glutPostRedisplay();
	}	
	else if(id == 3) {
		display_mode = FALSE;
		if(subdivision_mode==0 || subdivision_mode==1)
			myRBFSubdivision(order);
		else if(subdivision_mode==2)
			myNR4Subdivision();
		else if(subdivision_mode==3)
			myDDSubdivision();
		else 
			myUNR4Subdivision();
	}	
	else if(id == 4) {
		exit(1);
	}		  
	else if(id == 5) {
		if(icount>0) {
			R=1.0;
			count=icount;
			deltaT=1.0;
			for(i=0; i<count; i++) {
				for(j=0; j<dim; j++) CP[i][j]=iCP[i][j];
				T[i]=i*deltaT;
			}
			myRBFCoefficient();
			glutPostRedisplay();
		}
	}
	else if(id == 6) {
		display_mode=LAGRANGE;
		myMakeKnotVectors();
		glutPostRedisplay();
	}	
	else if(id == 7) {
		display_mode=BEZIER;
		myMakeKnotVectors();
		glutPostRedisplay();
	}
	else if(id == 8) {
		myDeletePoint();
		glutPostRedisplay();
	}
	else if(id == 9) {
		myKnotVectors();
		glutPostRedisplay();
	}
	else if(id == 10) {
		display_mode=BSPLINE;
		myMakeKnotVectors();
		glutPostRedisplay();
	}
	myTextCaption();
}

void myKeyboard(unsigned char theKey, int x, int y)
{
	switch (theKey)
	{
		case 'h' :	
			W-=deltaW;
			glutPostRedisplay();
			break;
		case 'j' :	
			W+=deltaW;
			glutPostRedisplay();
			break;
		case '<' :	
		case ',' :	
			R-=deltaR;
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case '>' :	
		case '.' :	
			R+=deltaR;
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case 'e' :	
			myBezierDegreeElevation();
			break;
		case 'i' :	
			myBSplineKnotInsertion();
			break;
/*
		case 'k' :	
			C-=deltaR;
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case 'l' :	
			C+=deltaR;
			myRBFCoefficient();
			glutPostRedisplay();
			break;
*/
		case 'q' :	
			poly_deg--;
			if(poly_deg<0) poly_deg=0;
			myMakeKnotVectors();
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case 'w' :	
			poly_deg++;
			myMakeKnotVectors();
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case 'b' :	
			base_mode++;
			if(base_mode==3) base_mode=0;
			myRBFCoefficient();
			glutPostRedisplay();
			break;
		case 'n' :	station_mode = !station_mode;
			glutPostRedisplay();
			break;
		case 'a' :	
			subdivision_mode++;
			if(subdivision_mode==5) subdivision_mode=0;
			if(subdivision_mode==0) order=4;
			else if(subdivision_mode==1) order=6;
			glutPostRedisplay();
			break;
		case 's':
			right_menu(3);
		break;
		case 'r':
			right_menu(2);
		break;
		case 'c':
			right_menu(1);
		break;
		case 'd':
			right_menu(5);
		break;
		case 'z':
			scale*=scaleDelta;
		break;
		case 'x':
			scale/=scaleDelta;
		break;
		case 'p':
			debug_mode=!debug_mode;
			if(debug_mode)
				fp=fopen("debug.txt", "wt");
			else
				fclose(fp);
		break;
		case ' ':
		if(display_mode==LAGRANGE || display_mode==BEZIER 
			|| display_mode==BSPLINE) {
			idle_mode=!idle_mode;
			if(idle_mode) 
				glutIdleFunc(myIdleFunc);
			else
				glutIdleFunc(NULL);
		}
		break;
		case '?':
			myIntoduction();
		break;
		case 27:	exit(-1);
	}
	myTextCaption();
}

void mySKeyboard (int key, int x, int y)
{
    switch (key) {
		case GLUT_KEY_UP : ypos+=10.;break;
		case GLUT_KEY_DOWN : ypos-=10.;break;
		case GLUT_KEY_LEFT : xpos-=10.;break;
		case GLUT_KEY_RIGHT : xpos+=10.; break;
		default : return;
    }
	glutPostRedisplay();
}

void myIdleFunc ()
{
	static int x=0;
	idletv=(double)(x++)/MAX;
	if(x==MAX+1) {
		idletv=-1;
		x=0;
		idle_mode=FALSE;
		glutIdleFunc(NULL);
	}	
	glutPostRedisplay();
}

void main(int argc, char** argv) {
	glutInit(&argc,argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(ww, wh);
	glutCreateWindow("Curves in Graphics Programming by lbg@dongseo.ac.kr Aug. 2003.");
	glfInit();
	glfLoadFont("fonts/garamond1.glf");
	glutReshapeFunc(myReshape); 
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotionFunc);
	glutKeyboardFunc(myKeyboard);
	glutPassiveMotionFunc(myPassiveMotionFunc);
	glutCreateMenu(right_menu);
	glutAddMenuEntry("clear",1);
	glutAddMenuEntry("reload",5);
	glutAddMenuEntry("RBF",2);
	glutAddMenuEntry("bezier",7);
	glutAddMenuEntry("lagrange",6);
	glutAddMenuEntry("B-spline",10);
	glutAddMenuEntry("subdivision",3);
	glutAddMenuEntry("delete point",8);
	glutAddMenuEntry("quit",4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	myIntoduction();
	glutMainLoop();
}

void myIntoduction()
{
	printf("Curves in Graphics Programming by lbg@dongseo.ac.kr Aug. 2003.\n");
	printf("b : select basis (multiquadrics, gaussian)\n");
	printf("    mq1 (x^2+r^2)^(1/2)\n");
	printf("    mq3 (x^2+r^2)^(3/2)\n");
	printf("    gs  exp(-r*x^2)\n");
	printf("<>: decrease, increase r values in the basis function\n");
	printf("hj: decrease, increase w values in the NR4 subdivision scheme\n");
	printf("qw: decrease, increase polynomial degree in the RBF\n");
	printf("s : excute subdivision schemes\n");
	printf("a : select schemes (RBF4, RBF6, NR4, D-D, UNR4)\n");
	printf("    NR4  Nira Dyn 4point subdivision scheme\n");
	printf("    D-D  Deslauries and Dubuc 4point subdivision scheme\n");
	printf("    rbf4 Radial Basis Function 4point subdivision scheme\n");
	printf("    rbf6 Radial Basis Function 6point subdivision scheme\n");
//	printf("n : non-station scheme in RBF, ns:non-station,  st:station\n");
//	printf("p : debug mode\n");
	printf("\n");
}