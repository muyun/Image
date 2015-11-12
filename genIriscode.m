clear, clc
% generate the iris code for the comparator
comm = 'E:\wenlong\documents\proj\iris\test\hd -i S1001R01_code.png  *_code.png -m S1010R01_codemask.tiff ?1_codemask.tiff  -s -8 8 -a minhd -o S1010L01_.compare.txt';
[status, cmdout] = dos(comm, '-echo');

% read the texture files
nsamp = 2;
nuser = 20;

% %name = ls('E:\wenlong\cs\iris\result\code\*.png');
dirname = 'E:\wenlong\documents\proj\iris\test\S*';
% files = getAllFiles(dirname);
files = dir(dirname);
files_n = numel(files);
% 
% %iris = cell(nuser, nsamp);
 %iriscode = zeros( 1280*8, nsamp, nuser);
 irismask = zeros(1280*8, nsamp, nuser);
% %nuser = 0;
% copy the images to the defined dir
% folder = 'E:\wenlong\cs\iris\CASIA-Iris-Interval_';
% for i = 1 : files_n
%       %filename = strcat('E:\wenlong\cs\iris\result\',  files(i).name);
%       copyfile(files{i}, folder);
% end
      
%name = zeros(240, 7);
 for i = 1 : files_n
     filename = strcat('E:\wenlong\documents\proj\iris\test\',  files(i).name);
      im = imread(filename);

      % the user id
      id = char(regexp(files(i).name, '\d*', 'match'));
      user = str2num(id(1,:)) - 1000;
      %sp = str2num(id(2,:));

      %the sample
      samp = mod(i, nsamp);
      if  samp == 0,
          samp = nsamp;
      end
           
     % name(user, samp) = 1; 
      % iris-code in binary format
       bin = reshape((dec2bin(typecast(im(:),'uint8'),8)-'0').',1,[]);
      
       %iriscode(:,  samp, user) = bin;
      irismask(:,  samp, user) = bin;
%       if samp == 7, % next user
%           iriscode = cat(user, iris);
%       end
      
end
%bin(user, samp);
%iris= cat(1, bin{:});
% store
fprintf('\n');
% fid = fopen('E:\wenlong\cs\iris\result\iris2_texture.bin', 'w');
% fprintf(fid, '%s', char(B));
% fclose(fid);
