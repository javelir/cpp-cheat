#include <stdlib.h>
#include <f2c.h>
#include <clapack.h>
#include <cblas.h>
#include <math.h>
#include <stdio.h>

void print_vecd(double * v, int n)
{
    int i;
    for(i=0; i<n; i++){
        fprintf(stderr, "%.2f ",v[i]);
    }
    printf("\n");
}

int vec_eqd(double * v1, double * v2, int n)
{
    int i;
    for(i=0; i<n; i++){
        if( v1[i] != v2[i] )
            return 0;
    }
    return 1;
}

void assert_eq_vecd(double * v1, double * v2, int n){
    if( vec_eqd(v1,v2,n) == 0 ){
        print_vecd(v1,n);
        print_vecd(v2,n);
    }
}

/**
* assert two integers are equal
* if not, print them to stderr
*/
void assert_eqi(int i1, int i2){
    if( i1 != i2 ){
        fprintf(stderr, "%d\n%d\n\n\n", i1, i2);
    }
}

//globals
char transa;
char transb;

int N;
int nrhs;
int lda;
int ldb;
int ldc;
int ipiv2[2];
int ldb;
int info;
int m;
int ntrf;
int k;

double alpha;
double beta;

double A4[4];
double B4[4];
double C4[4];
double b2[2];
double b4[4];
double c2[2];
double c4[4];
double D4[4];

int main(void)
{
  //SV solve system
    //1 2 = 5
    //3 4   11
    N = 2;
    nrhs = 1; //number of columns of b: if many solves several eqs with same A
    A4[0]=1.;
    A4[1]=3.;
    A4[2]=2.;
    A4[3]=4.;
    lda = 2; //max N,1. Leading Dimenstion A
    //ipv2: see TRF
    b2[0] = 5.;
    b2[1] = 11.;
    ldb = 2; //max 1,N
    dgesv_(&N, &nrhs, A4, &lda, ipiv2, b2, &ldb, &info);
    //result returned inside of b

    c2[0] = 1.;
    c2[1] = 2.;
    assert_eqi(info,0);
    assert_eq_vecd(b2,c2,N);

    //singular system error
      A4[0]=1.;
      A4[1]=2.;
      A4[2]=2.;
      A4[3]=4.;
      dgesv_(&N, &nrhs, A4, &lda, ipiv2, b2, &ldb, &info);

      assert_eqi(info,2);

    //nrhs TODO
      //1 2 = 5  11
      //3 4   11 25
      nrhs = 2;
      b4[0]=5.;
      b4[1]=11.;
      b4[2]=11.;
      b4[3]=25.;
      dgesv_(&N, &nrhs, A4, &lda, ipiv2, b4, &ldb, &info);
      //result returned inside of b

      c4[0] = 1.;
      c4[1] = 2.;
      c4[2] = 1.;
      c4[3] = 2.;
      /*assert_eqi(info,0);*/
      /*assert_eq_vecd(b4,c4,N);*/

  //MM matrix multiplication
      //alpha*op( A )*op( B ) + beta*C,
      //0 1 * 1 2 = 3 4
      //2 3   3 4   11 16
      transa = 'n'; //op A: n none
      transb = 'n'; //op A: n none
      m = 2; //rows of A
      n = 2; //cols of B
      k = 2; //cols of op(A)
      alpha = 1.;
      A4[0]=0.;
      A4[1]=2.;
      A4[2]=1.;
      A4[3]=3.;
      lda = 2; //max N,1. Leading Dimenstion A
      B4[0]=1.;
      B4[1]=3.;
      B4[2]=2.;
      B4[3]=4.;
      ldb = 2; //dim of b
      beta = 0.;
      ldc = 2; //dim of c

      dgemm_( &transa, &transb, &m, &n, &k, &alpha, A4, &lda,
                            B4, &ldb, &beta, C4, &ldc );
      //result returned in C

      D4[0] = 3.;
      D4[1] = 11.;
      D4[2] = 4.;
      D4[3] = 16.;
      assert_eq_vecd(C4,D4,lda);

  //TRI matrix inverse
 
  //TRF get the LU factorization
  
  //eigenvalues and eigenvectors
      //for non symmetric, several steps are needed
      //http://www.alglib.net/eigen/nonsymmetric/nonsymmetricevd.php
}