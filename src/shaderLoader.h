#ifndef _SHADER_LOADER_HPP
#define _SHADER_LOADER_HPP

#include "HeadersAndDefines.h"

unsigned int
loadShader(const std::string &vertex_shader_file,
           const std::string &fragment_shader_file);
unsigned int
loadShader(const std::string &vertex_shader_file,
           const std::string &geometry_shader_file,
           const std::string &fragment_shader_file);
unsigned int
_createShader(const std::string &shader_file, GLenum shader_type);
unsigned int
_createProgram(const std::vector<unsigned int> &shader_objects);

#endif
