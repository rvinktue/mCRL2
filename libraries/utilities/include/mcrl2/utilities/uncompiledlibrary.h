#ifndef __UNCOMPILED_LIBRARY_H
#define __UNCOMPILED_LIBRARY_H

/*
 * Extends the dynamic_library from dynamiclibrary.h to be able to compile a
 * source file and load the resulting library.
 *
 * Usage:
 *
 *   uncompiled_library mylib;
 *   mylib.compile(source_filename);
 *   myfunc = mylib.proc_address("myfunc");
 *   myfunc(10);
 *
 * Remarks:
 *
 * The source is compiled using a script that must take two string arguments.
 * The first argument is the source file, the second is the destination file.
 * After (successful) termination, only the source and destination files must
 * remain on disk -- it is the responsibility of the script to remove any
 * temporary files.
 *
 */

#include <sstream>
#include <stdexcept>
#include "dynamiclibrary.h"
#include "mcrl2/setup.h"

class uncompiled_library : public dynamic_library
{
private:
    std::string m_compile_script;
    std::string m_source_filename;
    std::string m_object_filename;

public:
    uncompiled_library(const std::string& script = "mcrl2compilerewriter") : m_compile_script(script) {};

    /// \overload
    /// Needed in order to properly call the uncompiled_library::unload!
    virtual ~uncompiled_library()
    {
      try
      {
        unload();
      }
      catch(std::runtime_error)
      {
        // Ignore
      }
    }

    /*
    void compile(const std::string& filename) throw(std::runtime_error)
    {
      std::stringstream commandline;
      commandline << m_compile_script << " " << filename << " " << filename << ".bin";
      std::cout << commandline << std::endl;
      int r = system(commandline.str().c_str());
      if (r != 0)
      {
        std::stringstream s;
        s << "Executing compile script failed, return code was " << std::hex << r;
        throw std::runtime_error(s.str());
      }
      m_filename = std::string("./") + filename + ".bin";
    }
    */

    void compile(const std::string& filename) throw(std::runtime_error)
    {
      m_source_filename = filename;
      m_object_filename = filename + ".o";

      std::stringstream compilecommandline;
      std::stringstream linkcommandline;

      compilecommandline << CXX << " -c " << CXXFLAGS << " " << SCXXFLAGS << " " << CPPFLAGS << " " << ATERM_CPPFLAGS << " -o " << filename << ".o " << filename;
      linkcommandline << CXX << " " << LDFLAGS << " " << SLDFLAGS << " -o " << filename << ".bin " << filename << ".o";

      int r = system(compilecommandline.str().c_str());
      if (r != 0)
      {
        std::stringstream s;
        s << "Compilation failed, return code was " << std::hex << r;
        s << " compile command was " << compilecommandline.str();
        throw std::runtime_error(s.str());
      }

      r = system(linkcommandline.str().c_str());
      if (r != 0)
      {
        std::stringstream s;
        s << "Linking failed, return code was " << std::hex << r;
        throw std::runtime_error(s.str());
      }

      m_filename = std::string("./") + filename + ".bin";
    }

    virtual void unload() throw(std::runtime_error)
    {
      dynamic_library::unload();
      if (!m_source_filename.empty())
      {
        if (unlink(m_source_filename.c_str()) != 0)
        {
          std::stringstream s;
          s << "Could not remove file: " << m_source_filename;
          throw std::runtime_error(s.str());
        }
      }

      if (!m_object_filename.empty())
      {
        if (unlink(m_object_filename.c_str()) != 0)
        {
          std::stringstream s;
          s << "Could not remove file: " << m_object_filename;
          throw std::runtime_error(s.str());
        }
      }

      if (!m_filename.empty())
      {
        if (unlink(m_filename.c_str()) != 0)
        {
          std::stringstream s;
          s << "Could not remove file: " << m_filename;
          throw std::runtime_error(s.str());
        }
      }
    }
};

#endif // __UNCOMPILED_LIBRARY_H
