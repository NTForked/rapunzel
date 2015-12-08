/*------------------------------------------------------------------------------
/                             Proiect de diplomă
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 CUDA Modules
/-------------------------------------------------------------------------------
*/

#define M_PI	3.14159265358979323846
#include "HeadersAndDefines.h"
#include "Hair.h"

__device__ float delta;
__device__ Vector3D *dev_copy;

// referenceVertex is base hair.
__device__ float collisionOffset( Vector3D vertex, Vector3D referenceVertex, Vector3D centerOfGravity ){
	return referenceVertex.Distance( centerOfGravity ) - vertex.Distance( centerOfGravity );
}

__device__ float collisionOffsetAfterFall( Vector3D vertex, Vector3D referenceVertex, Vector3D centerOfGravity, float fallSpeed ){
	return referenceVertex.Distance( centerOfGravity ) - ( vertex - Vector3D( 0.0f, fallSpeed, 0.0f ) ).Distance( centerOfGravity );
}

__device__ float collisionOffsetAfterWind( Vector3D vertex, Vector3D referenceVertex, Vector3D centerOfGravity, Vector3D windDirection, float windSpeed ){
	return referenceVertex.Distance( centerOfGravity ) - ( vertex + windDirection * windSpeed ).Distance( centerOfGravity );
}

__device__ void restoreFromCollision( Vector3D &vertex, Vector3D previousVertex, Vector3D referenceVertex, Vector3D centerOfGravity ){
	// Declarations.
	float collisionOffset_v;
	
	// Collision with mannequin.
	collisionOffset_v = collisionOffset( vertex, referenceVertex, centerOfGravity );
	if( collisionOffset_v > 0 ){
		vertex += ( vertex - centerOfGravity ).normalize() * ( collisionOffset_v + RESTORATION_OFFSET );
	}

	// If real inter-node hair length is surpasses than permitted length
	if( vertex.Distance( previousVertex ) > HAIR_STRAND_LENGTH * HAIR_ELASTICITY_FACTOR ){
		// Restore it to normal length.
		vertex = previousVertex + ( vertex - previousVertex ).normalize() * HAIR_STRAND_LENGTH;
	}
}

__device__ void fall( Vector3D &vertex, Vector3D copy_vertex, Vector3D referenceVertex, Vector3D upsizedCenterOfGravity, Vector3D downsizedCenterOfGravity, float fallSpeed ){
	// If it's standing up	
	if( vertex.y > referenceVertex.y ){
		// Get it away from the mannequin.
		vertex += ( copy_vertex - upsizedCenterOfGravity ).normalize() * fallSpeed;
	}
	// If it's hanging down
	else{
		// Bring it closer to the mannequin.
		vertex += ( downsizedCenterOfGravity - copy_vertex ).normalize() * fallSpeed;
	}
	vertex.y -= fallSpeed;
}

__device__ void negativeGravity( Vector3D &vertex, Vector3D copy_vertex, Vector3D referenceVertex, Vector3D upsizedCenterOfGravity, Vector3D downsizedCenterOfGravity, float fallSpeed ){
	// If it's hanging down
	if( vertex.y < referenceVertex.y ){
		// Get it away from the mannequin.
		vertex += ( copy_vertex - downsizedCenterOfGravity ).normalize() * fallSpeed;
	}
	// If it's standing up
	else{
		// Bring it closer to the mannequin.
		vertex += ( upsizedCenterOfGravity - copy_vertex ).normalize() * fallSpeed;
	}
	// Let it go up.
	vertex.y += fallSpeed;
}

__device__ void blowWind( Vector3D &vertex, Vector3D windDirection, float windSpeed, float t ){
	// Linear.
	// vertex += windDirection * windSpeed;

	// Sinusoidal.
	vertex += windDirection * abs( sin( t ) ) * windSpeed;

	// Euler. Not functional.
	// vertex = previousVertex + windDirection * windSpeed * t;

	// Verlet. Not functional.
	// vertex = ( previousVertex - previousPreviousVertex + windDirection * t * t ) * windSpeed;
}

