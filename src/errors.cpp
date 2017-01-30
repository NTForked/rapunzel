/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                             Headers And Defines
/-------------------------------------------------------------------------------
*/

#include "HeadersAndDefines.h"

// Handle any type of errors given the error number and the readable error
// string.
void
errorHandler(int /* error */,
             std::string /* personalizedText */,
             std::string /* officialText */) {
    /*
    HANDLE hStdout;
    // @todo: hStdout = GetStdHandle( STD_OUTPUT_HANDLE );
    // @todo: SetConsoleTextAttribute( hStdout, FOREGROUND_RED |
    FOREGROUND_INTENSITY );
    if( error == NULL )
        std::cerr << "ERROR N/A: ";
    else
        std::cerr << "ERROR " << error << ": ";
    if( personalizedText.empty() )
        std::cerr << "N/A: ";
    else
        std::cerr << personalizedText << ": ";
    if( officialText.empty() )
        std::cerr << "N/A" << std::endl;
    else
        std::cerr << officialText << std::endl;
    SetConsoleTextAttribute( hStdout, FOREGROUND_RED | FOREGROUND_GREEN |
    FOREGROUND_BLUE | FOREGROUND_INTENSITY );
    std::cin >> personalizedText;
    exit( error );
    */
}

#ifdef __CUDA_ARCH__
// Get CUDA error in readable format.
std::string
cuGetErrorString(CUresult cudaResult) {
    switch (cudaResult) {
    case CUDA_SUCCESS:
        return std::string("CUDA_SUCCESS");
    case CUDA_ERROR_INVALID_VALUE:
        return std::string("CUDA_ERROR_INVALID_VALUE");
    case CUDA_ERROR_OUT_OF_MEMORY:
        return std::string("CUDA_ERROR_OUT_OF_MEMORY");
    case CUDA_ERROR_NOT_INITIALIZED:
        return std::string("CUDA_ERROR_NOT_INITIALIZED");
    case CUDA_ERROR_DEINITIALIZED:
        return std::string("CUDA_ERROR_DEINITIALIZED");
    case CUDA_ERROR_PROFILER_DISABLED:
        return std::string("CUDA_ERROR_PROFILER_DISABLED");
    case CUDA_ERROR_PROFILER_NOT_INITIALIZED:
        return std::string("CUDA_ERROR_PROFILER_NOT_INITIALIZED");
    case CUDA_ERROR_PROFILER_ALREADY_STARTED:
        return std::string("CUDA_ERROR_PROFILER_ALREADY_STARTED");
    case CUDA_ERROR_PROFILER_ALREADY_STOPPED:
        return std::string("CUDA_ERROR_PROFILER_ALREADY_STOPPED");
    case CUDA_ERROR_NO_DEVICE:
        return std::string("CUDA_ERROR_NO_DEVICE");
    case CUDA_ERROR_INVALID_DEVICE:
        return std::string("CUDA_ERROR_INVALID_DEVICE");
    case CUDA_ERROR_INVALID_IMAGE:
        return std::string("CUDA_ERROR_INVALID_IMAGE");
    case CUDA_ERROR_INVALID_CONTEXT:
        return std::string("CUDA_ERROR_INVALID_CONTEXT");
    case CUDA_ERROR_CONTEXT_ALREADY_CURRENT:
        return std::string("CUDA_ERROR_CONTEXT_ALREADY_CURRENT");
    case CUDA_ERROR_MAP_FAILED:
        return std::string("CUDA_ERROR_MAP_FAILED");
    case CUDA_ERROR_UNMAP_FAILED:
        return std::string("CUDA_ERROR_UNMAP_FAILED");
    case CUDA_ERROR_ARRAY_IS_MAPPED:
        return std::string("CUDA_ERROR_ARRAY_IS_MAPPED");
    case CUDA_ERROR_ALREADY_MAPPED:
        return std::string("CUDA_ERROR_ALREADY_MAPPED");
    case CUDA_ERROR_NO_BINARY_FOR_GPU:
        return std::string("CUDA_ERROR_NO_BINARY_FOR_GPU");
    case CUDA_ERROR_ALREADY_ACQUIRED:
        return std::string("CUDA_ERROR_ALREADY_ACQUIRED");
    case CUDA_ERROR_NOT_MAPPED:
        return std::string("CUDA_ERROR_NOT_MAPPED");
    case CUDA_ERROR_NOT_MAPPED_AS_ARRAY:
        return std::string("CUDA_ERROR_NOT_MAPPED_AS_ARRAY");
    case CUDA_ERROR_NOT_MAPPED_AS_POINTER:
        return std::string("CUDA_ERROR_NOT_MAPPED_AS_POINTER");
    case CUDA_ERROR_ECC_UNCORRECTABLE:
        return std::string("CUDA_ERROR_ECC_UNCORRECTABLE");
    case CUDA_ERROR_UNSUPPORTED_LIMIT:
        return std::string("CUDA_ERROR_UNSUPPORTED_LIMIT");
    case CUDA_ERROR_CONTEXT_ALREADY_IN_USE:
        return std::string("CUDA_ERROR_CONTEXT_ALREADY_IN_USE");
    case CUDA_ERROR_INVALID_SOURCE:
        return std::string("CUDA_ERROR_INVALID_SOURCE");
    case CUDA_ERROR_FILE_NOT_FOUND:
        return std::string("CUDA_ERROR_FILE_NOT_FOUND");
    case CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND:
        return std::string("CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND");
    case CUDA_ERROR_SHARED_OBJECT_INIT_FAILED:
        return std::string("CUDA_ERROR_SHARED_OBJECT_INIT_FAILED");
    case CUDA_ERROR_OPERATING_SYSTEM:
        return std::string("CUDA_ERROR_OPERATING_SYSTEM");
    case CUDA_ERROR_INVALID_HANDLE:
        return std::string("CUDA_ERROR_INVALID_HANDLE");
    case CUDA_ERROR_NOT_FOUND:
        return std::string("CUDA_ERROR_NOT_FOUND");
    case CUDA_ERROR_NOT_READY:
        return std::string("CUDA_ERROR_NOT_READY");
    case CUDA_ERROR_LAUNCH_FAILED:
        return std::string("CUDA_ERROR_LAUNCH_FAILED");
    case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
        return std::string("CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES");
    case CUDA_ERROR_LAUNCH_TIMEOUT:
        return std::string("CUDA_ERROR_LAUNCH_TIMEOUT");
    case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING:
        return std::string("CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING");
    case CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED:
        return std::string("CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED");
    case CUDA_ERROR_PEER_ACCESS_NOT_ENABLED:
        return std::string("CUDA_ERROR_PEER_ACCESS_NOT_ENABLED");
    case CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE:
        return std::string("CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE");
    case CUDA_ERROR_CONTEXT_IS_DESTROYED:
        return std::string("CUDA_ERROR_CONTEXT_IS_DESTROYED");
    case CUDA_ERROR_UNKNOWN:
        return std::string("CUDA_ERROR_UNKNOWN");
    default:
        return std::string("cuGetErrorString() doesn't recognize error.");
    }
}
#endif
