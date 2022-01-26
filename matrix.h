#pragma once
#include <iostream>
#include <assert.h>
#include <string.h>
using std::cout,std::ostream;
#include <concepts>
using std::floating_point,std::same_as,std::integral;

template<typename T>
concept numeric=floating_point<T> || integral<T>;

template<numeric T,int n>
struct vec{
	static_assert(n>0);
	T data[n];
	vec(){for(int i=0;i<n;i++) data[i]=0;}
	vec(T x){for(int i=0;i<n;i++) data[i]=x;}
	vec(T x,T y){
		static_assert(n==2);
		data[0]=x,data[1]=y;
	}
	vec(T x,T y,T z){
		static_assert(n==3);
		data[0]=x,data[1]=y,data[2]=z;
	}
	vec(T x,T y,T z,T w){
		static_assert(n==4);
		data[0]=x,data[1]=y,data[2]=z,data[3]=w;
	}
	T &operator[](int i){return data[i];}
};

#define ELEMENTWISE(op,mod_op,ref)								\
template<typename T,int n>										\
vec<T,n> operator op(vec<T,n> ref a,vec<T,n> b){				\
	for(int i=0;i<n;i++){										\
		a[i] mod_op b[i];										\
	}															\
	return a;													\
}

//define element-wise operators
ELEMENTWISE(+ ,+=, );
ELEMENTWISE(- ,-=, );
//define modifying element-wise operators
ELEMENTWISE(+=,+=,&);
ELEMENTWISE(-=,-=,&);

#undef ELEMENTWISE

#define SCALAR_ELEMENTWISE(op,mod_op,ref)			\
template<typename T,int n>							\
vec<T,n> operator op(vec<T,n> ref a,T x){			\
	for(int i=0;i<n;i++){							\
		a[i] mod_op x;								\
	}												\
	return a;										\
}

//define element-wise operators
SCALAR_ELEMENTWISE(* ,*=, );
SCALAR_ELEMENTWISE(/ ,/=, );
//define modifying element-wise operators
SCALAR_ELEMENTWISE(*=,*=,&);
SCALAR_ELEMENTWISE(/=,/=,&);

#undef SCALAR_ELEMENTWISE


//for scalar first, vector second
#define SCALAR_ELEMENTWISE(op)						\
template<typename T,int n>							\
vec<T,n> operator op(T x,vec<T,n> a){				\
	for(int i=0;i<n;i++){							\
		a[i]=x op a[i];								\
	}												\
	return a;										\
}



//define element-wise operators
SCALAR_ELEMENTWISE(*);
SCALAR_ELEMENTWISE(/);

#undef SCALAR_ELEMENTWISE

template<typename T,int n>
T dot(vec<T,n> a,vec<T,n> b){
	T x=0;
	for(int i=0;i<n;i++) x+=a[i]*b[i];
	return x;
}

template<typename T>
vec<T,3> cross(vec<T,3> a,vec<T,3> b){
	vec<T,3> c;
	for(int i=0;i<3;i++){
		c[i]+=a[(i+1)%3]*b[(i+2)%3];
		c[i]-=a[(i-1)%3]*b[(i-2)%3];
	}
	return c;
}

template<floating_point T,int n>
T length(vec<T,n> a){
	if constexpr (same_as<T,float>) return sqrtf(dot(a,a));
	else return sqrt(dot(a,a));
}

template<floating_point T,int n>
vec<T,n> normalize(vec<T,n> a){
	return a/length(a);
}

template<typename T,int n> 
bool operator==(vec<T,n> x,vec<T,n> y){
	for(int i=0;i<n;i++){ 
		if(x[i]!=y[i]) return false;
	}
	return true;
}

template<typename T,int n>
ostream &operator<<(ostream &os,vec<T,n> v){
	os<<"[";
	for(int i=0;i<n;i++){
		os<<v[i];
		if(i<n-1) cout<<", ";
	}
	os<<"]\n";
	return os;
}





