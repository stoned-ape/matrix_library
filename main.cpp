#include "matrix.h"
#include <assert.h>
#include <math.h>
#include <complex>
using namespace std;



template<typename T,int r,int c>
bool is_equal(matrix<T,r,c> a,matrix<T,r,c> b,T &absdiff){
	absdiff=0;
	for(int i=0;i<r;i++){ 
		for(int j=0;j<c;j++){
			absdiff+=abs(a[i][j]-b[i][j]);
		}
	}
	return absdiff<1e-5;
}

template<typename T,int r,int c>
matrix<T,r,c> rand_mat(){
	matrix<T,r,c> m;
	for(int i=0;i<r;i++){ 
		for(int j=0;j<c;j++){
			m[i][j]=2*(rand()/(T)RAND_MAX)-1;
		}
	}
	return m;
}

template<typename T,int n>
vec<T,n> rand_vec(){
	vec<T,n> m;
	for(int i=0;i<n;i++){ 
		m[i]=2*(rand()/(T)RAND_MAX)-1;
	}
	return m;
}


template<typename T,int n>
void mul_test(){
	printf("testing multipliction for %d by %d:\n",n,n);
	int ntests=0,nfails=0;
	const matrix<T,n,n> id;
	for(int i=0;i<20;i++){
		auto r=rand_mat<T,n,n>();
		T diff;
		if(!is_equal(r,r*id,diff)){
			cout<<"multiplicative identity error("<<diff<<"): r = "<<r;
			cout<<"r*id = "<<r*id;
			nfails++;
		}
		ntests++;
	}
	printf("failed %d tests out of %d\n",nfails,ntests);

}

template<typename T,int n>
void inv_test(){
	printf("testing inverse for %d by %d:\n",n,n);
	int ntests=0,nfails=0;
	const matrix<T,n,n> id;
	for(int i=0;i<20;i++){
		auto r=rand_mat<T,n,n>();
		T diff;

		if(!is_equal(id,r*inverse(r),diff)){
			cout<<"inverse error("<<diff<<"): r = "<<r;
			cout<<"inverse(r) = "<<inverse(r);
			nfails++;
		}
		ntests++;
	}
	printf("failed %d tests out of %d\n",nfails,ntests);

}

template<typename T,int n>
void pow_test(){
	printf("testing exponentals for %d by %d:\n",n,n);
	int ntests=0,nfails=0;
	const matrix<T,n,n> id;
	for(int i=0;i<5;i++){
		auto r=rand_mat<T,n,n>();
		T diff;
		auto p1=pow(r,i);
		auto p2=pow(r,-i);
		if(!is_equal(id,p1*p2,diff)){
			cout<<"exponental error("<<diff<<"): r = "<<r;
			cout<<"pow(r,"<<i<<")*pow(r,-"<<i<<") = "<<p1*p2;
			nfails++;
		}
		ntests++;
	}
	printf("failed %d tests out of %d\n",nfails,ntests);

}

void scalar_test(){
	puts("testing matrix-scalar operations:");
	auto rnd=[]()->float{
		return rand()/(float)RAND_MAX;
	};
	float x=rnd();

	//matrix scalar operations
	float1x1 m(rnd());
	#define OP_TEST(op) assert((m op x)[0][0]==m[0][0] op x) 
	OP_TEST(*);
	OP_TEST(/);
	#undef OP_TEST
	#define OP_TEST(op) assert((x op m)[0][0]==x op m[0][0]) 
	OP_TEST(*);
	OP_TEST(/);
	#undef OP_TEST
	#define OP_TEST(mod_op){		\
		auto tmp1=m;				\
		auto tmp2=m[0][0];			\
		tmp1 mod_op x;				\
		tmp2 mod_op x;				\
		assert(tmp1[0][0]==tmp2);	\
	} 
	OP_TEST(*=);
	OP_TEST(/=);
	#undef OP_TEST

	//vector scalar operations
	float2 v(rnd());
	#define OP_TEST(op) assert((v op x)[0]==v[0] op x) 
	OP_TEST(*);
	OP_TEST(/);
	#undef OP_TEST
	#define OP_TEST(op) assert((x op v)[0]==x op v[0]) 
	OP_TEST(*);
	OP_TEST(/);
	#undef OP_TEST
	#define OP_TEST(mod_op){		\
		auto tmp1=v;				\
		auto tmp2=v[0];				\
		tmp1 mod_op x;				\
		tmp2 mod_op x;				\
		assert(tmp1[0]==tmp2);		\
	} 
	OP_TEST(*=);
	OP_TEST(/=);
	#undef OP_TEST
	puts("all tests passed");
}

