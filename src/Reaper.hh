#ifndef REAPER_HH
#define REAPER_HH

#include "GLContext.hh"
#include "GLErrorLogger.hpp"
#include "Renderer/PostProcessor.hpp"
#include "Joystick/SixAxis.hpp"
#include "Camera/Camera.hpp"

class Reaper
{
public:
  Reaper(GLContext& context);
  virtual ~Reaper();

public:
  void		run();

private:
  GLContext&	_context;
  GLErrorLogger	_errorLogger;
  PostProcessor	_postProcessor;
  SixAxis	_controller;
  Camera	_camera;
};

#endif // REAPER_HH
