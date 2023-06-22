function showallmeshes(directory)
  % function showallmeshes
  %
  % Load and show all meshes in the given directory (current if empty)

  if nargin == 0
    directory = pwd;
  end
  filelist = dir(directory);
  for index = 1:length(filelist)
    [~,~,extension] = fileparts(filelist(index).name);
    if strcmp(extension,'.mesh')
      meshfilepath = fullfile(filelist(index).folder,filelist(index).name);
      mesh = loadmesh(meshfilepath);
      showmesh(mesh);
      title(filelist(index).name,'Interpreter','none')
    end
  end
end