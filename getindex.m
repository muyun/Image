clear, clc
%
%dirname = 'E:\wenlong\documents\proj\iris\test\result\S*.txt';
%dirname = 'E:\wenlong\documents\proj\iris\lamp\CASIA-Iris-Lamp_\';
dirname = 'E:\wenlong\documents\proj\iris\CASIA-Iris-Interval\';
 files = getAllFiles(dirname);
%files = dir(dirname);
files_n = numel(files);
nuser = 200;
 nsamp = 2;
 ncandid = 30;
 
% copy the files in the same dir
 for i = 1 : files_n
     %filename =  strcat('E:\wenlong\documents\proj\iris\test\result\',  files(i).name);
     file = strjoin(files(i));
     copyfile(file, 'E:\wenlong\documents\proj\iris\CASIA-Iris-Interval_\');
 end
 
%  for i = 1 : files_n
%      filename = strcat('E:\wenlong\documents\proj\iris\test\result\',  files(i).name);
%      % im = imread(filename);
%       %filename = 'E:\wenlong\documents\proj\iris\test\result\S1200R01_.compare.txt';
%       
%       %name = 'E:\wenlong\documents\proj\iris\test\S1010L01.compare.txt';
%         fid = fopen(filename, 'rt');
%         m = 0;
%         %nuser = 20;
%        
%          hamm = zeros(nuser, 1);
%         while feof(fid) == 0
%                 m = m+1;
%                 tline = fgetl(fid);
%                 S = regexp(tline, ' ', 'split');
% 
%                 id = char(regexp(S{2}, '\d*', 'match'));
% 
%                 user = str2num(id(1,:)) - 1000;
%                 samp = str2num(id(2,:));
% 
%                  if samp == 2,
%                       val = S{3};
%                         hamm(user) = str2double(val);
%                  end
% 
%         end
% 
%         fprintf('\n');
%          [v, ind] = sort(hamm);
% 
%          %save in the file
%          fid_ = fopen('E:\wenlong\documents\proj\iris\test\result\index.txt', 'a');
%          fprintf(fid_, '%d ', ind);
%          fprintf(fid_, '\n');
% 
%          fprintf('\n');
%          fclose(fid_);
%          fclose(fid);
%          
%  end

 
% hammhit = zeros(1, ncandid);
%  hier_data = dlmread('E:\wenlong\documents\proj\iris\test\result\index.txt');
%  for k = 1 : nuser
%       kshit = (hier_data(k,:) == k);
%       if any(kshit),
%           hammhit(kshit) = hammhit(kshit) + 1;
%       end
%  end
%  
% hammhitrate = cumsum(hammhit) /nuser;
% 
% figure(2); hold on;
% title('The Indexing Performance');
% axis([1, ncandid, 60, 100]);
% plot(hammhitrate*100, 'k*--', 'MarkerSize', 8);
% %plot(hierhitrate*100, 'r.--', 'MarkerSize',8);
% xlabel('Penetration Rate (%)', 'FontSize', 14);
% ylabel('Hit Rate (%)', 'FontSize', 14);
% 
% h = legend('Hamming',1, 'Location', 'southeast');
% set(h, 'FontSize', 10, 'position', [0.7, 0.12, 0.2, 0.2]);
% hold off;
 
 
% name = 'E:\wenlong\documents\proj\iris\test\S1010L01.compare.txt';
% fid = fopen(name, 'rt');
% m = 0;
% nuser = 15;
% nsamp = 2;
%  hamm = zeros(nuser, 1);
% while feof(fid) == 0
%     m = m+1;
%     tline = fgetl(fid);
%     S = regexp(tline, ' ', 'split');
%     
%     id = char(regexp(S{2}, '\d*', 'match'));
%      
%     user = str2num(id(1,:)) - 1000;
%     samp = str2num(id(2,:));
%       
%      if samp == 2,
%           val = S{3};
%             hamm(user) = str2double(val);
%      end
%      
% end
% 
% fprintf('\n');
%  [v, ind] = sort(hamm);
%  
%  fid_ = fopen('index.txt', 'a');
%  fprintf(fid_, '%d ', ind);
%  fprintf(fid_, '\n');
%  
%  fprintf('\n');
%  fclose(fid_);
%  fclose(fid);
 