__global__ void tiltForward( Vector3D* dev_vertices, unsigned int vertices_size, float _rotation ){
	// Declarations.
	unsigned int index;
	float cosine, sine, y, z;
	
	index = blockIdx.x * blockDim.x + threadIdx.x;
	if( index < vertices_size ){
		cosine = (float)cos( _rotation * M_PI / 180.0f );
		sine = (float)sin( _rotation * M_PI / 180.0f );
		y = dev_vertices[index].y;
		z = dev_vertices[index].z;
		dev_vertices[index].y = cosine	* y + sine		* z;
		dev_vertices[index].z = -sine	* y + cosine	* z;
	}
}

__global__ void tiltSideways( Vector3D* dev_vertices, unsigned int vertices_size, float _rotation ){
	// Declarations.
	unsigned int index;
	float cosine, sine, x, y;
	
	index = blockIdx.x * blockDim.x + threadIdx.x;
	if( index < vertices_size ){
		cosine = (float)cos( _rotation * M_PI / 180.0f );
		sine = (float)sin( _rotation * M_PI / 180.0f );
		x = dev_vertices[index].x;
		y = dev_vertices[index].y;
		dev_vertices[index].x = cosine	* x + sine		* y;
		dev_vertices[index].y = -sine	* x + cosine	* y;
	}
}

__global__ void kernel( Vector3D *dev_vertices, Vector3D *dev_copy, unsigned int vertices_size, unsigned int hairLength, unsigned int gravity, Vector3D centerOfGravity, Vector3D hairUpsizedCenterOfGravity, Vector3D hairDownsizedCenterOfGravity, Vector3D windDirection, bool wind, float fallSpeed, float windSpeed, float timeSinceLastSimulation ){
	
	// Declarations.
	unsigned int index, remainder;
	
	// Simulation.
	index = blockIdx.x * blockDim.x + threadIdx.x;
//	for( i = index * 32; i < index * 32 + 32; i += 1 )
	if( index < vertices_size ){
		remainder = index % ( hairLength + 1 );
		if( remainder != 0 ){
			if( gravity == NORMAL_GRAVITY )
				fall( dev_vertices[index], dev_copy[index], dev_copy[index - remainder], hairUpsizedCenterOfGravity, hairDownsizedCenterOfGravity, fallSpeed );
			else if( gravity == NEGATIVE_GRAVITY )
				negativeGravity( dev_vertices[index], dev_copy[index], dev_copy[index - remainder], hairUpsizedCenterOfGravity, hairDownsizedCenterOfGravity, fallSpeed );
			if( wind ){
//				delta += DELTA_INCREMENT;
				blowWind( dev_vertices[index], windDirection, windSpeed, timeSinceLastSimulation );
//				if( stuck( vertices[index], previousVertices[index] ) )
//					vertices[i] += ( vertices[index] - centerOfGravity ).normalize() * 0.01f;
			}
			restoreFromCollision( dev_vertices[index], dev_copy[index - 1], dev_copy[index - remainder], centerOfGravity );
		//	previousVertices[index] = vertices[index];
		}
	}
}

__global__ void testKernel( Vector3D *dev_vertices, unsigned int vertices_size, unsigned int hairLength ){
	
	// Declarations.
	unsigned int index, remainder;
	
	// Simulation.
	index = blockIdx.x * blockDim.x + threadIdx.x;
	if( index < vertices_size ){
		remainder = index % ( hairLength + 1 );
		if( remainder != 0 ){
			dev_vertices[index].y += 0.01f;
		}
	}
}

__global__ void helloKernel(){
//	printf( "Hello from kernel!\n" );
}

cudaError_t tiltKernelWrapper( Vector3D *dev_vertices, unsigned int vertices_size, unsigned int tiltDirection ){
	
	// Declarations.
	cudaError_t cudaStatus;
	dim3 gridSize, blockSize;
	unsigned int affinity;
	
	// Launch a kernel on the GPU with one thread for each element.
	affinity = 512;
	gridSize = dim3( 1 + vertices_size / affinity, 1, 1 );
	blockSize = dim3( affinity, 1, 1 );

	switch( tiltDirection ){
	case 1 << TILT_FORWARD:
		tiltForward<<< gridSize, blockSize >>>( dev_vertices, vertices_size, TILT_SPEED );
		break;
	case 1 << TILT_BACKWARDS:
		tiltForward<<< gridSize, blockSize >>>( dev_vertices, vertices_size, -TILT_SPEED );
		break;
	case 1 << TILT_LEFT:
		tiltSideways<<< gridSize, blockSize >>>( dev_vertices, vertices_size, TILT_SPEED );
		break;
	case 1 << TILT_RIGHT:
		tiltSideways<<< gridSize, blockSize >>>( dev_vertices, vertices_size, -TILT_SPEED );
		break;
	}
	cudaStatus = cudaDeviceSynchronize();
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "tiltKernelWrapper()/cudaDeviceSynchronize() failed.", cudaGetErrorString( cudaStatus ) );
	return cudaSuccess;
}