void constructor_test(){
	puts("testing constructors:");
	{
		float3x3 m1;
		float3x3 m2(1);
		assert(m1==m2);
		float3x3 m3=rand_mat<float,3,3>();
		m1=m3;
		assert(m1==m3);
	}{
		float2 v22;
		v22[0]=1;v22[1]=2;
		float2x2 m22(v22);
		assert(m22[0][0]==1);
		assert(m22[1][1]==2);
	}{
		float3 v1;
		float3 v2(0);
		assert(v1==v2);
		float3 v3=rand_vec<float,3>();
		v1=v3;
		assert(v1==v3);
		assert(v1!=v2);
	}{
		float2 v2(1,2);
		assert(v2[0]==1 && v2[1]==2);
		float3 v3(1,2,3);
		assert(v3[0]==1 && v3[1]==2 && v3[2]==3);
		float4 v4(1,2,3,4);
		assert(v4[0]==1 && v4[1]==2 && v4[2]==3 && v4[3]==4);

		v2=float2(5,6);
		assert(v2[0]==5 && v2[1]==6);
		v3=float3(5,6,7);
		assert(v3[0]==5 && v3[1]==6 && v3[2]==7);
		v4=float4(5,6,7,8);
		assert(v4[0]==5 && v4[1]==6 && v4[2]==7 && v4[3]==8);
	}{
		float2x2 m22(float2(1,2),float2(3,4));
		assert(m22[0][0]==1 && m22[0][1]==2 && m22[1][0]==3 && m22[1][1]==4);
	}
	puts("all tests passed");

}


void mat_vec_test(){
	puts("testing matrix-vector multipliction:");
	float3x3 m33;
	m33[0][0]=1,m33[0][1]=2,m33[0][2]=7;
	m33[1][0]=3,m33[1][1]=4,m33[1][2]=8;
	m33[2][0]=5,m33[2][1]=6,m33[2][2]=9;
	float2x3 m23;
	m23[0][0]=1,m23[0][1]=2,m23[0][2]=3;
	m23[1][0]=4,m23[1][1]=5,m23[1][2]=6;
	float3x2 m32;
	m32[0][0]=1,m32[0][1]=2;
	m32[1][0]=3,m32[1][1]=4;
	m32[2][0]=5,m32[2][1]=6;
	float3 v3;
	v3[0]=1,v3[1]=2,v3[2]=3;
	float2 v2;
	v2[0]=1,v2[1]=2;
	//matrix*vector
	{
		cout<<"m = "<<m33;
		cout<<"v = "<<v3;
		float3 u=m33*v3;
		cout<<"m*v = "<<u;
		assert(u[0]==26);
		assert(u[1]==35);
		assert(u[2]==44);
	}{
		cout<<"m = "<<m23;
		cout<<"v = "<<v3;
		float2 u=m23*v3;
		cout<<"m*v = "<<u;
		assert(u[0]==14);
		assert(u[1]==32);
	}{
		cout<<"m = "<<m32;
		cout<<"v = "<<v2;
		float3 u=m32*v2;
		cout<<"m*v = "<<u;
		assert(u[0]==5);
		assert(u[1]==11);
		assert(u[2]==17);
	}
	//vector*matrix
	{
		cout<<"m = "<<m33;
		cout<<"v = "<<v3;
		float3 u=v3*m33;
		cout<<"v*m = "<<u;
		assert(u[0]==22);
		assert(u[1]==28);
		assert(u[2]==50);
	}{
		cout<<"m = "<<m23;
		cout<<"v = "<<v2;
		float3 u=v2*m23;
		cout<<"v*m = "<<u;
		assert(u[0]==9);
		assert(u[1]==12);
		assert(u[2]==15);
	}{
		cout<<"m = "<<m32;
		cout<<"v = "<<v3;
		float2 u=v3*m32;
		cout<<"v*m = "<<u;
		assert(u[0]==22);
		assert(u[1]==28);
	}
	puts("all tests passed");
}

void concept_test(){
	auto v0=vec<float,1>();
	auto v1=vec<double,1>();
	auto v2=vec<int,1>();
	auto v3=vec<long,1>();
	auto v4=vec<unsigned int,1>();
	auto v5=vec<unsigned long,1>();
	//auto v6=vec<complex<float>,1>();

	(void)v0;
	(void)v1;
	(void)v2;
	(void)v3;
	(void)v4;
	(void)v5;


}


int main(){
	srand(time(0));
	float2x3 m1;
	m1[0][0]=1,m1[0][1]=2,m1[0][2]=3;
	m1[1][0]=4,m1[1][1]=5,m1[1][2]=6;
	float3x2 m2;
	m2[0][0]=1,m2[0][1]=2;
	m2[1][0]=3,m2[1][1]=4;
	m2[2][0]=5,m2[2][1]=6;
	cout<<"m1 = "<<m1;
	cout<<"m2 = "<<m2;
	cout<<"m1*m2 = ";
	cout<<m1*m2;
	cout<<"m2*m1 = ";
	cout<<m2*m1;
	cout<<"transpose(m1) = "<<transpose(m1);

	float2x2 x(2);
	cout<<"x = "<<x<<"inverse(x) = "<<inverse(x);

	
	mul_test<double,2>();
	mul_test<double,3>();
	mul_test<double,4>();
	mul_test<double,5>();
	puts("");
	inv_test<double,2>();
	inv_test<double,3>();
	inv_test<double,4>();
	inv_test<double,5>();
	puts("");
	pow_test<double,2>();
	pow_test<double,3>();
	pow_test<double,4>();
	pow_test<double,5>();
	puts("");
	scalar_test();
	puts("");
	constructor_test();
	puts("");
	mat_vec_test();
	concept_test();

}























//