template<numeric T,int r,int c>
struct matrix{
	static_assert(r>0 && c>0);
	T data[r][c];
	matrix(){
		if(r==c){
			for(int i=0;i<r;i++){
				for(int j=0;j<c;j++){
					data[i][j]=(i==j)?1:0;
				}
			}
		}
	}
	matrix(T x){
		static_assert(c==r);
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				data[i][j]=(i==j)?x:0;
			}
		}
	}
	matrix(vec<T,r> v){
		static_assert(c==r);
		for(int i=0;i<r;i++){
			for(int j=0;j<c;j++){
				data[i][j]=(i==j)?v[i]:0;
			}
		}
	}
	matrix(vec<T,c> x,vec<T,c> y){
		static_assert(r==2);
		memcpy(data[0],x.data,c*sizeof(T));
		memcpy(data[1],y.data,c*sizeof(T));
	}
	matrix(vec<T,c> x,vec<T,c> y,vec<T,c> z){
		static_assert(r==3);
		memcpy(data[0],x.data,c*sizeof(T));
		memcpy(data[1],y.data,c*sizeof(T));
		memcpy(data[2],z.data,c*sizeof(T));
	}
	matrix(vec<T,c> x,vec<T,c> y,vec<T,c> z,vec<T,c> w){
		static_assert(r==4);
		memcpy(data[0],x.data,c*sizeof(T));
		memcpy(data[1],y.data,c*sizeof(T));
		memcpy(data[2],z.data,c*sizeof(T));
		memcpy(data[3],w.data,c*sizeof(T));
	}
	T *operator[](int i){
		assert(i<r);
		return data[i];
	}
};


//create nicer names for small matrices
typedef matrix<float,1,1> float1x1;
typedef matrix<float,1,2> float1x2;
typedef matrix<float,1,3> float1x3;
typedef matrix<float,1,4> float1x4;

typedef matrix<float,2,1> float2x1;
typedef matrix<float,2,2> float2x2;
typedef matrix<float,2,3> float2x3;
typedef matrix<float,2,4> float2x4;

typedef matrix<float,3,1> float3x1;
typedef matrix<float,3,2> float3x2;
typedef matrix<float,3,3> float3x3;
typedef matrix<float,3,4> float3x4;

typedef matrix<float,4,1> float4x1;
typedef matrix<float,4,2> float4x2;
typedef matrix<float,4,3> float4x3;
typedef matrix<float,4,4> float4x4;


template<typename T,int r,int c>
ostream &operator<<(ostream &os,matrix<T,r,c> m){
	os<<"[\n";
	for(int i=0;i<r;i++){
		os<<"\t[";
		for(int j=0;j<c;j++){
			os<<m.data[i][j];
			if(j<c-1) os<<", ";
		}
		os<<"]";
		if(i<r-1) cout<<",";
		os<<"\n";
	}
	os<<"]\n";
	return os;
}

#define ELEMENTWISE(op,mod_op,ref)								\
template<typename T,int r,int c>								\
matrix<T,r,c> operator op(matrix<T,r,c> ref a,matrix<T,r,c> b){	\
	for(int i=0;i<r;i++){										\
		for(int j=0;j<c;j++){									\
			a[i][j] mod_op b[i][j];								\
		}														\
	}															\
	return a;													\
}

//define element-wise operators
ELEMENTWISE(+ ,+=, );
ELEMENTWISE(- ,-=, );
//define modifying element-wise operators
ELEMENTWISE(+=,+=,&);
ELEMENTWISE(-=,-=,&);

#undef ELEMENTWISE



template<typename T,int r,int c> 
matrix<T,r,r> operator*(matrix<T,r,c> x,matrix<T,c,r> y){ 
	matrix<T,r,r> z;
	for(int i=0;i<r;i++){ 
		for(int j=0;j<r;j++){ 
			z[i][j]=0;
			for(int k=0;k<c;k++){ 
				z[i][j]+=x[i][k]*y[k][j]; 
			}
		} 
	} 
	return z; 
}

template<typename T,int n> 
matrix<T,n,n> operator*=(matrix<T,n,n> &x,matrix<T,n,n> y){
	return x=x*y;
}

template<typename T,int r,int c> 
matrix<T,c,r> transpose(matrix<T,r,c> x){
	matrix<T,c,r> t;
	for(int i=0;i<r;i++){ 
		for(int j=0;j<c;j++){ 
			t[j][i]=x[i][j];
		}
	}
	return t;
}


