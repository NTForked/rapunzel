/*------------------------------------------------------------------------------
/                             Proiect de diplomã
/
/                            Pavel Andrei-Cristian
/                               341C1, ACS, UPB
/
/                                 BMP Loader
/-------------------------------------------------------------------------------
*/

#include "bmpLoader.h"

bmpLoader::bmpLoader() {
}

std::string
bmpLoader::loadBMP(std::string textureFilename) {
    // Declarations.
    int offsetToPixelArray, size, i;
    std::ifstream input;
    char byte[54], tmp;

    input.open(textureFilename, std::ios::in | std::ios::binary);
    if (!input.is_open())
        errorHandler(ERROR_CODE5,
                     std::string("ERROR 5: BMP file " + textureFilename +
                                 " doesn't exist."),
                     "@todo: GetLastErrorAsString()");

    input.read(byte, 54 * sizeof(char));
    offsetToPixelArray = byte[10];
    width = byte[18];
    height = byte[22];
    size = 3 * width * height;
    data.reserve(size);
    input.seekg(offsetToPixelArray, std::ios::beg);
    for (i = 0; i < size; i += 1) {
        input.read(&tmp, sizeof(char));
        data += tmp;
        if (i % 3 == 2) {
            tmp = data[i - 2];
            data[i - 2] = data[i];
            data[i] = tmp;
        }
    }
    input.close();
    return data;
}
