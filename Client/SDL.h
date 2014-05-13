#ifndef SDLCLASS_HPP
#define SDLCLASS_HPP
#include <exception>
#include <string>
#include <SDL2/SDL.h>

/**
 * Taken from http://wiki.libsdl.org/SDL_Init?highlight=%28\bCategoryAPI\b%29|%28SDLFunctionTemplate%29#C.2B-.2B-
 */

class InitError: public std::exception {
	public:
		InitError();
		InitError(const std::string&);
		virtual ~InitError() throw();
		virtual const char* what() const throw();
	private:
		std::string msg;
};

class SDL {
	public:
		SDL(Uint32 flags = 0) throw(InitError);
		virtual ~SDL();
};
#endif
