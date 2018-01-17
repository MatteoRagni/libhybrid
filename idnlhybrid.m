function model = idnlhybrid(modelname, order, params, ts, varargin)
%IDNLHYBRID Builds the Hybrid non linear model as an idnlgrey

  % Argument Parsing
  p = inputParser;

  validModelName = @(x) validateattributes(x, {'char'}, {'nonempty'});
  validOrder = @(x) validateattributes(x, {'numeric'}, {'numel', 3});
  validTs = @(x) true; % validateattributes(x, {'numeric'}, {'>', 0});
  validBool = @(x) isa(x, 'logical');
  validJumpCond = @(x) validatestring(x, {'C', 'D'});

  addRequired(p, 'modelname', validModelName);
  addRequired(p, 'order', validOrder);
  addRequired(p, 'params');
  addRequired(p, 'ts', validTs);

  addParameter(p, 'ics', []);
  addParameter(p, 'path', './libhybrid', validModelName);
  addParameter(p, 'jumpLogic', 'D', validJumpCond);
  addParameter(p, 'forceCompile', false, validBool);

  parse(p, modelname, order, params, ts, varargin{:});
  args = p.Results;

  % Checking sources and compiling
  model_source = sprintf('%s.c', args.modelname);
  if idnlhybrid_check_sources(model_source, args) || args.forceCompile
    idnlhybrid_compile(model_source, args);
  end

  if ~exist(sprintf(modelname, mexext()), 'file')
    error('LIBHYBRID:Idnlhybrid:ModelNotFound', 'File %s not found. Must be compiled', source);
  end

  % Fixing intitial conditions and order of the model 
  order_exp = args.order;
  order_exp(3) = order_exp(3) + 2;
  if ~isempty(args.ics)
    if isa(args.ics, 'struct')
      ics_exp(1).Name = 'Flow Time';
      ics_exp(1).Value = 0.0;
      ics_exp(1).Fixed = true;

      ics_exp(2).Name = 'Jump Time';
      ics_exp(2).Value = 0.0;
      ics_exp(2).Fixed = true;

      for i = 1:length(args.ics)
        ics_exp(i + 2) = args.ics(i);
      end
    elseif isa(args.ics, 'numeric')
      ics_exp = [zeros(1, size(args.ics,2)); zeros(1, size(args.ics,2)); args.ics];
    else
      error('LIBHYBRID:Idnlhybrid:InitialConditions', 'Unknown initial conditions type');
    end
  end
  % Creating idnlgrey object with hybrid model
  model = idnlgrey(args.modelname, order_exp, args.params, ics_exp, args.ts);
end

function compile = idnlhybrid_check_sources(model_source, args)
%IDNLHYBRID_CHECK_SOURCES Check that all files used in compilation exist
  if ~exist(sprintf('%s.%s', args.modelname, mexext()), 'file')
    compile = true;
    return
  end
  sources = { ...
    model_source, ...
    sprintf('%s/libhybrid.h', args.path), ...
    sprintf('%s/libhybrid.c', args.path), ...
    sprintf('%s/librk4/librk4.h', args.path), ...
    sprintf('%s/librk4/librk4.c', args.path), ...
    sprintf('%s/mex_wrapper.c', args.path), ...
  };
  object_dir = dir(sprintf('%s.%s', args.modelname, mexext()));
  for i = 1:length(sources)
    source = sources{i};
    if ~exist(source, 'file')
      error('LIBHYBRID:Idnlhybrid:FileNotFound', 'File %s not found. Check the path!', source);
    end
  end
  compile = false;
  sources_dir = cellfun(@dir, sources);
  model_time = object_dir.datenum;
  for i = 1:length(sources_dir)
    compile = compile || sources_dir(i).datenum > model_time;
  end
end



function idnlhybrid_compile(model_source, args)
%IDNLHYBRID_COMPILE compiles the mex file
  jump_logic = 1;
  if (args.jumpLogic == 'C')
    jump_logic = 2;
  end

  source_librk4 = sprintf('%s/librk4/librk4.c', args.path);
  source_libhybrid = sprintf('%s/libhybrid.c', args.path);
  source_mexwrapper = sprintf('%s/mex_wrapper.c', args.path);
  mex('-DMATLAB_WRAPPER', ...
      '-DMATLAB_SYSTEM_IDENTIFICATION', ...
      sprintf('-DHYB_MODEL_SOURCE="\\"%s\\""', model_source), ...
      sprintf('-DHYB_JUMP_LOGIC=%d', jump_logic), ...
      source_librk4, source_libhybrid, source_mexwrapper, ...
      '-output', args.modelname);
end
