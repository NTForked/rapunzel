/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 BMP Loader
/-------------------------------------------------------------------------------
*/

#ifndef BMP_LOADER_H
#define BMP_LOADER_H

#pragma once

#include "HeadersAndDefines.h"

class bmpLoader{
public:
    std::string data;
    int width, height;

    // Constructor.
    bmpLoader();

    std::string loadBMP( std::string textureFilename );
};

#endif