// Helper function for using CUDA to compute stencil operations in parallel.
cudaError_t kernelWrapper( Vector3D *dev_vertices, unsigned int vertices_size, unsigned int hairLength, unsigned int gravity, Vector3D centerOfGravity, Vector3D upsizedCenterOfGravity, Vector3D downsizedCenterOfGravity, Vector3D windDirection, bool wind, float fallSpeed, float windSpeed, float &time, float timeSinceLastSimulation ){
	
	// Declarations.
	cudaError_t cudaStatus;
	dim3 gridSize, blockSize;
	cudaEvent_t start, stop;
	unsigned int affinity;
	
	cudaStatus = cudaEventCreate( &start );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventCreate( &start ) failed.", cudaGetErrorString( cudaStatus ) );

	cudaStatus = cudaEventCreate( &stop );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventCreate( &stop ) failed.", cudaGetErrorString( cudaStatus ) );
	
	cudaStatus = cudaFree( dev_copy );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "kernel()/cudaFree() failed.", cudaGetErrorString( cudaStatus ) );

	cudaStatus = cudaMalloc( &dev_copy, vertices_size * sizeof( Vector3D ) );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "kernel()/cudaMalloc() failed.", cudaGetErrorString( cudaStatus ) );

	cudaStatus = cudaEventRecord( start, 0 );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventRecord( start, 0 ) failed.", cudaGetErrorString( cudaStatus ) );

	cudaMemcpy( dev_copy, dev_vertices, vertices_size * sizeof( Vector3D ), cudaMemcpyDeviceToDevice );
		
	// Launch a kernel on the GPU with one thread for each element.
	affinity = 512;
	gridSize = dim3( 1 + vertices_size / affinity, 1, 1 );
	blockSize = dim3( affinity, 1, 1 );
	
//	sqrt3_affinity = 8;
//	pow_3 = pow( vertices_size, 0.33333333333333333333333333333333333333 );
//	gridSize = dim3( pow_3 / sqrt3_affinity, pow_3 / sqrt3_affinity, pow_3 / sqrt3_affinity );
//	blockSize = dim3( sqrt3_affinity, sqrt3_affinity, sqrt3_affinity );

//	gridSize = dim3( 1, 1, 1 );
//	blockSize = dim3( 1, 1, 1 );

	//helloKernel<<< gridSize, blockSize >>>();
//	testKernel<<< gridSize, blockSize >>>( dev_vertices, vertices_size, hair.length );
	kernel<<< gridSize, blockSize >>>( dev_vertices, dev_copy, vertices_size, hairLength, gravity, centerOfGravity, upsizedCenterOfGravity, downsizedCenterOfGravity, windDirection, wind, fallSpeed, windSpeed, timeSinceLastSimulation );

	cudaStatus = cudaDeviceSynchronize();
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "kernelWrapper()/cudaDeviceSynchronize() failed.", cudaGetErrorString( cudaStatus ) );

	cudaStatus = cudaEventRecord( stop, 0 );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventRecord( stop, 0 ) failed.", cudaGetErrorString( cudaStatus ) );
	
	cudaStatus = cudaEventSynchronize( stop );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventSynchronize()1 failed.", cudaGetErrorString( cudaStatus ) );

	cudaStatus = cudaEventElapsedTime( &time, start, stop );
	if( cudaStatus != cudaSuccess )
		errorHandler( cudaStatus, "cudaEventElapsedTime() failed.", cudaGetErrorString( cudaStatus ) );
	
//	cudaMemcpy( dev_vertices, dev_copy, vertices_size * sizeof( Vector3D ), cudaMemcpyDeviceToDevice );

		
	return cudaSuccess;
}