template<typename T,int n> 
matrix<T,n,n> inverse(matrix<T,n,n> x){
	//initialize data structure of 2 matrices side by side
	//the first is x and the second is the identity matrix
	T data_raw[n][2*n];
	T *data[n];
	for(int i=0;i<n;i++) data[i]=&data_raw[i][0];
	for(int i=0;i<n;i++){ 
		for(int j=0;j<2*n;j++){
			if(j<n) data[i][j]=x[i][j];
			else data[i][j]=(i==j-n)?1:0;
		}
	}
	//make the first matrix diagonal
	for(int i=0;i<n;i++){
        if(data[i][i]==0){
            bool b=false;
            for(int j=i+1;j<n;j++){
                if(data[j][i]!=0){
                    T *tmp=data[i];
                    data[i]=data[j];
                    data[j]=tmp;
                    b=true;
                    break;
                }
            }
            if(!b){
            	cout<<"uninvertible\n";
            	exit(1);
            }
        }
        for(int j=i+1;j<n;j++){
        	T coef=-data[j][i]/data[i][i];
            for(int k=0;k<2*n;k++) data[j][k]+=coef*data[i][k];
		}
	}
	//make the first matrix the identity matrix
	for(int i=n-1;i>0;i--){
        if(data[i][i]==0){
            bool b=false;
            for(int j=n-1;j>i;j--){
                if(data[j][i]!=0){
                    T *tmp=data[i];
                    data[i]=data[j];
                    data[j]=tmp;
                    b=true;
                    break;
                }
            }
            if(!b){
            	cout<<"uninvertible\n";
            	exit(1);
            }
        }
        for(int j=i-1;j>=0;j--){
            T coef=-data[j][i]/data[i][i];
            for(int k=0;k<2*n;k++) data[j][k]+=coef*data[i][k];
        }
    }
    for(int i=0;i<n;i++){
		for(int j=2*n-1;j>=n;j--){
            data[i][j]/=data[i][i];
		}
    }
    //the second matrix is now the inverse(x)
    //copy the second matrix into inv and return it
	matrix<T,n,n> inv;
	for(int i=0;i<n;i++){ 
		for(int j=0;j<n;j++){
			inv[i][j]=data[i][j+n];
		}
	} 
	return inv;
}

template<typename T,int n> 
matrix<T,n,n> operator/(matrix<T,n,n> x,matrix<T,n,n> y){
	return x*inverse(y);
}

template<typename T,int n> 
matrix<T,n,n> operator/=(matrix<T,n,n> &x,matrix<T,n,n> y){
	return x*=inverse(y);
}

template<typename T,int n>
matrix<T,n,n> pow(matrix<T,n,n> m,int p){
	matrix<T,n,n> x;
	if(p>=0){
		for(int i=0;i<p;i++) x*=m;
		return x;
	}
	p=abs(p);
	auto m_inv=inverse(m);
	for(int i=0;i<p;i++) x*=m_inv;
	return x;
}

template<typename T,int r,int c> 
bool operator==(matrix<T,r,c> x,matrix<T,r,c> y){
	for(int i=0;i<r;i++){ 
		for(int j=0;j<c;j++){ 
			if(x[i][j]!=y[i][j]) return false;
		}
	}
	return true;
}


//for matrix first, scalar second
#define SCALAR_ELEMENTWISE(op,mod_op,ref)			\
template<typename T,int r,int c>					\
matrix<T,r,c> operator op(matrix<T,r,c> ref a,T x){	\
	for(int i=0;i<r;i++){							\
		for(int j=0;j<c;j++){						\
			a[i][j] mod_op x;						\
		}											\
	}												\
	return a;										\
}

//define element-wise operators
SCALAR_ELEMENTWISE(* ,*=, );
SCALAR_ELEMENTWISE(/ ,/=, );
//define modifying element-wise operators
SCALAR_ELEMENTWISE(*=,*=,&);
SCALAR_ELEMENTWISE(/=,/=,&);

#undef SCALAR_ELEMENTWISE


//for scalar first, matrix second
#define SCALAR_ELEMENTWISE(op)					\
template<typename T,int r,int c>				\
matrix<T,r,c> operator op(T x,matrix<T,r,c> a){	\
	for(int i=0;i<r;i++){						\
		for(int j=0;j<c;j++){					\
			a[i][j]=x op a[i][j];				\
		}										\
	}											\
	return a;									\
}

//define element-wise operators
SCALAR_ELEMENTWISE(*);
SCALAR_ELEMENTWISE(/);

#undef SCALAR_ELEMENTWISE


//matrix-vector multiplication
template<typename T,int r,int c>
vec<T,r> operator*(matrix<T,r,c> m,vec<T,c> v){
	vec<T,r> u;
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			u[i]+=v[j]*m[i][j];
		}
	}
	return u;
}

template<typename T,int r,int c>
vec<T,c> operator*(vec<T,r> v,matrix<T,r,c> m){
	vec<T,c> u;
	for(int i=0;i<r;i++){
		for(int j=0;j<c;j++){
			u[j]+=v[i]*m[i][j];
		}
	}
	return u;
}



//create nicer names for small vectors
typedef vec<float,2> float2;
typedef vec<float,3> float3;
typedef vec<float,4> float4;














//