# Matrix Library

This is a generic header-only library for doing mathimatical operations
on arbtrarally sized vectors and matrices.

Use `make` to build or  `make run`  to build and run tests

## Features

* Matrices
	* Addition
	* Subtraction
	* Matrix Multiplication
	* Inversion
	* Transpositon
	* Division (using inversion) 

* Vectors
	* Addition
	* Subtraction
	* Dot products 
	* Cross products
	* Normalization

* Cross-type
	* Matrix-vector Multiplication
	* Matrix-scalar operations (\* , / , \*= , /=)
	* Vector-scalar operations (\* , / , \*= , /=)
	* Raising a matrix to the power of an integer

## Implementation
	
This library doesn't heap-allocate which makes it optimal for
applications requiring small vectors and matrices like graphics.
This requires that the size of matrices are known at compile 
time.  The number of rows and columns are passed as template
arguments along with the type.

```c++
matrix<float,3,2> m; //three columns and two rows
```

This architecture makes it possible to turn a large number of invalid
operations into compile-time errors.

For example the inverse function only accepts one integer as 
a template parameter.  Thus, it will only accept a square matrix.

```c++
template<typename T,int n> 
matrix<T,n,n> inverse(matrix<T,n,n> x);

inverse(matrix<float,2,3>()); //compile-time error
inverse(matrix<float,3,3>()); //works
```

Matrix multiplication requires that the shape of the 
second matrix be the reverse of the first's shape

```c++
template<typename T,int r,int c> 
matrix<T,r,r> operator*(matrix<T,r,c> x,matrix<T,c,r> y);

auto c=matrix<float,3,4>() * matrix<float,3,2>(); //compile-time error
auto c=matrix<float,3,4>() * matrix<float,4,3>(); //works
```

Template parameters can also be used to constrain operations
to a certain size.  For example, the cross product is only
valid between two 3 dimensional vectors

```c++
template<typename T>
vec<T,3> cross(vec<T,3> a,vec<T,3> b);

cross(vec<float,4>,vec<float,4>); //compile-time error
cross(vec<float,3>,vec<float,3>); //works 

``` 

Concepts are also used to contrain what types can be passed to a
template.  For example, calculating the magnitude of a vector
requires a square root which only makes sense for floating point
vectors 

```c++
template<std::floating_point T,int n>
T length(vec<T,n> a){
	if constexpr (std::same_as<T,float>) return sqrtf(dot(a,a));
	else return sqrt(dot(a,a));
}

length(vec<int,3>()); //compile-time error
length(vec<double,3>()); //works
```

## Contructors 

```c++
matrix<float,2,2> m; //identity matrix
//	1	0
//	0	1

matrix<float,2,2> m(2); //uniform scaling matrix
//	2	0
//	0	2

matrix<float,2,2> m(vec<float,3>(a,b)); //non-niform scaling matrix
//	a	0
//	0	b

matrix<float,2,2> m(vec<float,3>(7,2) , vec<float,3>(4,5)); //set the columns
//	7	4
//	2	5

vec<float,2> v; //v=[0,0]

vec<float,2> v(6); //v=[6,6]

vec<float,2> v(3,4); //v=[3,4]

```



















//
