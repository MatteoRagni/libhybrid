function model = idnlhybrid(filename, order, parameters, ics, Ts)
%IDNLHYBRID Builds the Hybrid non linear model as an idnlgrey
%
% Input for the non linear model are:
%  * filename: will be used to compile the mex function that will be
%    included in the model
%  * order: the order of the model, an array that contains [Ny, Nu, Nx]
%    will be changed to [Ny, Nu, Nx + 2]
%  * parameters: the parameters for the model as described in idnlgrey doc
%  * ics: initial conditions as described in idnlgrey doc
%  * Ts: Integration step size

  other_sources = { ...
    'libhybrid.c', ...
    'libhybrid.h', ...
    'librk4/librk4.h', ...
    'librk4/librk4.c', ...
    'mex_wrapper.c', ...
  }

end

function ret = idnlhybrid_should_compile(object, sources)
  filename_data = dir(filename);
  if isempty(filename_data)
    ret = true;
    return;
  end

  filename_data = filename.datenum;
  other_sources_data = [];
  @(x)

end
